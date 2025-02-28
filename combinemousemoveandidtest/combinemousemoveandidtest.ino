#include <HID-Project.h>
#include <Usb.h>
#include <hidboot.h>

uint8_t rawHidData[64];  // Buffer for incoming Raw HID data.

USB Usb;
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

const uint8_t encryption_key = 0x55;  // Same XOR key as Python
uint8_t xor_decrypt(uint8_t value) {
    return value ^ encryption_key;
}

int8_t dx = 0, dy = 0, dz = 0;  // Mouse movement and scroll variables
int lmb = 0, rmb = 0, mmb = 0, xb1 = 0, xb2 = 0;  // Mouse button states

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
}

void loop() {
  Usb.Task();  // Process USB mouse input

  int8_t x = 0, y = 0, scroll = 0;

  if (RawHID.available()) {
    RawHID.read();
    x = xor_decrypt(rawHidData[0]);  // Decrypt x using XOR
    y = xor_decrypt(rawHidData[1]);  // Decrypt y using XOR
    RawHID.enable();
  } else {
    x = dx;  // Use USB mouse input
    y = dy;
    scroll = 0;
  }

  // Move mouse
  Mouse.move(x, y, scroll);

  // Reset movement values
  dx = 0;
  dy = 0;
  dz = 0;

  // Handle button presses
  if (lmb) Mouse.press(MOUSE_LEFT); else Mouse.release(MOUSE_LEFT);
  if (rmb) Mouse.press(MOUSE_RIGHT); else Mouse.release(MOUSE_RIGHT);
  if (mmb) Mouse.press(MOUSE_MIDDLE); else Mouse.release(MOUSE_MIDDLE);
  if (xb1) Mouse.press(0x08); else Mouse.release(0x08); // XButton1 (Back Button)
  if (xb2) Mouse.press(0x10); else Mouse.release(0x10); // XButton2 (Forward Button)
}
