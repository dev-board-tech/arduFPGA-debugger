# arduFPGA-debugger

# In development...

 This is the debug interface PC application to debug arduFPGA microcontroller designs.

This application is developed to work in tandem with <a href="https://github.com/dev-board-tech/arduFPGA-mega-first-stage-boot-loader">arduFPGA first stage boot-loader</a> that include a simple debugger using UART interface.

Depending of the design, the UART interface can be used only by the user application or by the bootloader.

This application help developers to read/write FLASH, RAM & EEPROM memory content and monitor and modify small sections of the memory.

This way being able to debut issues in the applications in development.

The application is developed in QT for easily porting to other platforms.

At this date it supports only reading of FLASH, RAM and EEPROM memory, the default communication speed is 115200b 8b none.

### Reading memory description:

There are four modes of reading the memory using four buttons on each memory window:

* "Refresh window" button = will read memory content that can be shown in the window and load it to internal buffer and display it to the screen, all boxes will have the values displayed in black colour, this function will not compare the read content with the internal buffer content.
* "Compare window" button = will read memory content that can be shown in the window and compare it with the content in the internal buffer, if a byte value is different than the internal buffer that value is displayed in red, this function will found the difference from the last load buffer and current reading of memory from arduFPGA.
* "Refresh compare window" button = each time this button is press the debugger will read the memory shown in the window from arduFPGA and compare it with the values from internal buffer, the buffer will be actualized with the new rad data, this way if a value is different from the last press of the same button will be displayed in red.
* "Read memory" button = will read the entire memory in the internal buffer and displayed, this function will freeze the debugger during reading of the memory, this button can be used for data clone.

There is "Realtime load" check box, if is checked when the displayed addresses are changed with scroll button, mouse scroll or window resizing the debugger will automatically read the memory content from arduFPGA with the last pushed button settings, the "Read memory" button will not change the settings.
