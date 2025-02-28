    #include "Mouse.h"
     
    #if defined(_USING_HID)
     
    static const uint8_t _hidReportDescriptor[] PROGMEM = {
      
      //  Mouse
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,        // Usage (Mouse)
    0xA1, 0x01,        // Collection (Application)
    0x09, 0x01,        //   Usage (Pointer)
    0xA1, 0x00,        //   Collection (Physical)
    0x05, 0x09,        //     Usage Page (Button)
    0x19, 0x01,        //     Usage Minimum (Button 1)
    0x29, 0x05,        //     Usage Maximum (Button 5) (Added extra buttons)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x05,        //     Report Count (5 buttons)
    0x81, 0x02,        //     Input (Data,Var,Abs)
    0x75, 0x03,        //     Report Size (3 bits, padding)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x03,        //     Input (Cnst,Var,Abs)
    0x05, 0x01,        //     Usage Page (Generic Desktop)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x09, 0x38,        //     Usage (Wheel)
    0x15, 0x81,        //     Logical Minimum (-127)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x03,        //     Report Count (3)
    0x81, 0x06,        //     Input (Data,Var,Rel)
    0x05, 0x0C,        //     Usage Page (Consumer)
    0x0A, 0x38, 0x02,  //     Usage (AC Pan) (Horizontal scroll)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x06,        //     Input (Data,Var,Rel)
    0xC0,              //   End Collection
    0xC0               // End Collection
    };
     
    //================================================================================
    //================================================================================
    //	Mouse
     
    Mouse_::Mouse_(void) : _buttons(0)
    {
        static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
        HID().AppendDescriptor(&node);
    }
     
    void Mouse_::begin(void) 
    {
    }
     
    void Mouse_::end(void) 
    {
    }
     
    void Mouse_::click(uint8_t b)
    {
    	_buttons = b;
    	move(0,0,0);
    	_buttons = 0;
    	move(0,0,0);
    }
     
    void Mouse_::move(signed char x, signed char y, signed char wheel)
    {
    	uint8_t m[4];
    	m[0] = _buttons;
    	m[1] = x;
    	m[2] = y;
    	m[3] = wheel;
    	HID().SendReport(1,m,4);
    }
     
    void Mouse_::buttons(uint8_t b)
    {
    	if (b != _buttons)
    	{
    		_buttons = b;
    		move(0,0,0);
    	}
    }
     
    void Mouse_::press(uint8_t b) 
    {
    	buttons(_buttons | b);
    }
     
    void Mouse_::release(uint8_t b)
    {
    	buttons(_buttons & ~b);
    }
     
    bool Mouse_::isPressed(uint8_t b)
    {
    	if ((b & _buttons) > 0) 
    		return true;
    	return false;
    }
     
    Mouse_ Mouse;
     
    #endif