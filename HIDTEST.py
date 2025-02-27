import hid

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

# Example: Move the mouse by 10px right, 10px down
send_mouse(10, 100)

device.close()
