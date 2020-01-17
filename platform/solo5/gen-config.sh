#!/bin/bash

SRC=$1
DST=$2
VAR_NAME=lkl_json_config

JSON=""
for LINE in $(cat ${SRC}); do
  JSON="${JSON}$(printf ${LINE} \
    | sed -e 's#\"#\\\"#g' \
    )"
done

[ -e ${DST} ] && rm -f ${DST}

echo "const char *${VAR_NAME} = \"${JSON}\";" >> ${DST}
