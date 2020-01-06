#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#include "solo5.h"

/* imported from http://xenbits.xenproject.org/gitweb/?p=mini-os.git;a=blob;f=mm.c;h=932ceebc2867a3a2cdf982751d6ce70f4e195741;hb=refs/heads/master */

#if defined(__x86_64__)
#define PAGE_SHIFT  12
#else
#error "Unknown architecture"
#endif
#define PAGE_SIZE   (1UL << PAGE_SHIFT)
#define PAGE_MASK   (~(PAGE_SIZE - 1))

#define PHYS_PFN(x) ((unsigned long)(x) >> PAGE_SHIFT)

#define ROUND_PGDOWN(_p)  ((_p) & PAGE_MASK)
#define ROUND_PGUP(_p)    (((_p) + (PAGE_SIZE - 1)) & PAGE_MASK)

/*
 * ALLOCATION BITMAP
 * One bit per page of memory. Bit set => page is allocated.
 */

static uintptr_t *mm_alloc_bitmap;
static size_t mm_alloc_bitmap_size;
static size_t nr_free_pages;

#define PAGES_PER_MAPWORD (sizeof(unsigned long) * 8)

#define ALLOCATED_IN_MAP(_pn) \
  (mm_alloc_bitmap[(_pn) / PAGES_PER_MAPWORD] & \
   (1UL << ((_pn) & (PAGES_PER_MAPWORD - 1))))

static inline int chk_free_pages(size_t needed)
{
  return needed <= nr_free_pages;
}

static inline int get_order(size_t size)
{
  int order;
  size = (size - 1) >> PAGE_SHIFT;
  for (order = 0; size; order++)
    size >>= 1;
  return order;
}

/*
 * Hint regarding bitwise arithmetic in map_{alloc,free}:
 *  -(1<<n)  sets all bits >= n.
 *  (1<<n)-1 sets all bits <  n.
 * Variable names in map_{alloc,free}:
 *  *_idx == Index into `mm_alloc_bitmap` array.
 *  *_off =- Bit offset within an element of the `mm_alloc_bitmap` array.
 */

static void map_alloc(uintptr_t first_page, size_t nr_pages)
{
  unsigned long start_off, end_off, curr_idx, end_idx;

  curr_idx  = first_page / PAGES_PER_MAPWORD;
  start_off = first_page & (PAGES_PER_MAPWORD - 1);
  end_idx   = (first_page + nr_pages) / PAGES_PER_MAPWORD;
  end_off   = (first_page + nr_pages) & (PAGES_PER_MAPWORD - 1);

  if (curr_idx == end_idx) {
    mm_alloc_bitmap[curr_idx] |= ((1UL << end_off) - 1) & -(1UL << start_off);
  } else {
    mm_alloc_bitmap[curr_idx] |= -(1UL << start_off);
    while (++curr_idx < end_idx)
      mm_alloc_bitmap[curr_idx] = ~0;
    mm_alloc_bitmap[curr_idx] |= (1UL << end_off) - 1;
  }

  nr_free_pages -= nr_pages;
}

static void map_free(uintptr_t first_page, size_t nr_pages)
{
  unsigned long start_off, end_off, curr_idx, end_idx;

  curr_idx  = first_page / PAGES_PER_MAPWORD;
  start_off = first_page & (PAGES_PER_MAPWORD - 1);
  end_idx   = (first_page + nr_pages) / PAGES_PER_MAPWORD;
  end_off   = (first_page + nr_pages) & (PAGES_PER_MAPWORD - 1);

  nr_free_pages += nr_pages;

  if (curr_idx == end_idx) {
    mm_alloc_bitmap[curr_idx] &= -(1UL << end_off) | ((1UL << start_off) - 1);
  } else {
    mm_alloc_bitmap[curr_idx] &= (1UL << start_off) - 1;
    while (++curr_idx < end_idx)
      mm_alloc_bitmap[curr_idx] = 0;
    mm_alloc_bitmap[curr_idx] &= -(1UL << end_off);
  }
}

