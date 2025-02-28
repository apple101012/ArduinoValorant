#include <HID-Project.h>
#include <Usb.h>
#include <hidboot.h>

uint8_t rawHidData[64];  // Buffer for incoming Raw HID data.

USB Usb;
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

int8_t dx = 0, dy = 0, dz = 0;  // Mouse movement and scroll variables
int lmb = 0, rmb = 0, mmb = 0, xb1 = 0, xb2 = 0;  // Mouse button states
int fakelmb = 0;

class MouseParser : public MouseReportParser {
  protected:
    void OnMouseMove(MOUSEINFO *mi) override {
      dx = mi->dX;
      dy = mi->dY;
      dz = mi->dZ;  // Capture scroll movement
    }

    void OnLeftButtonDown(MOUSEINFO *mi) override { lmb = 1; }
    void OnLeftButtonUp(MOUSEINFO *mi) override { lmb = 0; }

    void OnRightButtonDown(MOUSEINFO *mi) override { rmb = 1; }
    void OnRightButtonUp(MOUSEINFO *mi) override { rmb = 0; }

    void OnMiddleButtonDown(MOUSEINFO *mi) override { mmb = 1; }
    void OnMiddleButtonUp(MOUSEINFO *mi) override { mmb = 0; }

    void OnXB1ButtonDown(MOUSEINFO *mi) override { xb1 = 1; }
    void OnXB1ButtonUp(MOUSEINFO *mi) override { xb1 = 0; }

    void OnXB2ButtonDown(MOUSEINFO *mi) override { xb2 = 1; }
    void OnXB2ButtonUp(MOUSEINFO *mi) override { xb2 = 0; }
};

MouseParser mouseParser;

void setup() {
  Mouse.begin();
  RawHID.begin(rawHidData, sizeof(rawHidData));  // Start RawHID
  Usb.Init();  // Initialize USB Host
  HidMouse.SetReportParser(0, &mouseParser);
//
//  Serial.begin(115200);
}

void loop() {
  Usb.Task();  // Process USB mouse input

  int8_t x = 0, y = 0, scroll = 0;

  if (RawHID.available()) {
    RawHID.read();
    x = rawHidData[0];
    y = rawHidData[1];
//   lmb = rawHidData[2];

    RawHID.enable();  // Flush data
  } else {
    x = dx;  // Use USB mouse input
    y = dy;
    scroll = 0;
  }

  // Move mouse
  Mouse.move(x, y, scroll);
//  Serial.print("x: "); Serial.println(dz);
  x = 0;
  y = 0;
  dx = 0;
  dy = 0;
  dz = 0;
  // Handle button presses
  if (lmb == 0) { Mouse.release(MOUSE_LEFT); } else { Mouse.press(MOUSE_LEFT); }
  if (rmb == 0) { Mouse.release(MOUSE_RIGHT); } else { Mouse.press(MOUSE_RIGHT); }
  if (mmb == 0) { Mouse.release(MOUSE_MIDDLE); } else { Mouse.press(MOUSE_MIDDLE); }
}
