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