/*
 * BINARY BUDDY ALLOCATOR
 */

typedef struct chunk_head_st chunk_head_t;
typedef struct chunk_tail_st chunk_tail_t;

struct chunk_head_st {
  chunk_head_t  *next;
  chunk_head_t **pprev;
  unsigned       level;
};

struct chunk_tail_st {
  unsigned level;
};

/* Linked lists of free chunks of different powers-of-wto in size. */
#define FREELIST_SIZE ((sizeof(void *) << 3) - PAGE_SHIFT)
static chunk_head_t *free_head[FREELIST_SIZE];
static chunk_head_t  free_tail[FREELIST_SIZE];
#define FREELIST_EMPTY(_l) ((_l)->next == NULL)

/*
 * Initialize allocator, placing addresses [@min,@max] in free pool.
 * @min and @max are PHYSICAL addresses.
 */

static void init_page_allocator(uintptr_t min, uintptr_t max)
{
  unsigned i;
  unsigned long range;
  unsigned long r_min, r_max;
  chunk_head_t *ch;
  chunk_tail_t *ct;

  for (i = 0; i < FREELIST_SIZE; i++) {
    free_head[i]       = &free_tail[i];
    free_tail[i].pprev = &free_head[i];
    free_tail[i].next  = NULL;
  }

  min = ROUND_PGUP(min);
  max = ROUND_PGDOWN(max);

  /* Allocate space for the allocation bitmap. */
  mm_alloc_bitmap_size = (max + 1) >> (PAGE_SHIFT + 3);
  mm_alloc_bitmap_size = ROUND_PGUP(mm_alloc_bitmap_size);
  mm_alloc_bitmap = (unsigned long *)min;
  min += mm_alloc_bitmap_size;

  /* All allocated by default. */
  memset(mm_alloc_bitmap, ~0, mm_alloc_bitmap_size);

  /* Add all heap space passed from solo5 tender */
  r_min = (unsigned long)min;
  r_max = (unsigned long)max;
  range = r_max - r_min;

  /* Free up the memory we've been given to play with. */
  map_free(PHYS_PFN(r_min), range >> PAGE_SHIFT);

  while (range != 0) {
    /*
     * Next chunk is limited by alignment of min, but also
     * must not be bigger then remaining range.
     */
    for (i = PAGE_SHIFT; (1UL << (i + 1)) <= range; i++)
      if (r_min & (1UL << i))
        break;

    ch = (chunk_head_t *)r_min;
    r_min += 1UL << i;
    range -= 1UL << i;
    ct = (chunk_tail_t *)r_min - 1;
    i -= PAGE_SHIFT;
    ch->level       = i;
    ch->next        = free_head[i];
    ch->pprev       = &free_head[i];
    ch->next->pprev = &ch->next;
    free_head[i]    = ch;
    ct->level       = i;
  }
}

/* Allocate 2^@order contiguos pages. Returns a VIRTUAL address. */
static uintptr_t alloc_pages(unsigned order)
{
  unsigned i;
  chunk_head_t *alloc_ch, *spare_ch;
  chunk_tail_t            *spare_ct;

  if (!chk_free_pages(1UL << order))
    goto no_memory;

  /* Find smallest order with can satisfy the request. */
  for (i = order; i < FREELIST_SIZE; i++) {
    if (!FREELIST_EMPTY(free_head[i]))
      break;
  }

  if (i == FREELIST_SIZE)
    goto no_memory;

  /* Unlink a chunk. */
  alloc_ch = free_head[i];
  free_head[i] = alloc_ch->next;
  alloc_ch->next->pprev = alloc_ch->pprev;

  /* We may have to break the chunk a number of times. */
  while (i != order) {
    /* Split into two equal parts. */
    i--;
    spare_ch = (chunk_head_t *)((char *)alloc_ch + (1UL << (i + PAGE_SHIFT)));
    spare_ct = (chunk_tail_t *)((char *)spare_ch + (1UL << (i + PAGE_SHIFT))) - 1;
    /* Create new header for spare chunk. */
    spare_ch->level = i;
    spare_ch->next  = free_head[i];
    spare_ch->pprev = &free_head[i];
    spare_ct->level = i;

    /* Link in the spare chunk. */
    spare_ch->next->pprev = &spare_ch->next;
    free_head[i] = spare_ch;
  }

  map_alloc(PHYS_PFN(alloc_ch), 1UL << order);

  return (uintptr_t)alloc_ch;

no_memory:

  return 0;
}

