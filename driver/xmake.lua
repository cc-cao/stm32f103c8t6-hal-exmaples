target("stm32f1xx_driver")
    set_kind("static")

    add_files(
        "system/*.c",
        "STM32F1xx_HAL_Driver/Src/*.c",
        "startup_stm32f103xb.s"
    )
