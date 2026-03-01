function(stm32_add_example target_name)
    cmake_parse_arguments(EXAMPLE "USE_PRINT" "" "" ${ARGN})

    file(GLOB _example_sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    )

    add_executable(${target_name} ${_example_sources} ${STM32_SYSCALL_SOURCES})

    # Keep static library resolution stable for cross-library symbol references.
    target_link_libraries(
        ${target_name}
        PRIVATE
            stm32_common
            stm32f1xx_driver
            stm32_common
            stm32f1xx_driver
            stm32_build_options
    )

    if(EXAMPLE_USE_PRINT)
        target_compile_definitions(${target_name} PRIVATE USE_PRINT)
    endif()

    set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")

    add_custom_command(
        TARGET ${target_name}
        POST_BUILD
        COMMAND ${CMAKE_SIZE} -Ax $<TARGET_FILE:${target_name}>
        COMMAND ${CMAKE_SIZE} -Bd $<TARGET_FILE:${target_name}>
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target_name}> $<TARGET_FILE:${target_name}>.bin
        VERBATIM
    )
endfunction()
