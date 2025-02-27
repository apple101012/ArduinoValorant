#include <hidboot.h>
#include <usbhub.h>
#include <SPI.h>
#include <Mouse.h>  // Arduino Mouse Library for controlling cursor

// Class to handle mouse input from the USB Host Shield
class MouseRptParser : public MouseReportParser {
protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
};

// Move cursor based on external mouse movement
void MouseRptParser::OnMouseMove(MOUSEINFO *mi) {
    Mouse.move(mi->dX, mi->dY, 0);  // Move cursor by dx, dy
}

// Button events - translate to Arduino Mouse functions
void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi) {
    Mouse.press(MOUSE_LEFT);
}

void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi) {
    Mouse.release(MOUSE_LEFT);
}

void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi) {
    Mouse.press(MOUSE_RIGHT);
}

void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi) {
    Mouse.release(MOUSE_RIGHT);
}

void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi) {
    Mouse.press(MOUSE_MIDDLE);
}

void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi) {
    Mouse.release(MOUSE_MIDDLE);
}

// USB and Mouse setup
USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);
MouseRptParser Prs;

void setup() {
    Serial.begin(115200);
#if !defined(__MIPSEL__)
    while (!Serial);
#endif
    Serial.println("Starting USB Host Shield...");

    if (Usb.Init() == -1) {
        Serial.println("USB Host Shield initialization failed!");
        while (1);
    }
    Serial.println("USB Host Shield initialized successfully.");

    HidMouse.SetReportParser(0, &Prs);

    Mouse.begin();  // Start Arduino Mouse emulation
}

void loop() {
    Usb.Task(); // Process USB input events
}
