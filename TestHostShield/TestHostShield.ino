#include <hidboot.h>
#include <usbhub.h>
#include <SPI.h>

// Create a MouseRptParser class to handle mouse input
class MouseRptParser : public MouseReportParser {
protected:
    void OnMouseMove(MOUSEINFO *mi);    // Mouse move detection
    void OnLeftButtonDown(MOUSEINFO *mi); // Left button down
    void OnLeftButtonUp(MOUSEINFO *mi);   // Left button up
    void OnRightButtonDown(MOUSEINFO *mi); // Right button down
    void OnRightButtonUp(MOUSEINFO *mi);   // Right button up
    void OnMiddleButtonDown(MOUSEINFO *mi); // Middle button down
    void OnMiddleButtonUp(MOUSEINFO *mi);   // Middle button up
};

// Mouse move event: print the changes in X and Y axis
void MouseRptParser::OnMouseMove(MOUSEINFO *mi) {
    Serial.print("dx=");
    Serial.print(mi->dX, DEC);  // X-axis movement
    Serial.print(" dy=");
    Serial.println(mi->dY, DEC);  // Y-axis movement
}

// Button press events
void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi) {
    Serial.println("Left Button Pressed");
}

void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi) {
    Serial.println("Left Button Released");
}

void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi) {
    Serial.println("Right Button Pressed");
}

void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi) {
    Serial.println("Right Button Released");
}

void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi) {
    Serial.println("Middle Button Pressed");
}

void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi) {
    Serial.println("Middle Button Released");
}

// USB and Mouse setup
USB Usb;
USBHub Hub(&Usb);  // Hub for connected USB devices
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb); // Mouse protocol
MouseRptParser Prs;  // Mouse parser object to handle the data

void setup() {
    Serial.begin(115200);
#if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial connection (important for boards like Leonardo)
#endif
    Serial.println("Starting USB Host Shield...");

    // Initialize USB Host Shield
    if (Usb.Init() == -1) {
        Serial.println("USB Host Shield initialization failed!");
        while (1); // Halt if initialization fails
    }
    Serial.println("USB Host Shield initialized successfully.");

    // Set the report parser for mouse input
    HidMouse.SetReportParser(0, &Prs);
}

void loop() {
    Usb.Task(); // Handle USB events (detect devices, mouse movement, button presses)
}
