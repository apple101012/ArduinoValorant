#include <Usb.h>
#include <hidboot.h>

uint8_t rawHidData[64];  // Buffer for incoming Raw HID data

USB Usb;
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);  // Using the HIDBoot class for mouse

class MouseRptParser : public MouseReportParser {
  protected:
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) override {
      MouseReportParser::Parse(hid, is_rpt_id, len, buf);

      // Print out the entire buffer to the serial monitor
      if(len > 0) {
        Serial.print("Buffer: ");
        for (uint8_t i = 0; i < len; i++) {
          Serial.print(buf[i], HEX);  // Print each byte in hexadecimal
          Serial.print(" ");
        }
        Serial.println();
      }
    }
};

MouseRptParser mouseParser;

void setup() {
  Serial.begin(115200);  // Start the Serial Monitor
  Usb.Init();  // Initialize USB Host
  HidMouse.SetReportParser(0, &mouseParser);  // Set up the mouse report parser
}

void loop() {
  Usb.Task();  // Process USB mouse input
  delay(100);  // Small delay to prevent excessive serial output
}
