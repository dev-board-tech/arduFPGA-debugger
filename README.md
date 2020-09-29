# arduFPGA-debugger

# In development...

 This is the debug interface PC application to debug arduFPGA microcontroller designs.

This application is developed to work in tandem with <a href="https://github.com/dev-board-tech/arduFPGA-mega-first-stage-boot-loader">arduFPGA first stage boot-loader</a> that include a simple debugger using UART interface.

Depending of the design, the UART interface can be used only by the user application or by the bootloader.

This application help developers to read/write FLASH, RAM & EEPROM memory content and monitor and modify small sections of the memory.

This way being able to debut issues in the applications in development.

The application is developed in QT for easily porting to other platforms.

At this date it supports only reading of FLASH, RAM and EEPROM memory, the default communication speed is 115200b 8b none.
