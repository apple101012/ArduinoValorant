#include <HID-Project.h>
#include <Usb.h>
#include <hidboot.h>

uint8_t rawHidData[64];  // Buffer for incoming Raw HID data.

USB Usb;
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

int8_t dx = 0, dy = 0;  // Mouse movement variables

class MouseParser : public MouseReportParser {
  protected:
    void OnMouseMove(MOUSEINFO *mi) override {
      dx = mi->dX;
      dy = mi->dY;
    }
};

MouseParser mouseParser;

void setup() {
  Mouse.begin();
  RawHID.begin(rawHidData, sizeof(rawHidData));  // Start RawHID
  Usb.Init();  // Initialize USB Host
  HidMouse.SetReportParser(0, &mouseParser);
}

void loop() {
  Usb.Task();  // Process USB mouse input

  int8_t x = 0, y = 0;

  if (RawHID.available()) {
    RawHID.read();
    x = rawHidData[0];
    y = rawHidData[1];
    RawHID.enable();  // Flush data
  } else {
    x = dx;  // Use USB mouse input
    y = dy;
  }

  Mouse.move(x, y);
  dx = 0;  // Reset values after moving
  dy = 0;
}
