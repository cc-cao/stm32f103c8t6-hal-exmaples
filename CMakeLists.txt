cmake_minimum_required(VERSION 3.22)

project(stm32f103_demo VERSION 1.0.1 LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(_stm32_cpu_flags -mcpu=cortex-m3 -mthumb)

add_library(stm32_build_options INTERFACE)
target_compile_definitions(stm32_build_options INTERFACE USE_HAL_DRIVER STM32F103xB USE_FULL_ASSERT)
target_include_directories(
    stm32_build_options
    INTERFACE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/generic
        ${CMAKE_SOURCE_DIR}/driver/STM32F1xx_HAL_Driver/Inc
        ${CMAKE_SOURCE_DIR}/driver/STM32F1xx_HAL_Driver/Inc/Legacy
        ${CMAKE_SOURCE_DIR}/driver/CMSIS/Device/ST/STM32F1xx/Include
        ${CMAKE_SOURCE_DIR}/driver/CMSIS/Include
)
target_compile_options(
    stm32_build_options
    INTERFACE
        $<$<COMPILE_LANGUAGE:C,CXX>:${_stm32_cpu_flags}>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wall -fdata-sections -ffunction-sections -Wno-missing-braces>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti -Wno-volatile>
        $<$<COMPILE_LANGUAGE:ASM>:${_stm32_cpu_flags}>
        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -Wall -fdata-sections -ffunction-sections>
)
target_link_options(
    stm32_build_options
    INTERFACE
        ${_stm32_cpu_flags}
        -Wl,--gc-sections
        -T${CMAKE_SOURCE_DIR}/driver/STM32F103XX_FLASH_OLD.ld
)
target_link_libraries(stm32_build_options INTERFACE c m)

file(GLOB _common_sources CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/generic/*.c
    ${CMAKE_SOURCE_DIR}/bsp/*.c
    ${CMAKE_SOURCE_DIR}/bsp/oled/*.c
)
add_library(stm32_common STATIC ${_common_sources})
target_link_libraries(stm32_common PUBLIC stm32_build_options)

file(GLOB _driver_sources CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/driver/STM32F1xx_HAL_Driver/Src/*.c
)
add_library(stm32f1xx_driver STATIC
    ${_driver_sources}
    ${CMAKE_SOURCE_DIR}/driver/system/system_stm32f1xx.c
    ${CMAKE_SOURCE_DIR}/driver/startup_stm32f103xb.s
)
target_link_libraries(stm32f1xx_driver PUBLIC stm32_build_options)

set(STM32_SYSCALL_SOURCES
    ${CMAKE_SOURCE_DIR}/driver/system/syscalls.c
    ${CMAKE_SOURCE_DIR}/driver/system/sysmem.c
)

include(${CMAKE_SOURCE_DIR}/cmake/STM32Example.cmake)

file(GLOB _example_dirs LIST_DIRECTORIES true CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/examples/*)
foreach(_example_dir IN LISTS _example_dirs)
    if(IS_DIRECTORY "${_example_dir}" AND EXISTS "${_example_dir}/CMakeLists.txt")
        add_subdirectory("${_example_dir}")
    endif()
endforeach()
