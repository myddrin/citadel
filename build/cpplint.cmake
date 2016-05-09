
find_package(PythonInterp QUIET)

set (CPPLINT_SCRIPT)

function(get_cpplint VAR)
    if (EXISTS "${CPPLINT_SCRIPT}")
        set(${VAR} ${CPPLINT_SCRIPT} PARENT_SCOPE)
    else()
        # search for in PATH
        set(cpplint)
        message(FATAL_ERROR "searching not supported yet")
        set(${VAR} ${cpplint} PARENT_SCOPE)
        set(CPPLINT_SCRIPT ${cpplint})  # so we don't search again
    endif()
endfunction()

function(check_cpplint TARGET_NAME CFG_FILE)

    if (NOT ${PYTHONINTERP_FOUND})
        message ("Warning: CPPLINT step ignored as python is not found")
        return()
    endif()
    set(cpplint)
    get_cpplint(cpplint)
    if (NOT EXISTS ${cpplint})
        message ("Warning: CPPLINT step ignored as cpplint.py is not found")
        return()
    endif()

    get_target_property(sources ${TARGET_NAME} SOURCES)
    get_filename_component(directory ${CFG_FILE} DIRECTORY)

    set(cmd)
    
    if (MSVC)
        set(cmd ${cmd} "--output=vs7")
    endif()
    
    # todo(tsr): grep cfg for extensions
    set(cmd "--extensions=hpp,h,cpp")

    add_custom_target(${TARGET_NAME}_cpplint
        COMMAND
        "${PYTHON_EXECUTABLE}"
        "${CPPLINT_SCRIPT}"
        "${cmd}"
        ${sources}
        DEPENDS ${sources} ${CFG_FILE}
        WORKING_DIRECTORY ${directory}
        COMMENT "Checking style of ${TARGET_NAME}"
        VERBATIM
    )
endfunction()