echo 1 > /sys/module/vfio/parameters/enable_unsafe_noiommu_mode
echo 82000000.role > /sys/bus/platform/drivers/role_driver/unbind
echo vfio-platform > /sys/bus/platform/devices/82000000.role/driver_override
echo 82000000.role > /sys/bus/platform/drivers_probe
