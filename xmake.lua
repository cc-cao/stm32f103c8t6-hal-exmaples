set_xmakever("3.0.0")
set_project("stm32f103_demo")
set_version("1.0.1")
add_requires("gnu-rm")
add_rules("mode.release", "mode.debug")
set_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

set_toolchains("@gnu-rm")
set_defaultplat("cross")
set_defaultarchs("cortex-m3")
set_languages("c17", "c++20")
set_targetdir("build/target")

add_defines("USE_HAL_DRIVER", "STM32F103xB", "USE_FULL_ASSERT")

add_cxxflags(
    "-fno-exceptions",
    "-fno-rtti",
    "-Wno-volatile"
)

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
    "-lc -lm",
    "-Wl,--gc-sections",
    -- "-specs=nano.specs",  -- 用10.3的版本必须要去掉, 用更老的版本必须加上, 用更新的版本可以加可以不加
    "-T$(projectdir)/driver/STM32F103XX_FLASH_OLD.ld",  -- 如果用更新的版本则可以使用不带OLD的ld文件
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
    "generic/*.c",
    "bsp/**.c"
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

includes("driver")

-- 子模块 具体的 程序
includes("examples/**")
