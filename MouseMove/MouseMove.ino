#include <Usb.h>
#include <Mouse.h>
#include <Wire.h>
#include <SPI.h>
#include <usbhub.h>

USB Usb;
USBHub Hub(&Usb);

byte bf[2];

#include <hidboot.h>

HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

String myString;
int j = 0;
int c = 0;
int e = 0;
int lmb = 0;
int rmb = 0;
int mmb = 0;
int xb1 = 0;
int xb2 = 0;
int dx;
int dy;
int dz;
int arr[2];
int arrv[8];

// Define the MouseRptParser class to handle the mouse reports
class MouseRptParser : public MouseReportParser {
protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
    void OnXB1ButtonUp(MOUSEINFO *mi);
    void OnXB1ButtonDown(MOUSEINFO *mi);
    void OnXB2ButtonUp(MOUSEINFO *mi);
    void OnXB2ButtonDown(MOUSEINFO *mi);
};

void MouseRptParser::OnMouseMove(MOUSEINFO *mi) {
    dx = mi->dX;
    dy = mi->dY;
    dz = mi->dZ;
}

void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi) {
    lmb = 0;
}

void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi) {
    lmb = 1;
}

void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi) {
    rmb = 0;
}

void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi) {
    rmb = 1;
}

void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi) {
    mmb = 0;
}

void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi) {
    mmb = 1;
}

void MouseRptParser::OnXB1ButtonUp(MOUSEINFO *mi) {
    xb1 = 0;
}

void MouseRptParser::OnXB1ButtonDown(MOUSEINFO *mi) {
    xb1 = 1;
}

void MouseRptParser::OnXB2ButtonUp(MOUSEINFO *mi) {
    xb2 = 0;
}

void MouseRptParser::OnXB2ButtonDown(MOUSEINFO *mi) {
    xb2 = 1;
}

MouseRptParser Prs;

void setup() {
    Mouse.begin();
    Usb.Init(); // Initialize the USB host shield
    HidMouse.SetReportParser(0, &Prs);
    delay(2000); // Wait for the USB host shield to initialize
}

void loop() {
  
    dx = 0;
    dy = 0;
    dz = 0;

    Usb.Task();  // Handle the USB tasks

    // Check if mouse data has been received
    if (dx != 0 || dy != 0) {
        Mouse.move(dx, dy, dz);
        dz = 0;
    } else {
        Mouse.move(0, 0, 0); // No movement detected, so don't move the mouse
    }

    // Handle mouse button presses based on the parsed button states
    if (lmb == 0) {
        Mouse.release(MOUSE_LEFT);
    } else if (lmb == 1) {
        Mouse.press(MOUSE_LEFT);
    }

    if (rmb == 0) {
        Mouse.release(MOUSE_RIGHT);
    } else if (rmb == 1) {
        Mouse.press(MOUSE_RIGHT);
    }

    if (mmb == 0) {
        Mouse.release(MOUSE_MIDDLE);
    } else if (mmb == 1) {
        Mouse.press(MOUSE_MIDDLE);
    }

    if (xb1 == 0) {
        Mouse.release(MOUSE_XB1);
    } else if (xb1 == 1) {
        Mouse.press(MOUSE_XB1);
    }

    if (xb2 == 0) {
        Mouse.release(MOUSE_XB2);
    } else if (xb2 == 1) {
        Mouse.press(MOUSE_XB2);
    }

    delay(10); // Optional: to avoid CPU overuse, adjust as necessary
}
