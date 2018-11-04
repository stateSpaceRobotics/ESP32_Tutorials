# Tutorial 2: Multi-Core Programs
## Introduction
In this tutorial, we'll re-write our Hello World program with a twist: we'll use both of the ESP32's cores! The ESP32 is a special microcontroller in that it has two cores that can simultaneously run code. This is great for WiFi-enabled devices like the ESP32 as it allows the slower WiFi code to run on one core while more timing sensitive code (interrupts, PWM, etc.) can run on the second core unaffected by the intermittent delays caused by the WiFi subroutines. In this tutorial, we'll explore the basics of executing code on both cores simultaneously. Additionally, we'll cover the basic use of semaphores, special objects that allow programs to safely exchange data between cores.

## Hardware Setup
No special hardware is needed for this tutorial. All we need is our trusty ESP32 and a micro USB cable to connect to your computer. Make sure these are all connected just like in Tutorial 1.

## Software
### Project Setup
Now, we can setup a new project for the tutorial. Follow the steps you learned from Tutorial 1 to create a project named `Tutorial_2_Multi-Core`. Make sure to open `main.cpp` in Atom if you haven't already!

### FreeRTOS and CoreID's
For the ESP32, the Arduino framework is based off of a lower-level software framework call FreeRTOS (Free Real-Time Operating System). As a result, when we include the Arduino libraries, we also get access to FreeRTOS. As part of FreeRTOS, we have access to a variety of functions and objects designed to make multi-core programming easy. The first, and most basic, function we will use is `xPortGetCoreID()`. This function returns a number (0 or 1 for the ESP32) corresponding to the core that is running the call to `xPortGetCoreID`.

### Writing the Program
The objective of our program is to print `Hello World: ` followed by the ID number of the core that made the write to the serial port. We'll also configure each core to run this command repeatedly. To save time, we'll start from the program we wrote in Tutorial 1:
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
You can copy this code and paste it over the contents of the `main.cpp` in your Tutorial 2 project.

#### Writing the Loop: Printing Core ID
We'll start by modifying the loop function. By default, Arduino automatically runs the `loop` function on Core 1. Core 0 is reserved for running the WiFi libraries. Thus, we can use the `loop` function as the base for our printing task on Core 1. This function is already setup to print `Hello World` every 250 milliseconds. All we need to do is change line containing the print command to the following:
```c++
    Serial.print("Hello World: "); Serial.println(xPortGetCoreID());
```
We made a couple of changes here. First, notice we changed the first print command from `Serial.println` to `Serial.print` and modified the text to write to the serial port. `Serial.print` omits the newline character automatically added by `Serial.println`. This allows our second change, the extra `Serial.println` call, to print its output on the same line before starting a new line. In our second call, notice that instead of a string, we are using a function call, `xPortGetCoreID()`. Remember, this call returns the ID number of the core that made the function call. This will return that ID as an integer, which `Serial.println` will convert to a string and write to the serial port. At this point, our code looks like this:
```c++
#include <Arduino.h>

void setup() {
    // put your setup code here, to run once:
    Serial.begin(921600);
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.print("Hello World: "); Serial.println(xPortGetCoreID());
    delay(250);
}

```
This program will successfully compile, upload, and run. Feel free to try it! When you look at the serial port, `Hello World: 1` will be displayed on a new line every 250 milliseconds. At this point, Core 1's task is setup and ready to go! All that remains is to write an identical task for Core 0 to use.

#### FreeRTOS Task Function
To start a task on Core 0, we need to give it a function to run. Since we are doing the same task as what Core 1 is doing, you might think we can just re-use `loop`. After all, it's a function that's doing what we want right? But wait! Remember how `loop` is a special function that gets called repeatedly by Arduino? Well, when we tell Core 0 to run a function, it will run the function only once. So, if we give it `loop`, it will run exactly once and then promptly exit. But that's not what we want! We want it to print `Hello World: 0` every 250 milliseconds! So, we need to write a function similar to `loop` but that runs forever, writing to the serial port every 250 milliseconds. We can do this quite easily by adding the following function above `setup`:
```c++
void loop0(void * parameter) {
    while(1) {
        Serial.print("Hello World: "); Serial.println(xPortGetCoreID());
        delay(250);
    }
}
```
Woah now, what's going on here, you ask? Well, we can recognize the serial printing commands and the delay call. Those are the same as in `loop`, so this must be doing something similar. But what about `while(1)`? `while(...) {...}` is a special command that repeatedly runs the contents of the curly braces `{...}` until the statement in the parentheses `(...)` evaluates to zero. Since the statement inside the parentheses is just a constant one, the `while` loop we created will run forever. As such, it will continuously run the contents, the serial writing and delay, as fast as it can, forever. This is exactly what we want! Finally, this function has an input parameter. It's a void pointer, which means it could to any kind of data (int, float, char, and more!). We don't use it here, but it's needed for when we tell FreeRTOS to run the function on Core 0. Now, all that's left is to tell Core 0 to run this function and we'll have a multi-threaded program!

