echo 0 > /sys/class/fpga_bridge/br$1/set
echo 1 > /sys/class/fpga_manager/fpga0/flags
echo role_$1.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo 1 > /sys/class/fpga_bridge/br$1/set
