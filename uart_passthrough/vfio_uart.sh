echo 1 > /sys/module/vfio/parameters/enable_unsafe_noiommu_mode
echo 81000000.serial > /sys/bus/platform/drivers/uartlite/unbind
echo vfio-platform > /sys/bus/platform/devices/81000000.serial/driver_override
echo 81000000.serial > /sys/bus/platform/drivers_probe
