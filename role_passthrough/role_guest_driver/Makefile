KERNEL_DIR=/root/NF_card_hw_sw_dev/software/arm-linux/virt/
all:
	        make -C $(KERNEL_DIR) M=`pwd` modules
clean:
	        rm -rf *.o *.cmd *.order Module.symvers *.ko *.mod* ./.* .tmp_versions
obj-m += role.o