#### FreeRTOS Task Setup
Now, we need to tell the ESP32 to run our `loop0` function on Core 0. Since `loop0` runs infinitely, we only need to tell the ESP32 to run it once. Therefore, we need a place in the code that is already running that is only executed once... Sound familiar? Yep, the `setup` function is the perfect place where we can tell the ESP32 to run `loop0`! To do so, we'll use another FreeRTOS function, `xTaskCreatePinnedToCore`. This ones a little more complicated, so pay attention! Start by adding the following lines to the end of `setup`:
```c++
    xTaskCreatePinnedToCore(
                            loop0,   /* Function to implement the task */
                            "loop0", /* Name of the task */
                            10000,      /* Stack size in words */
                            NULL,       /* Task input parameter */
                            0,          /* Priority of the task */
                            NULL,       /* Task handle. */
                            0  /* Core where the task should run */
    );
```
Now, let's break it down. This is _technically_ only one line of code. After all, it's just a single function call. However, we can split it up like this to make things a little easier to read. First, we are just calling `xTaskCreatePinnedToCore`. The catch is we have a _lot_ of parameters for it! The first parameter is just the function we want the new task to run, `loop0` here. Next, we need to give a name to the task so FreeRTOS can keep track of it. For simplicity, we just use "loop0" as the name too. Next, we specify the stack size. This is how many words (32 bits or 4 bytes for the ESP32) of memory are allocated to the task. 10000 is a little overkill for such a simple task but is small enough to not be a big drain on the ESP32, so we'll leave it like that. Next, we give the input parameter for the function. FreeRTOS uses this to pass information to the function. Remember that void pointer? This parameter is what gets passed to that pointer! We don't have any data to give it, so we'll just give it NULL instead. Next, we give it a priority. FreeRTOS uses this to figure out which tasks get preference when multiple tasks run on the same core. We'll leave it at priority 0, max priority, for now. Next, we give a task handle for FreeRTOS to use. This is a pointer that FreeRTOS will link to the task so we can later access and modify the task (stop it, change priority, etc.). We don't need any of that, so we'll leave this as NULL too. Finally, we specify which core to use. Since we want this to run on Core 0, we'll give it 0. Your code should now look like this:
```c++
#include <Arduino.h>

int currentCore;

void loop0(void * parameter) {
    while(1) {
        Serial.print("Hello World: "); Serial.println(xPortGetCoreID());
        delay(250);
    }
}

void setup() {
    mutex = xSemaphoreCreateMutex();
    // put your setup code here, to run once:
    Serial.begin(921600);
    xTaskCreatePinnedToCore(
                            loop0,   /* Function to implement the task */
                            "loop0", /* Name of the task */
                            10000,      /* Stack size in words */
                            NULL,       /* Task input parameter */
                            0,          /* Priority of the task */
                            NULL,       /* Task handle. */
                            0  /* Core where the task should run */
    );
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.print("Hello World: "); Serial.println(xPortGetCoreID());
    delay(250);
}

```
There you go! This is a working multi-threaded program. When uploaded, this program will print from tasks running on both cores. However, this example is a little simplistic. What if we want to use data and variables on both tasks? How do we prevent the tasks from accessing the same variable simultaneously? Well, we'll need to add semaphores!

#### Race Conditions
Let's modify our code to make things a little interesting. Let's rewrite our program to look like this:
```c++
#include <Arduino.h>

int currentCore;

void loop0(void * parameter) {
    while(1) {
        currentCore = xPortGetCoreID();
        Serial.print("Hello World: "); Serial.println(currentCore);
        delay(250);
    }
}

void setup() {
    mutex = xSemaphoreCreateMutex();
    // put your setup code here, to run once:
    Serial.begin(921600);
    xTaskCreatePinnedToCore(
                            loop0,   /* Function to implement the task */
                            "loop0", /* Name of the task */
                            10000,      /* Stack size in words */
                            NULL,       /* Task input parameter */
                            0,          /* Priority of the task */
                            NULL,       /* Task handle. */
                            0  /* Core where the task should run */
    );
}

void loop() {
    // put your main code here, to run repeatedly:
    currentCore = xPortGetCoreID();
    Serial.print("Hello World: "); Serial.println(currentCore);
    delay(250);
}

```
So, things look mostly the same. However, note that, at the top, we declared a new global variable, `currentCore`. This variable, since it's global, can be accessed by the entire program, including both tasks. Next, notice how we use this variable in each task. In both tasks, we get and store the ID of the core in `currentCore`. Then, we use `currentCore` in our print statement instead of directly using the `xPortGetCoreID` function call. At first glance, you might think "so what's the big deal?" Well, let's think about it. The contents of `loop` and `loop0` are executed on different cores. This means that they both execute simultaneously. Furthermore, there are no guaruntees about the lines `loop` and `loop0` execute simultaneously, so any pair of lines of code could run. So, think about it: what would happen if `loop0` ran its line calling `xPortGetCoreID` right after `loop` ran its line calling `xPortGetCoreID` but right before `loop` ran its serial write lines? Well, `currentCore` is only one variable shared by the two tasks, so when `loop0` ran its line, `currentCore` naturally got updated to 0. So, when `loop` runs its serial commands, it would use the current value of `currentCore`, which is 0! But its running on Core 1! This isn't what we want at all!

