# BLE Serial Bridge [![](https://jitpack.io/v/cansik/ble-serial-bridge.svg)](https://jitpack.io/#cansik/ble-serial-bridge)
A firmware to use an ESP32 as a BLE device controlled over serial commands.


### Driver

Currently there is a Java driver and a [Python example](test/serial_test.py) script which encapsules the native serial interface.

```groovy
repositories {
	maven { url 'https://jitpack.io' }
}

dependencies {
        implementation 'com.github.cansik:ble-serial-bridge:-78138d765e-1'
}
```

### API

#### Scanning

Scan surrounding devices for a specific `service id`.

```
scan interval window scan_time service_id
```

#### Connecting & Disconnecting

Connect and disconnect to a device.

```
connect device_id
disconnect device_id
```

#### List Connections

List all active BLE connections.

```
list
```

#### Read and Write

Connect and disconnect to a device.

```
read device_id service_id characteristic_id [format]
write device_id service_id characteristic_id value [format]
```

#### Notifications (Beta)

It is possible to register for a notification of a specific charactersitic.

```
register device_id service_id characteristic_id
```
