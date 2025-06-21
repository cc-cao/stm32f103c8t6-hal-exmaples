set_xmakever("2.9.9")
set_project("stm32f103_demo")
add_rules("mode.release", "mode.debug")

toolchain("gcc-arm-none-eabi")
    set_kind("standalone")
    set_sdkdir(os.scriptdir() .. "/../tools/arm-gnu-toolchain-14.2.rel1-darwin-arm64-arm-none-eabi")
toolchain_end()

set_toolchains("gcc-arm-none-eabi")
set_defaultmode("debug")
set_defaultplat("cross")
set_defaultarchs("cortex-m3")

set_targetdir("build/target")

set_languages("c17", "c++20")

add_defines("USE_HAL_DRIVER", "STM32F103xB", "USE_FULL_ASSERT")

add_cxxflags(
    "-fno-exceptions",
    "-fno-rtti"
)

-- if is_mode("debug") then
--     add_cxflags("-g -gdwarf-2")
-- else 
--     add_cxflags("-Os")
-- end

add_cxflags("-Og -g")
add_ldflags("-g")

add_cxflags(
    "-mcpu=cortex-m3 -mthumb",
    "-Wall -fdata-sections -ffunction-sections",
    "-Wno-missing-braces",
    {force = true}
)

add_asflags(
    "-mcpu=cortex-m3 -mthumb",
    "-x assembler-with-cpp",
    "-Wall -fdata-sections -ffunction-sections",
    {force = true}
)

add_ldflags(
    "-mcpu=cortex-m3 -mthumb",
    "-specs=nano.specs",
    "-T./driver/STM32F103XX_FLASH.ld",
    "-lc -lm -lnosys",
    "-Wl,--gc-sections",
    {force = true}
)

add_includedirs(
    "driver/STM32F1xx_HAL_Driver/Inc",
    "driver/STM32F1xx_HAL_Driver/Inc/Legacy",
    "driver/CMSIS/Device/ST/STM32F1xx/Include",
    "driver/CMSIS/Include",
    "generic",
    "."
)

add_files(
    "driver/STM32F1xx_HAL_Driver/Src/*.c",
    "driver/system/*.c",
    "generic/*.c",
    "bsp/**.c",
    "driver/startup_stm32f103xb.s"
)

task("flash")
    set_menu {
        usage = "xmake flash [options]",
        description = "openocd flash write",
        options = {
            {'t', "target", "kv", nil, "The name of the target to flash, e.g., demo or hello"}
        }
    }

    on_run(function ()
        import("core.base.option")
        local target = option.get("target")
        if not target then
            raise("You must provide a target name with -t, e.g., xmake flash -t demo")
        end
        os.exec("probe-rs download --chip STM32F103C8 build/target/%s", target)
        os.exec("probe-rs reset --chip STM32F103C8")
    end)
task_end()

-- 子模块 具体的 程序
includes("examples")
