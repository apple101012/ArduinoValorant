import hid
import time

# Set up your Arduino's VID & PID
VENDOR_ID = 0x2341
PRODUCT_ID = 0x8036

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

def send_mouse(x, y):
    """Send HID mouse movement data to Arduino"""
    x = unsign(x)
    y = unsign(y)
    report = [0, x, y]  # Button is 0 (no buttons pressed), x and y are the movement values
    print(f"Sending: {report}")
    device.write(report)

def move_mouse_to(x_target, y_target, delay=0.05):
    """Move the mouse gradually to (x_target, y_target)"""
    x_current = 0
    y_current = 0

    # Gradually move towards the target (x_target, y_target)
    while x_current != x_target or y_current != y_target:
        if x_current < x_target:
            x_current += 1
        elif x_current > x_target:
            x_current -= 1

        if y_current < y_target:
            y_current += 1
        elif y_current > y_target:
            y_current -= 1

        # Send mouse movement data
        send_mouse(x_current, y_current)

        # Delay to slow down the movement
        time.sleep(delay)

    print("Movement complete!")

# Example: Move the mouse gradually to (100, 100)
move_mouse_to(100, 100, delay=0.05)

device.close()
