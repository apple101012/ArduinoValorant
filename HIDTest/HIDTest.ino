#include <HID-Project.h>


uint8_t rawHidData[64];  // Adjusted size for raw HID data.

void setup() {
  Mouse.begin();
  RawHID.begin(rawHidData, sizeof(rawHidData));  // Start RawHID with data size
}

void loop() {
  // Variable to store the incoming buttons, x, y movement
  uint8_t buttons = 0;
  int8_t x = 0, y = 0;

  // Check if there is incoming HID data
  if (RawHID.available()) {
    int bytesRead = RawHID.read();  // Read the incoming data
    buttons = rawHidData[0];
    x = rawHidData[1];
    y = rawHidData[2];

    // Handle mouse button press based on incoming HID data
    if (buttons == 1) {
      Mouse.press(MOUSE_LEFT);
      Mouse.release(MOUSE_LEFT);
    }

    // Move the mouse based on the received x, y values
    Mouse.move(x, y);
    RawHID.enable();  // Flush the data from the rawHidData for the next reading
  } else {
    // If no data received, move the mouse like a normal mouse
    // Use your own logic for mouse movement when no data is available
    int defaultX = 0;  // Default movement values
    int defaultY = 0;
    
    // Move the mouse by the default values
    Mouse.move(defaultX, defaultY);

    // Optional: Add a slight delay to prevent overly fast movement
    delay(10);  // Adjust this delay if needed
  }
}
