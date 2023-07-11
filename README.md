# Mission-E CAN DATA

## Arduino IDE Settings

Install Arduino IDE with STM32duino.

### STM32 settings
THe following settings need to be set in the Arduino IDE - in the Tools menu:

- Board: Nucleo 144
- Optimize: Debug (or something else, depending on the use case)
- Board part number: Nucleo H743ZI2
- C RUntime Library: Newlib Nano (default)
- Upload method: SWD
- Usb Support: None
- U(S)ART Support: Enabled (generic 'Serial')
- USB Speed: High Speed/Low Speed (depends on the use case)