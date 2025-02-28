import hid
import time
import threading
import numpy as np
import keyboard
import win32api
from mss import mss
import json

# Set up your Arduino's VID & PID
VENDOR_ID = 0x2341
PRODUCT_ID = 0x8036

# Open HID device (Arduino)
try:
    device = hid.device()
    device.open(VENDOR_ID, PRODUCT_ID)
    print("Connected to Arduino Mouse")
except Exception as e:
    print(f"Connection failed: {e}")
    exit()


def unsign(value):
    """Ensure values are treated as unsigned 8-bit integers"""
    return value & 0xFF


def send_mouse(leftclick, x, y):
    """Send HID mouse movement data to Arduino"""
    leftclick = unsign(leftclick)
    x = unsign(x)
    y = unsign(y)
    report = [0, x, y, leftclick]  # Button is 0 (no buttons pressed), x and y are the movement values
    print(f"Sending: {report}")
    device.write(report)

# Triggerbot configuration
class Triggerbot:
    def __init__(self):
        # Hardcoded config values
        self.sct = mss()
        self.triggerbot = False
        self.triggerbot_toggle = True
        self.exit_program = False  # Flag to indicate whether to exit the program
        self.toggle_lock = threading.Lock()

        # Hardcoded config values (These values replace the ones from the JSON)
        self.keybind_toggle = 0x4F  # Keybind for toggle (Right Shift)
        self.always_enabled = False
        self.trigger_delay = 40
        self.base_delay = 0.0168
        self.color_tolerance = 70
        self.monitor_id = 1  # Monitor to capture from
        self.R, self.G, self.B = (250, 100, 250)  # Purple color
        self.screenshot = self.sct.monitors[self.monitor_id]

    def cooldown(self):
        time.sleep(0.1)
        with self.toggle_lock:
            self.triggerbot_toggle = True
            # Beep to indicate triggerbot status change
            # This is just for feedback, remove or replace with other functionality if needed
            print("Triggerbot toggled", "ON" if self.triggerbot else "OFF")

    def searcherino(self):
        frame = np.array(self.sct.grab(self.screenshot))
        img = frame[100:300, 100:300]  # Change this zone as needed

        pmap = np.array(img)
        pixels = pmap.reshape(-1, 4)
        color_mask = (
                (pixels[:, 0] > self.R - self.color_tolerance) & (pixels[:, 0] < self.R + self.color_tolerance) &
                (pixels[:, 1] > self.G - self.color_tolerance) & (pixels[:, 1] < self.G + self.color_tolerance) &
                (pixels[:, 2] > self.B - self.color_tolerance) & (pixels[:, 2] < self.B + self.color_tolerance)
        )
        matching_pixels = pixels[color_mask]

        if self.triggerbot and len(matching_pixels) > 0:
            delay_percentage = self.trigger_delay / 100.0  # Convert to a decimal value

            actual_delay = self.base_delay + self.base_delay * delay_percentage
            time.sleep(actual_delay)

            # Send HID input to Arduino to simulate a click
            send_mouse(1,0, 0)  # This could be mouse movement or you could simulate a click, e.g. send a mouse button press here
            send_mouse(0, 0, 0)
            # Beep feedback on action
            print("Triggerbot action - Mouse click simulated")
            time.sleep(0.005)

    def toggle(self):
        if keyboard.is_pressed("f10"):
            with self.toggle_lock:
                if self.triggerbot_toggle:
                    self.triggerbot = not self.triggerbot
                    print("Triggerbot", "Activated" if self.triggerbot else "Deactivated")
                    self.triggerbot_toggle = False
                    threading.Thread(target=self.cooldown).start()

    def hold(self):
        while True:
            while win32api.GetAsyncKeyState(self.keybind_toggle) < 0:
                self.triggerbot = True
                self.searcherino()
            else:
                time.sleep(0.1)

    def starterino(self):
        while not self.exit_program:  # Keep running until the exit_program flag is True
            if self.always_enabled:
                self.toggle()
                self.searcherino() if self.triggerbot else time.sleep(0.1)
            else:
                self.hold()


# Run triggerbot
triggerbot = Triggerbot()
triggerbot.starterino()

device.close()
