target("w25q80")
    add_defines("USE_PRINT")

    add_files(
        "./*.c", 
        "./*.cpp"
    )

    after_build(function(target)
        local elf = target:targetfile()
        local sdk = target:toolchains()[1]:bindir()
        local objcopy = sdk .. "/arm-none-eabi-objcopy"
        local size = sdk .. "/arm-none-eabi-size"
        os.exec("%s -Ax %s", size, elf)
        os.exec("%s -Bd %s", size, elf)
        os.exec("%s %s -O binary %s", objcopy, elf, elf .. ".bin")
        os.exec("rm %s", elf)
    end)
