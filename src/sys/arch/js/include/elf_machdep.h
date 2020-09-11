#ifndef _JS_ELF_MACHDEP_H_
#define _JS_ELF_MACHDEP_H_

#define ELF32_MACHDEP_ID    EM_JS
#define ELF64_MACHDEP_ID    EM_JS

#define ELF32_MACHDEP_ENDIANNESS  ELFDATA2LSB
#define ELF64_MACHDEP_ENDIANNESS  ELFDATA2LSB

#define ELF32_MACHDEP_ID_CASES  \
        case EM_JS:             \
            break;

#define ELF64_MACHDEP_ID_CASES  \
        case EM_JS:             \
            break;

#define ARCH_ELFSIZE        32

#endif /* _JS_ELF_MACHDEP_H_ */
