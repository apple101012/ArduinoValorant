/* Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.

This software may be distributed and modified under the terms of the GNU
General Public License version 2 (GPL2) as published by the Free Software
Foundation and appearing in the file GPL2.TXT included in the packaging of
this file. Please note that GPL2 Section 2[b] requires that all works based
on this software must also be made publicly available under the terms of
the GPL2 ("Copyleft").

Contact information
-------------------

Circuits At Home, LTD
Web      :  http://www.circuitsathome.com
e-mail   :  support@circuitsathome.com
 */

#include "hidboot.h"

void MouseReportParser::Parse(USBHID *hid __attribute__((unused)), bool is_rpt_id __attribute__((unused)), uint8_t len __attribute__((unused)), uint8_t *buf) {
    MOUSEINFO *pmi = (MOUSEINFO*)buf;

    if (prevState.mouseInfo.bmLeftButton != pmi->bmLeftButton) {
        if (pmi->bmLeftButton) {
            OnLeftButtonDown(pmi);
        } else {
            OnLeftButtonUp(pmi);
        }
    }

    if (prevState.mouseInfo.bmRightButton != pmi->bmRightButton) {
        if (pmi->bmRightButton) {
            OnRightButtonDown(pmi);
        } else {
            OnRightButtonUp(pmi);
        }
    }

    if (prevState.mouseInfo.bmMiddleButton != pmi->bmMiddleButton) {
        if (pmi->bmMiddleButton) {
            OnMiddleButtonDown(pmi);
        } else {
            OnMiddleButtonUp(pmi);
        }
    }

    // Handle the extra mouse buttons (Side buttons)
    if (prevState.mouseInfo.bmXButton1 != pmi->bmXButton1) {
        if (pmi->bmXButton1) {
            OnXB1ButtonDown(pmi);
        } else {
            OnXB1ButtonUp(pmi);
        }
    }

    if (prevState.mouseInfo.bmXButton2 != pmi->bmXButton2) {
        if (pmi->bmXButton2) {
            OnXB2ButtonDown(pmi);
        } else {
            OnXB2ButtonUp(pmi);
        }
    }

    // Handle mouse movement
    if (pmi->dX || pmi->dY) {
        OnMouseMove(pmi);
    }

    // Update previous state
    prevState.bInfo[0] = buf[0];
}

void KeyboardReportParser::Parse(USBHID *hid, bool is_rpt_id __attribute__((unused)), uint8_t len __attribute__((unused)), uint8_t *buf) {
    // On error - return
    if (buf[2] == 1)
        return;

    if (prevState.bInfo[0x00] != buf[0x00]) {
        OnControlKeysChanged(prevState.bInfo[0x00], buf[0x00]);
    }

    for (uint8_t i = 2; i < 8; i++) {
        bool down = false;
        bool up = false;

        for (uint8_t j = 2; j < 8; j++) {
            if (buf[i] == prevState.bInfo[j] && buf[i] != 1)
                down = true;
            if (buf[j] == prevState.bInfo[i] && prevState.bInfo[i] != 1)
                up = true;
        }
        if (!down) {
            HandleLockingKeys(hid, buf[i]);
            OnKeyDown(*buf, buf[i]);
        }
        if (!up)
            OnKeyUp(prevState.bInfo[0], prevState.bInfo[i]);
    }
    for (uint8_t i = 0; i < 8; i++)
        prevState.bInfo[i] = buf[i];
}

const uint8_t KeyboardReportParser::numKeys[10] PROGMEM = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')'};
const uint8_t KeyboardReportParser::symKeysUp[12] PROGMEM = {'_', '+', '{', '}', '|', '~', ':', '"', '~', '<', '>', '?'};
const uint8_t KeyboardReportParser::symKeysLo[12] PROGMEM = {'-', '=', '[', ']', '\\', ' ', ';', '\'', '`', ',', '.', '/'};
const uint8_t KeyboardReportParser::padKeys[5] PROGMEM = {'/', '*', '-', '+', '\r'};

uint8_t KeyboardReportParser::OemToAscii(uint8_t mod, uint8_t key) {
    uint8_t shift = (mod & 0x22);

    // [a-z]
    if (VALUE_WITHIN(key, 0x04, 0x1d)) {
        if ((kbdLockingKeys.kbdLeds.bmCapsLock == 0 && shift) ||
            (kbdLockingKeys.kbdLeds.bmCapsLock == 1 && shift == 0))
            return (key - 4 + 'A');
        else
            return (key - 4 + 'a');
    }
    else if (VALUE_WITHIN(key, 0x1e, 0x27)) {
        if (shift)
            return ((uint8_t)pgm_read_byte(&getNumKeys()[key - 0x1e]));
        else
            return ((key == UHS_HID_BOOT_KEY_ZERO) ? '0' : key - 0x1e + '1');
    }
    else if(VALUE_WITHIN(key, 0x59, 0x61)) {
        if(kbdLockingKeys.kbdLeds.bmNumLock == 1)
            return (key - 0x59 + '1');
    } else if(VALUE_WITHIN(key, 0x2d, 0x38))
        return ((shift) ? (uint8_t)pgm_read_byte(&getSymKeysUp()[key - 0x2d]) : (uint8_t)pgm_read_byte(&getSymKeysLo()[key - 0x2d]));
    else if(VALUE_WITHIN(key, 0x54, 0x58))
        return (uint8_t)pgm_read_byte(&getPadKeys()[key - 0x54]);
    else {
        switch(key) {
            case UHS_HID_BOOT_KEY_SPACE: return (0x20);
            case UHS_HID_BOOT_KEY_ENTER: return ('\r');
            case UHS_HID_BOOT_KEY_ZERO2: return ((kbdLockingKeys.kbdLeds.bmNumLock == 1) ? '0': 0);
            case UHS_HID_BOOT_KEY_PERIOD: return ((kbdLockingKeys.kbdLeds.bmNumLock == 1) ? '.': 0);
        }
    }
    return (0);
}

// Added functions for extra mouse buttons
void MouseReportParser::OnXB1ButtonDown(MOUSEINFO *mi) {
    Serial.println("XButton1 Pressed");
}

void MouseReportParser::OnXB1ButtonUp(MOUSEINFO *mi) {
    Serial.println("XButton1 Released");
}

void MouseReportParser::OnXB2ButtonDown(MOUSEINFO *mi) {
    Serial.println("XButton2 Pressed");
}

void MouseReportParser::OnXB2ButtonUp(MOUSEINFO *mi) {
    Serial.println("XButton2 Released");
}
