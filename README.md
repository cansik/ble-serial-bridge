# BLE Serial Bridge [![](https://jitpack.io/v/cansik/ble-serial-bridge.svg)](https://jitpack.io/#cansik/ble-serial-bridge)
A firmware to use an ESP32 as a BLE device controlled over serial commands.


### Driver

Currently there is a Java driver to use the Serial interface and a Python example script.

### API

#### Scanning

Scan surrounding devices for a specific `service id`.

```bash
scan interval window scan_time service_id
```

#### Connecting & Disconnecting

Connect and disconnect to a device.

```bash
connect device_id
disconnect device_id
```

#### List Connections

List all active BLE connections.

```bash
list
```

#### Read and Write

Connect and disconnect to a device.

```bash
read device_id service_id characteristic_id [format]
write device_id service_id characteristic_id value [format]
```

#### Notifications (Beta)

It is possible to register for a notification of a specific charactersitic.

```bash
register device_id service_id characteristic_id
```
