# Tutorial 1: Hello World

## Introduction
With Tutorial 0 completed, you should have a fully functional setup for programming an ESP32. For the rest of this tutorial series, an [Espressif ESP32 Dev Module](https://www.amazon.com/Espressif-ESP32-ESP32-DEVKITC-ESP-WROOM-32-soldered/dp/B01N0SB08Q) will be used. Other ESP32 kits can be used, but pin assignments and the method for programming the ESP32 may change and are thus not supported in this tutorial series.

This tutorial will demonstrate how to connect an ESP32 for programming. In addition, this tutorial will show you how to create an ESP32 project in PlatformIO. This project will print "Hello World" to the serial stream. Finally, you will learn how to view this serial stream on your computer.

## Hardware Setup
Using a USB to USB micro cable, connect the ESP32 to your computer. Ensure this cable is capable of both power and data transmission. Once connected, the ESP32's red power LED should turn on and be clearly visible. To verify that the ESP32 is properly connected, check that the ESP32 is correctly present in the connected devices list for your OS.

### Windows 10
In Windows 10 (other versions of Windows will be similar), open the Start Menu. In the search bar, type in `Device Manager`. This should return a control panel application of the same name. Run this application. This program will show a list of currently connected devices, organized into multiple categories. If your ESP32 is properly connected, it will be present under the `Ports (COM & LPT)` category with the name `Silicon Labs CP210x USB to UART Bridge (COM3)`. Note that the number in the `(COM_)` portion may vary. Take note of whatever number is present as that is the COM port the ESP32 was connected to by the OS.

### Linux (Ubuntu 16.04)
Open a terminal and run the command `lsusb`. In this list of usb devices, you should find a `Silicon Labs CP210x USB to UART Bridge` device present. Next, run `ls /dev/ttyUSB*`. This will display all USB serial devices. If the ESP32 is the only device present, it will likely be `ttyUSB0`. If there are multiple devices present, it may be a different device number. You will have to take steps to determine that on your own, depending on the connected hardware. Make sure to remember what device the ESP32 is.

## Software
With the hardware properly connected and ready, the program can now be written. To write a program for the ESP32 in PlatformIO, a project folder must be created. In addition to the main.cpp file which contains the program, a project folder contains other additional files like libraries and configuration files to help organize more complex programs and to tell PlatformIO how to compile and program the ESP32 with your program.

### Project Setup
To setup a PlatformIO project, open VSCode and open the PlatformIO Home page. Navigate to the home tab of the PlatformIO home page. In the Quick Access section, you will see an option `New Project`. Click that button. A prompt to create a new project will be opened. First, a name for the project must be given. In the name field, enter `Tutorial_1_Hello_World`. Next, PlatformIO must be told what device this project is using.  Click on this field to open a dropdown menu. In grayed out text, the platform will be displayed. Beneath this grayed out platform name, devices within that platform will be displayed. Navigate to the `Espressif 32` platform and select the `Espressif 32 Dev Module` (Hint: you can just type in `Espressif 32 Dev Module` instead of scrolling!). Next, the software framework must be selected. Click on the `Framework` field and select `Arduino`. Finally, the location where the project folder will be stored must be specified. You can use the default location (hover over the `?` to see where that is) or you can uncheck the box and specify a directory to use. You can store this project wherever you prefer, just don't forget where it is! Once you've configured everything, double check your entries and click the `Finish` button. The prompt will disappear and the project will be created. Once it is created, a new window will appear in VSCode showing the files in your newly created project. Congratulations! You've successfully made your first project!

### Writing the Program
In the window with your project's file hierarchy, click the `main.cpp` in the `src` folder. This will open the file in VSCode's text editor. You should see the following lines in the file already:
```c++
#include <Arduino.h>

void setup() {
    // put your setup code here, to run once:
}

void loop() {
    // put your main code here, to run repeatedly:
}
```

#### What's Already There
First, let's take a look at what is already in the file piece by piece. At the top of the file, we have:
```c++
#include <Arduino.h>
```
In C++, #include is a macro telling the compiler to include the contents of the file in the angle brackets. It's equivalent to copy-pasting the contents of the file, `Arduino.h` in this case, into the file right where the include is located. In this case, the `Arduino.h` file contains all of the commands and macros of the Arduino software framework. Including this allows us to use Arduino on the ESP32.

Next, we have this block:
```c++
void setup() {
    // put your setup code here, to run once:
}
```
This is a function in C++. `void` tells the compiler (and any snooping programmers!) what the function returns, nothing in this case. Next, `setup` is the name of the function. The empty parentheses, `()`, describe the input parameters for the function. In this case, `setup` doesn't take any inputs. The brackets `{` and `}` enclose the contents of the function. This is what is run when the function is called. In between these brackets is the line `// put your setup code here, to run once:`. This is a C++ comment. Comments are not actual code and are there as notes to the programmer. This comment gives some information about the purpose of the `setup` funtion. `setup` is a special function in the Arduino framework; it is automatically run _once_ automatically at the start of the program's execution. This is where you would put code to setup objects your program will need to successfully run.

The next block is almost identical to the `setup` function code block:
```c++
void loop() {
    // put your main code here, to run repeatedly:
}
```
Similarly, this defines a function, `loop`, with no return value and no input parameters. Looking at the comment, this function is similarly a special function in the Arduino framework. After `setup` runs once, `loop` is continuously run. Every time `loop` finishes running and returns, the Arduino framework automatically calls it again. This is where the main program goes. Things like polling input pins and writing text to the serial line all happen in here.

#### Writing the Setup Function
Let's begin by writing the setup function. Since we want to print text onto the serial port, we'll need to setup a way to access the serial port from the ESP32. In the Arduino framework, this is done via the Serial object. To setup the port, you must call the function `Serial.begin` while passing in an integer representing the baud rate (how fast the serial port communicates). The ESP32 can support very fast speeds, so let's use a baud rate of 921600. Add the following line to your setup function:
```c++
    Serial.begin(921600);
```
Remember to include the semicolon at the end of the line! Your program file should now look like this:
```c++
#include <Arduino.h>

void setup() {
    // put your setup code here, to run once:
    Serial.begin(921600);
}

void loop() {
    // put your main code here, to run repeatedly:
}
```
Congratulations! You're done with the setup function! Now, onto the loop!

#### Writing the Loop Function
With the setup function complete, the Serial port is setup and ready to use. The first thing to add to the loop function is the line to actually print `Hello World` onto the serial port. Add this line to the loop function:
```c++
    Serial.println("Hello World");
```
Again, remember the semicolon! This command will print `Hello World` to the serial port. It will then automatically enter a newline so that the next time you write to the serial port will write the text onto a new line in the console.

Your code should now look like this:
```c++
#include <Arduino.h>

void setup() {
    // put your setup code here, to run once:
    Serial.begin(921600);
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println("Hello World");
}
```
We're not done yet, though! Although this program could now successfully compile and would print `Hello World` just like we wanted it to, there is one catch. Remember how `loop` gets called continuously every time it finishes executing? Well, our current loop function finishes quickly (about 140 microseconds!). As a result, it will print `Hello World` over 5000 times per second! That's absolutely impossible to read, so let's slow it down to a more human readable speed. Add the following line to `loop`:
```c++
    delay(250);
```
`delay` is a special Arduino function. It will pause the program for the number of milliseconds passed as its input parameter. In this case, it will pause for 250 milliseconds, or 0.25 seconds. As a result, our program now takes about 250 milliseconds to finish. This will slow down `loop`, causing `Hello World` to be printed at readable rate of 4 times per second. As a result, you code should now look like this:
```c++
#include <Arduino.h>

void setup() {
    // put your setup code here, to run once:
    Serial.begin(921600);
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println("Hello World");
    delay(250);
}
```
And there you have it! Your first program for the ESP32 is done and ready to go! Next, we'll compile it!

### Compiling
Now that your program is finished, let's compile it! In the PlatformIO sidebar in VSCode, look for the checkmark icon. Hovering over it, you should see words `PlatformIO: Build`. This button will try to compile your code. Press it! A terminal will open in VSCode, showing the output of the compiler as it runs. If compilation is successful, the title bar of this terminal will turn green before closing. If compilation fails, it will turn red and print some error text telling you why it failed. Since the terminal closes on successful compilation, it can be hard to see the terminal's contents, especially when compiling small programs. Anyway, your program should successfully compile. If it doesn't, check your code and ensure there aren't any typos! With your code successfully compiling, it is now time to upload it to the ESP32!

### Uploading
In the same sidebar you found the `PlatformIO: Build` command, look for the rightward arrow icon. This is usually right below the build button. Hovering over it will display `PlatformIO: Upload`. Pressing this will instruct PlatformIO to compile your program and automatically upload it to your ESP32. Usually, PlatformIO will automatically identify the ESP32 and use the appropriate COM (if Windows) or ttyUSB (Linux) port. If you have multiple serial devices connected, it may need some help choosing the right one. Similar to the build command, a terminal showing the output of the upload will appear. It will flash a green title bar and close if the upload is successful or will flash red and stay open if an error occurrs. After attempting to upload your code, the upload should successfully complete. If it doesn't, try re-plugging your ESP32 into your computer and double check your hardware connections.

## Checking the Serial Monitor
At this point, your program is successfully written, compiled, and uploaded to the ESP32. It's actually running right now! "But where is `Hello World`!?" Well, now it's time to open up the Serial Monitor so you can see those fabled hellos. In the same sidebar we found the build and upload commands, look for the icon with a two-pronged connector. It will say `Serial Monitor` when you hover over it. Click it to open a prompt to setup the Serial Monitor. Two fields need to be populated. First, the Port needs to be specified. PlatformIO will try to auto-select the appropriate port. Verify this is correct by comparing it to the COM (Windows) or ttyUSB (Linux) port you verified the ESP32 was connected to. Next, the Baudrate needs to be set. This should be the same Baudrate you configured the ESP32's Serial device to use, 921600 for this example. Set these two fields correctly and click `Start`. A terminal will open and connect to the ESP32.

On connection, the Serial Monitor will restart the ESP32, causing the program to restart. On startup the ESP32 prints some debug information at a buadrate of 78600, which will look like a bunch of garbage on the serial monitor. After that, it should start printing out `Hello World` once every 250 milliseconds!

## Conclusion
There it is! You successfully hooked up an ESP32 to your computer. You then setup a new project for an ESP32, wrote a program, compiled it, and uploaded it to an actual ESP32. You then pulled up the Serial Monitor to look at the output produced by the ESP32. Well done! In the next tutorial, we'll play with a more advanced function of the ESP32: multi-core processing. That's right! The ESP32 doesn't have one core, it has two! And we're gonna use them both, at the same time!