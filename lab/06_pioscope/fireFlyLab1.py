import board
import busio
import time
import adafruit_apds9960.apds9960 as apds9960

# Set up the APDS9960 color sensor
i2c = busio.I2C(board.SCL, board.SDA)
sensor = apds9960.APDS9960(i2c)
sensor.enable_color_sensor()
sensor.enable_proximity_sensor()
sensor.set_adc_integration_time(50)
sensor.set_gain(apds9960.GAIN_4X)

# Set up the RGB LED
led_r = board.LED_R
led_g = board.LED_G
led_b = board.LED_B

# Track the brightness reading from the color sensor and update the LED accordingly
while True:
    clear = sensor.color_data[3]
    brightness = clear * 255 // 65535
    led_r.duty_cycle = brightness
    led_g.duty_cycle = brightness
    led_b.duty_cycle = brightness
    time.sleep(0.1)
