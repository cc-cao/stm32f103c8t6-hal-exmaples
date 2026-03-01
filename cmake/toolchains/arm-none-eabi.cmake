set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_TRY_COMPILE_PLATFORM_VARIABLES ARM_GCC_BIN_DIR)

if(NOT DEFINED ARM_GCC_BIN_DIR)
    message(FATAL_ERROR "ARM_GCC_BIN_DIR is not set. Please set it in CMakeUserPresets.json.")
endif()

set(_arm_gcc "${ARM_GCC_BIN_DIR}/arm-none-eabi-gcc")
set(_arm_gxx "${ARM_GCC_BIN_DIR}/arm-none-eabi-g++")
set(_arm_objcopy "${ARM_GCC_BIN_DIR}/arm-none-eabi-objcopy")
set(_arm_size "${ARM_GCC_BIN_DIR}/arm-none-eabi-size")

foreach(_tool IN ITEMS _arm_gcc _arm_gxx _arm_objcopy _arm_size)
    if(NOT EXISTS "${${_tool}}")
        message(FATAL_ERROR "Tool not found: ${${_tool}}")
    endif()
endforeach()

set(CMAKE_C_COMPILER "${_arm_gcc}" CACHE FILEPATH "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "${_arm_gxx}" CACHE FILEPATH "CXX compiler" FORCE)
set(CMAKE_ASM_COMPILER "${_arm_gcc}" CACHE FILEPATH "ASM compiler" FORCE)

set(CMAKE_OBJCOPY "${_arm_objcopy}" CACHE FILEPATH "objcopy tool path" FORCE)
set(CMAKE_SIZE "${_arm_size}" CACHE FILEPATH "size tool path" FORCE)
