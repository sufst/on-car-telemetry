# STM32 - XBEE Demo Sample

Demo sample requires two UART interfaces, both 115200 rate/8 Data/1 Stop/None parity/None Flow Control:
- Xbee - STM32 Interface
- STM32 - Host Interface (Terminal)

Example of terminal output without Xbee connected (no device detected)
![image](https://user-images.githubusercontent.com/47193610/217953437-c3d73373-4ba6-4660-bb01-61ff0e837ee8.png)

Type `menu` to show command list. Here - its unavailable.

![image](https://user-images.githubusercontent.com/47193610/217953477-0a7ee241-56a2-402e-92fc-66665282323e.png)

## Build

Run `make` in `src` directory.

## Debug

Use Stlink.
The most convienient - use Cortex-Debug Extension with pre-defined configuration which comes within this repo.
