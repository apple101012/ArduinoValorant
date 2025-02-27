#include <HID-Project.h>

uint8_t rawHidData[64];  // Adjusted size for raw HID data.

void setup() {
  Mouse.begin();
  RawHID.begin(rawHidData, sizeof(rawHidData));  // Start RawHID with data size
//  Serial.begin(9600);  // For debugging, print incoming HID data
}

void loop() {
  int8_t x = 0, y = 0;

  // Check if there is incoming HID data
  if (RawHID.available()) {
    int bytesRead = RawHID.read();  // Read the incoming data
    x = rawHidData[0];  // X movement
    y = rawHidData[1];  // Y movement

    // Debugging: Print the raw HID data for analysis
    Serial.print("Raw HID Data: ");
//    Serial.print("X: ");
//    Serial.print(x);
//    Serial.print(" Y: ");
//    Serial.println(y);

    // Move the mouse based on the received x, y values
    Mouse.move(x, y);
    RawHID.enable();  // Flush the data from the rawHidData for the next reading
  } else {
    // If no data received, move the mouse like a normal mouse
    int defaultX = 0;  // Default movement values
    int defaultY = 0;

    // Move the mouse by the default values
    Mouse.move(defaultX, defaultY);

    delay(10);  // Optional: Adjust this delay if needed
  }
}
