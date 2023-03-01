<h1 align="center"> OCT - On-Car Telemetry </h2>

# Table of Contents

- [About](#about)
- [Development Environment](#development-environment)
- [Useful Resources](#useful-resources)
- [Related Projects](#related-projects)

# About

The On-Car-Telemetry is reponsible for unpacking CAN data from CAN-S and CAN-C into PDU
(Protocol Data Unit) telemetry frames and sending it via UART to the Xbee transmitter.
Xbee provides the bidirectional transmission between Intermediate server and On-Car Telemetry Unit.
The microcontroller used in this project is the STM32 F446RE.

More detailed information about this project is available to members on the
SUFST Docs Site.

# Setup / Development Environment

## Submodules

This project depends on middlewares in the `src/Middlewares/` folder, some of
which are Git submodules. When first cloning this repo, run the following
commands:

```sh
git submodule init
git submodule update
```

For more information on submodules, see the [Git submodules documentation](https://git-scm.com/book/en/v2/Git-Tools-Submodules).

## Building and Flashing

To build this project and flash code to the microcontroller, you will need the
following on your path:

- [Make](https://www.gnu.org/software/make/)
- [ARM GNU Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)
- [STLink Open Source Toolset](https://github.com/stlink-org/stlink)
- `mkdir`, `rm`, `tput` and `echo`

Build with:

```sh
cd src
make
```

Flash with:

```sh
cd src
make flash
```

For detailed toolchain setup instructions, see the SUFST Docs Site.

> Note: Windows users should run these commands from Git Bash.

## VS Code

This project is set up to be edited and debugged in [VS Code](https://code.visualstudio.com).
The `.vscode`folder includes tasks and launch configurations.

Since the toolchain is set up to be fully command line based, it is also
possible to use other code editors.

> Note: Windows users should set Git Bash as the shell in VS Code.

## STM32CubeMX

[STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) is used
to generate boilerplate initialisation code for the microcontroller. These
configurations are stored in `src/OCT_F4.ioc` which should not be edited manually.
To minimise the chance of merge conflicts, changes to the `.ioc` should be made
as infrequently as possible as the `.ioc` format is not well suited to version
control.

# Useful Resources

Microcontroller:

- [STM32F446RE MCU Datasheet](https://www.st.com/resource/en/datasheet/stm32f446re.pdf)
- [STM32F4xx HAL Manual](https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf)

ThreadX RTOS:

- [ThreadX Overview](https://docs.microsoft.com/en-us/azure/rtos/threadx/overview-threadx)
- [ThreadX API](https://docs.microsoft.com/en-us/azure/rtos/threadx/chapter4)

Xbee:

- [Xbee Pro S2C Zigbee](https://www.digi.com/resources/documentation/digidocs/pdfs/90002002.pdf)

# Related Projects

- [Wireless Telemetry GUI](https://github.com/sufst/wireless-telemetry-gui)
- [Intermediate Server](https://github.com/sufst/intermediate-server)
- [Can-Defs](https://github.com/sufst/can-defs)
- [RTCAN](https://github.com/sufst/rtcan)