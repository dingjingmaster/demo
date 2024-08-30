function(check_system_header_exists headerFile absFilePath)
    if (EXISTS "/usr/include/${headerFile}")
        set(${absFilePath} TRUE PARENT_SCOPE)
    elseif (EXISTS "/usr/local/include/${headerFile}")
        set(${absFilePath} TRUE PARENT_SCOPE)
    else ()
        set(${absFilePath} FALSE PARENT_SCOPE)
    endif ()
endfunction()


# config 配置开始
function(start_config_file filePath content)
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/save-config-file.py "START" "${filePath}" "${content}")
endfunction()


# config 配置结束
function(stop_config_file filePath content)
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/save-config-file.py "STOP" "${filePath}" "${content}")
endfunction()


# 文件中追加内容
function(append_config_file filePath content)
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/save-config-file.py "CONTENT" "${filePath}" "${content}")
endfunction()


# 字符串分割，取最后一个子串
function(string_split_and_take_last str res)
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/get-file-name.py ${str} OUTPUT_VARIABLE out)
    set(${res} ${out} PARENT_SCOPE)
endfunction()