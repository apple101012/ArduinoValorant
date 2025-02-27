#include <Usb.h>
#include <Mouse.h>
#include <Wire.h> 
#include <SPI.h> 
#include <usbhub.h>

USB         Usb; 
USBHub      Hub(&Usb); 

byte bf[2];

#include <hidboot.h> 

HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb); 

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

class MouseRptParser : public MouseReportParser {
  protected: 
    void OnMouseMove  (MOUSEINFO *mi);
    void OnLeftButtonUp (MOUSEINFO *mi); 
    void OnLeftButtonDown (MOUSEINFO *mi); 
    void OnRightButtonUp  (MOUSEINFO *mi); 
    void OnRightButtonDown  (MOUSEINFO *mi); 
    void OnMiddleButtonUp (MOUSEINFO *mi); 
    void OnMiddleButtonDown (MOUSEINFO *mi);
    void OnXB1ButtonUp (MOUSEINFO *mi); 
    void OnXB1ButtonDown (MOUSEINFO *mi);
    void OnXB2ButtonUp (MOUSEINFO *mi); 
    void OnXB2ButtonDown (MOUSEINFO *mi);
}; 


void MouseRptParser::OnMouseMove(MOUSEINFO *mi) { 
  dx = mi->dX; 
  dy = mi->dY; 
  dz = mi->dZ;
  Serial.print("Mouse moved: X=");
  Serial.print(dx);
  Serial.print(", Y=");
  Serial.print(dy);
  Serial.print(", Scroll=");
  Serial.println(dz);
};


void MouseRptParser::OnLeftButtonUp (MOUSEINFO *mi) { 
  lmb = 0; 
  Serial.println("Left Button Released");
};

void MouseRptParser::OnLeftButtonDown (MOUSEINFO *mi) { 
  lmb = 1; 
  Serial.println("Left Button Pressed");
}; 

void MouseRptParser::OnRightButtonUp  (MOUSEINFO *mi) { 
  rmb = 0; 
  Serial.println("Right Button Released");
};

void MouseRptParser::OnRightButtonDown  (MOUSEINFO *mi) { 
  rmb = 1; 
  Serial.println("Right Button Pressed");
}; 

void MouseRptParser::OnMiddleButtonUp (MOUSEINFO *mi) {
  mmb = 0; 
  Serial.println("Middle Button Released");
}; 

void MouseRptParser::OnMiddleButtonDown (MOUSEINFO *mi) { 
  mmb = 1; 
  Serial.println("Middle Button Pressed");
};

void MouseRptParser::OnXB1ButtonUp (MOUSEINFO *mi) {
  xb1 = 0; 
  Serial.println("XB1 Button Released");
}; 

void MouseRptParser::OnXB1ButtonDown (MOUSEINFO *mi) { 
  xb1 = 1; 
  Serial.println("XB1 Button Pressed");
};

void MouseRptParser::OnXB2ButtonUp (MOUSEINFO *mi) {
  xb2 = 0; 
  Serial.println("XB2 Button Released");
}; 

void MouseRptParser::OnXB2ButtonDown (MOUSEINFO *mi) { 
  xb2 = 1; 
  Serial.println("XB2 Button Pressed");
};


MouseRptParser  Prs; 

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000);  // Wait up to 5 seconds for Serial (if available)

  Mouse.begin();
  Usb.Init();
  HidMouse.SetReportParser(0, &Prs);

  Serial.println("USB Mouse Ready!");
}


void loop() {
  dx = 0; 
  dy = 0; 
  j = 0; 
  c = 0; 
  e = 0;

  Usb.Task();

  if (Serial.available()) {
    Serial.readBytes(bf, 2);
    Mouse.move(bf[0], bf[1], 0);
  } else {
    Mouse.move(dx, dy, dz);
    dz = 0;
  }

  if (lmb == 0){ 
    Mouse.release(MOUSE_LEFT); 
  } else if (lmb == 1){ 
    Mouse.press(MOUSE_LEFT); 
  }

  if (rmb == 0){ 
    Mouse.release(MOUSE_RIGHT); 
  } else if (rmb == 1){ 
    Mouse.press(MOUSE_RIGHT); 
  } 

  if (mmb == 0) { 
    Mouse.release(MOUSE_MIDDLE); 
  } else if (mmb == 1){ 
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
}
