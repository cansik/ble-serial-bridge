import serial
import time

port = serial.Serial('/dev/tty.usbserial-14410', 115200, timeout=1)

device_id = "30:ae:a4:14:e7:62"
service_id = "846123f6-ccf1-11ea-87d0-0242ac130003"
neopixel_color_id = "fc3affa6-5020-47ce-93db-2e9dc45c9b55"
imu_id = "98f09e34-73ab-4f2a-a5eb-a95e7e7ab733"

test_notify = False


def millis():
    return time.time_ns() // 1000000


def send_and_read(command, wait_time=100, skip_receiving=False):
    print(command)
    message = "%s\n" % command
    port.write(message.encode('utf_8'))
    stime = millis()

    if skip_receiving:
        time.sleep(wait_time / 1000)

    while millis() - stime < wait_time and not skip_receiving:
        result = port.readline().decode('utf_8')
        if result.strip() != "":
            print(result.strip())


send_and_read("connect %s" % device_id)

send_and_read("write %s %s %s %s %s" % (device_id, service_id, neopixel_color_id, 0x0000FF, "i32"))
send_and_read("read %s %s %s %s" % (device_id, service_id, neopixel_color_id, "i32"))

time.sleep(2)

send_and_read("disconnect %s" % device_id)
port.close()
exit(0)

if test_notify:
    send_and_read("register %s %s %s" % (device_id, service_id, imu_id), wait_time=5000)

# test text
send_and_read("write %s %s %s %s" % (device_id, service_id, neopixel_color_id, "hello"))
send_and_read("read %s %s %s" % (device_id, service_id, neopixel_color_id))

# test i8
send_and_read("write %s %s %s %s %s" % (device_id, service_id, neopixel_color_id, 22, "i8"))
send_and_read("read %s %s %s %s" % (device_id, service_id, neopixel_color_id, "i8"))

# test i16
send_and_read("write %s %s %s %s %s" % (device_id, service_id, neopixel_color_id, 1000, "i16"))
send_and_read("read %s %s %s %s" % (device_id, service_id, neopixel_color_id, "i16"))

# test i32
send_and_read("write %s %s %s %s %s" % (device_id, service_id, neopixel_color_id, 0x00FFFF, "i32"))
send_and_read("read %s %s %s %s" % (device_id, service_id, neopixel_color_id, "i32"))

time.sleep(5)

send_and_read("disconnect %s" % device_id)
port.close()