static void free_pages(void *pointer, unsigned order)
{
  chunk_head_t *freed_ch, *to_merge_ch;
  chunk_tail_t *freed_ct;
  unsigned long mask;

  /* First free the chunk */
  map_free(PHYS_PFN(pointer), 1UL << order);

  /* Create free chunk */
  freed_ch = (chunk_head_t *)pointer;
  freed_ct = (chunk_tail_t *)((char *)pointer + (1UL << (order + PAGE_SHIFT))) - 1;

  /* Now, possibly we can conceal chunks together */
  while (order < FREELIST_SIZE) {
    mask = 1UL << (order + PAGE_SHIFT);
    if ((unsigned long)freed_ct & mask) {
      to_merge_ch = (chunk_head_t *)((char *)freed_ch - mask);
      if (ALLOCATED_IN_MAP(PHYS_PFN(to_merge_ch)) ||
          to_merge_ch->level != order)
        break;

      /* Merge with predecessor */
      freed_ch = to_merge_ch;
    } else {
      to_merge_ch = (chunk_head_t *)((char *)freed_ch + mask);
      if (ALLOCATED_IN_MAP(PHYS_PFN(to_merge_ch)) ||
          to_merge_ch->level != order)
        break;

      /* Merge with successor */
      freed_ct = (chunk_tail_t *)((char *)to_merge_ch + mask) - 1;
    }

    /* We are commited to merging, unlink the chunk. */
    *(to_merge_ch->pprev) = to_merge_ch->next;
    to_merge_ch->next->pprev = to_merge_ch->pprev;

    order++;
  }

  /* Link the new chunk. */
  freed_ch->level = order;
  freed_ch->next  = free_head[order];
  freed_ch->pprev = &free_head[order];
  freed_ct->level = order;

  freed_ch->next->pprev = &freed_ch->next;
  free_head[order] = freed_ch;
}

void solo5_init_mm(const struct solo5_start_info *info)
{
  uintptr_t solo5_heap_start, solo5_heap_end;

  if (!info)
    return;

  solo5_heap_start = ROUND_PGUP(info->heap_start);
  solo5_heap_end = ROUND_PGDOWN(info->heap_start + info->heap_size);

  init_page_allocator(solo5_heap_start, solo5_heap_end);
}

void *
mmap(void *addr, size_t length, int prot, int nflags, int fd, off_t offset)
{
  unsigned int shift;
  uintptr_t align;
  void *mem;

  if (fd != -1 || offset != 0 || addr) {
    errno = EINVAL;
    return MAP_FAILED;
  }

  shift = (nflags & MAP_ALIGNMENT_MASK) >> MAP_ALIGNMENT_SHIFT;
  if (shift == 0) {
    align = PAGE_SIZE;
  } else {
    align = 1UL << shift;
  }

  if (align % PAGE_SIZE) {
    errno = EINVAL;
    return MAP_FAILED;
  }

  mem = (void *)alloc_pages(get_order(length));
  if (!mem) {
    errno = ENOMEM;
    return MAP_FAILED;
  }

  return mem;
}

int
munmap(void *addr, size_t length)
{
  free_pages(addr, get_order(length));

  return 0;
}
