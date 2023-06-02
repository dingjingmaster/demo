#!/bin/bash

OEM_INI_FILE="oem.ini"


OEM_INI_FILE_BAK="${OEM_INI_FILE}.bak"

# 没有则创建 oem.ini 文件
[[ ! -f "${OEM_INI_FILE}" ]] && touch "${OEM_INI_FILE}"

# 检测是否有 [Support]， 没有则添加 指定配置
[[ -f "${OEM_INI_FILE}" ]] && [[ $(cat "${OEM_INI_FILE}" | grep -i '[Support]' | wc -l) -eq 0 ]] && echo -e "[Support]\ndevelopuseribbon=true" > "${OEM_INI_FILE}"

# 有 [Support], 检测是否存在 developuseribbon
[[ -f "${OEM_INI_FILE}" ]] && [[ $(cat "${OEM_INI_FILE}" | grep -ix '\[Support\]' | wc -l) -eq 1 ]] && [[ $(cat "${OEM_INI_FILE}" | grep -i 'developuseribbon=' | wc -l) -eq 0 ]] && cp "${OEM_INI_FILE}" "${OEM_INI_FILE_BAK}" && sed 's/\[Support\]/\[Support\]\ndevelopuseribbon=true/' "${OEM_INI_FILE_BAK}" > ${OEM_INI_FILE}

rm -f "${OEM_INI_FILE_BAK}"
