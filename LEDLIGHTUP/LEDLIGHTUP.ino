#include <Wire.h>
#include <usbhub.h>

USBHost usb;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting USB Host Shield...");

    if (usb.Init() == -1) {
        Serial.println("USB Host Shield initialization failed!");
        while (1); // Halt execution if initialization fails
    }
    Serial.println("USB Host Shield initialized successfully.");
}

void loop() {
    usb.Task(); // Keeps USB communication running
}
