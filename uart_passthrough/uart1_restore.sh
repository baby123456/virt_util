echo 81001000.serial > /sys/bus/platform/drivers/vfio-platform/unbind
echo uartlite > /sys/bus/platform/devices/81001000.serial/driver_override
echo 81001000.serial > /sys/bus/platform/drivers_probe
