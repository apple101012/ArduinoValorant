import time
import numpy as np
import cv2
import win32api
import hid
from mss import mss

class bob:
    def __init__(self):
        self.sct = mss()

        # Hardcoded config values
        self.aimbot_hotkey = 0x46  # Shift key (left) 0xA0
        self.experimental_filtering = False
        self.anti_astra = False
        self.enable_rcs = False
        self.x_fov = 41  #41
        self.y_fov = 30  #30
        self.cop = 1
        self.x_speed = 0.44   #0.44
        self.y_speed = 0.5    #0.5
        self.x_only = False
        self.custom_yoffset = 0
        self.encryption_key = 0x55
        # USB HID setup for Arduino
        self.VENDOR_ID = 0x1532
        self.PRODUCT_ID = 0x008A
        try:
            self.device = hid.device()
            self.device.open(self.VENDOR_ID, self.PRODUCT_ID)
            print("Connected to Arduino Mouse")
        except Exception as e:
            print(f"Connection failed: {e}")
            exit()

        # Set up screen capture area
        self.screenshot = self.sct.monitors[1]
        self.roundedgrabx = int(self.x_fov)
        self.roundedgraby = int(self.y_fov)
        self.screenshot['left'] = int((self.screenshot['width'] / 2) - (self.roundedgrabx / 2))
        self.screenshot['top'] = int((self.screenshot['height'] / 2) - (self.roundedgraby / 2))
        self.screenshot['width'] = self.roundedgrabx
        self.screenshot['height'] = self.roundedgraby
        self.center_x = self.roundedgrabx / 2
        self.center_y = self.roundedgraby / 2

        # Color filtering values
        if self.anti_astra:
            self.lower = np.array([140, 111, 160])
            self.upper = np.array([148, 154, 194])
        else:
            self.lower = np.array([140, 110, 150])
            self.upper = np.array([150, 195, 255])

        self.purple = (150, 119, 179)
        self.hue_threshold = 0
        self.similarity_threshold = (0.82, 0.85)

        # Aim offset tuning
        if self.experimental_filtering:
            self.cop_ready = {1: 3, 2: -1, 3: -4}.get(self.cop, 0)
        else:
            self.cop_ready = {1: 5, 2: 3, 3: -1}.get(self.cop, 0)

        if self.custom_yoffset != 0:
            self.cop_ready = self.custom_yoffset

    def xor_encrypt(self, value):
        return (value ^ self.encryption_key) & 0xFF
    # def send_mouse(self, leftclick, x, y):
    #     """Send HID mouse movement data to Arduino"""
    #     leftclick = leftclick & 0xFF
    #     x = x & 0xFF
    #     y = y & 0xFF
    #     report = [0, x, y]
    #     # report = [0, x, y, leftclick]  # Button is 0 (no buttons pressed), x and y are the movement values
    #     print(f"Sending: {report}")
    #     self.device.write(report)
    def send_mouse(self, leftclick, x, y):
        """Send HID mouse movement data to Arduino with XOR encryption"""
        leftclick = leftclick & 0xFF
        x = self.xor_encrypt(x)  # Encrypt x with XOR
        y = self.xor_encrypt(y)  # Encrypt y with XOR
        report = [0, x, y]
        print(f"Sending encrypted: {report}")
        self.device.write(report)

    def run(self):
        """Main aimbot loop"""
        if win32api.GetAsyncKeyState(self.aimbot_hotkey) < 0:
            print("errr")
            # Take screenshot
            img = np.array(self.sct.grab(self.screenshot))
            hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
            mask = cv2.inRange(hsv, self.lower, self.upper)
            kernel = np.ones((3, 3), np.uint8)
            dilated = cv2.dilate(mask, kernel, iterations=5)
            thresh = cv2.threshold(dilated, 60, 255, cv2.THRESH_BINARY)[1]
            contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

            if contours:
                # Find aim position
                M = cv2.moments(thresh)
                point_to_aim = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
                closestX = point_to_aim[0] + 1
                closestY = point_to_aim[1] - self.cop_ready
                diff_x = closestX - self.center_x
                diff_y = closestY - self.center_y
                target_x = int(diff_x * self.x_speed)
                target_y = int(diff_y * self.y_speed)

                if self.x_only:
                    target_y = 0

                if self.enable_rcs and win32api.GetAsyncKeyState(0x01) < 0:  # If left mouse button is pressed
                    time.sleep(0.001)
                    target_y += 6

                # Send movement to Arduino
                self.send_mouse(0, target_x, target_y)

    def starterino(self):
        """Run the aimbot continuously"""
        try:
            while True:
                self.run()
        except Exception as e:
            print("An exception occurred in the thread:", e)
        finally:
            self.device.close()  # Close HID device on exit

bot = bob()
bot.starterino()