This little bug is an example of what is called a race condition. Race conditions are nasty little bugs that occurr in multi-threaded applications. What makes race conditions so onerous is the difficulty in figuring out a race condition is even occurring! Look at our example. That `loop` could ever possibly print 0 instead of 1 means that our program is wrong and has a bug that needs to be fixed! However, if build the code and test it, it will work correctly! How can this be!? Well, even though `loop` _can_ print 0 in certain circumstances, the odds of those conditions occurring are rare. Since the bug is so rare, it's easy not to notice it in testing. But it _can_ happen, which means it _will_ happen, usually when you least want it to!

#### Thread-Safe: Semaphores
So, race conditions are awful. But, multi-threading is really cool and useful! How do we use multi-threading without causing terrible race conditions? Fear not, FreeRTOS has a way and that way is semaphores! Semaphores are special variables that allow multiple, simultaneous tasks to synchronize with each other. This allows multiple tasks to share data in a way that ensures all of the tasks take turns and don't trip over each other in a race condition. To start, let's declare the semaphore by adding the following line to your program right below where `currentCore` is declared:
```c++
SemaphoreHandle_t mutex;
```
This declares a variable, `mutex`, that will contain the semaphore. If you're curious about why the name `mutex` was used, mutex is short MUTual EXclusion. The idea is that the semaphore is _mutally exclusive_, meaning only one task can use the semaphore at a time. While one task has the semaphore, the others can't access it and have to wait their turn instead. Think of it like a relay race: only the runner (task) with the baton (semaphore) can actually run; other runners must wait until the baton holder gives them the baton. This allows us to make some guaruntees about the order and synchronocity of operations between multiple tasks.

Next, we need to create a semaphore to put in `mutex`. At beginning of `setup`, add the following line:
```c++
    mutex = xSemaphoreCreateMutex();
```

Now that we have a `mutex` semaphore, let's use it! We need to add some commands to tell both of our tasks that they can only use `currentCore` (read or write!) when they have the semaphore. Thus, each task needs two extra lines of code: one line to tell it to take the semaphore (or wait its turn if the semaphore isn't available) and one to return the semaphore when it's done using `currentCore`. After adding these lines, your code should look like this:
```c++
#include <Arduino.h>

int currentCore;
SemaphoreHandle_t mutex;

void loop0(void * parameter) {
    while(1) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        currentCore = xPortGetCoreID();
        Serial.print("Hello World: "); Serial.println(currentCore);
        xSemaphoreGive(mutex);
        delay(250);
    }
}

void setup() {
    mutex = xSemaphoreCreateMutex();
    // put your setup code here, to run once:
    Serial.begin(921600);
    xTaskCreatePinnedToCore(
                            loop0,   /* Function to implement the task */
                            "loop0", /* Name of the task */
                            10000,      /* Stack size in words */
                            NULL,       /* Task input parameter */
                            0,          /* Priority of the task */
                            NULL,       /* Task handle. */
                            0  /* Core where the task should run */
    );
}

void loop() {
    // put your main code here, to run repeatedly:
    xSemaphoreTake(mutex, portMAX_DELAY);
    currentCore = xPortGetCoreID();
    Serial.print("Hello World: "); Serial.println(currentCore);
    xSemaphoreGive(mutex);
    delay(250);
}
```
What we've done is wrap the parts of our tasks that use `currentCore` in a pair of FreeRTOS calls: `xSemaphoreTake` and `xSemaphoreGive`. First, we call `xSemaphoreTake`, passing in two parameters. The first parameter, `mutex`, makes sense: it's the semaphore we want to take. The second parameter is how long the function call should wait for the semaphore before giving up. Here we use `portMAX_DELAY`, which is a FreeRTOS constant. This value is the longest possible time `xSemaphoreTake` can wait. So, this line instructs both of the tasks to take the semaphore before using `currentCore`. Both tasks will also wait if the semaphore is unavailable (when the other task is using `currentCore`!). Next, `xSemaphoreGive` returns the mutex, making it available for the next task in line to take it. With these calls, our program is now thread-safe! If both tasks want to use `currentCore` at the same time, our mutex semaphore will force one to wait until the other finishes. Since each task only uses `currentCore` for a few microseconds every 250 milliseconds, the effect the semaphores have on our code is imperceptible, causing only a few microseconds of delay in one task in the very rare case the two tasks want to access `currentCore` at the same time. This small addition ensures our program never encounters a race condition and fails! If you check the serial console now, you will see both tasks happily taking turns printing `Hello World` with their corresponding Core ID's!

## Conclusion
In this tutorial, you learned how to create a FreeRTOS task and run it on the ESP32's other core. Additionally, you learned how to be careful about race conditions and use semaphores to ensure they never happen! Next, we'll start playing with another unique feature of the ESP32: WiFi!