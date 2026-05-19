import time
import math
from rpi_ws281x import PixelStrip, Color

LED_COUNT = 30
LED_PIN = 18
LED_FREQ_HZ = 800000
LED_DMA = 10
LED_BRIGHTNESS = 100
LED_INVERT = False
LED_CHANNEL = 0

strip = PixelStrip(
    LED_COUNT,
    LED_PIN,
    LED_FREQ_HZ,
    LED_DMA,
    LED_INVERT,
    LED_BRIGHTNESS,
    LED_CHANNEL
)

strip.begin()

def set_all(r, g, b):
    for i in range(strip.numPixels()):
        strip.setPixelColor(i, Color(r, g, b))
    strip.show()

# blue breathing effect for 3 seconds
start = time.time()

while time.time() - start < 3:

    brightness = (math.sin(time.time() * 4) + 1) / 2

    r = 0
    g = 0
    b = int(255 * brightness)

    set_all(r, g, b)

    time.sleep(0.02)

# turn LEDs off
set_all(0, 0, 0)
