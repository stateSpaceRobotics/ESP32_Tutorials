# Tutorial 0: Setup

## Introduction
This tutorial is the first in a series meant to teach some of the fundamentals of using the Arduino framework on the ESP32 microprocessor. These tutorials attempt to cover all of the elements needed for using the ESP32 as a wireless interface for precision control of DC motors using PWM control and Quadrature Encoder feedback. The basics of PID and tuning PID controllers are also explored. As an end result, the reader should know enough to implement basic motor control and wireless control of the ESP32.

## VSCode
First, the text editor must be installed. To work with the Arduino framework, VSCode will be used as a base for use with PlatformIO. Navigate to [code.visualstudio.com](https://code.visualstudio.com/). A download link will be displayed to download the appropriate VSCode editor for your OS. Download VSCode from this website and install it according to your OS (run the exe for Windows, etc.).

## PlatformIO
With VSCode installed, next PlatformIO must be installed on top of VSCode. PlatformIO extends VSCode into a full IDE for the embedded development targeted in this tutorial series. Navigate to [PlatformIO.org's page for installing on VSCode](https://platformio.org/install/ide?install=vscode). Follow the instructions on this page to install PlatformIO for VSCode.

## ESP32 Platform
With PlatformIO installed on top of VSCode, the final step is to install the toolchain for the ESP32 within PlatformIO. Open up the PlatformIO home page in VSCode. Using the sidebar, navigate to the "Platforms" page. In the "Embedded" tab, search for `Espressif 32`. Click on this entry. On the subsequent page, click the install button. This will install the ESP32's software toolchain.

## Conclusion
Congratulations! You now have a setup capable of programming an ESP32 microcontroller! The next tutorial will walk through writing a simple program and the steps to compile and upload the code to the ESP32.