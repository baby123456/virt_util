echo 81000000.serial > /sys/bus/platform/drivers/vfio-platform/unbind
echo uartlite > /sys/bus/platform/devices/81000000.serial/driver_override
echo 81000000.serial > /sys/bus/platform/drivers_probe
