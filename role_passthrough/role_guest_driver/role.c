#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/ioctl.h>    
#include <linux/uaccess.h>
#include "role.h"

// device struct
struct role_dev{ 
    void __iomem *regs;
    void __iomem *mems;
	struct device *dev;
    struct platform_device *pdev;
    int irq; 
    struct tasklet_struct tasklet;
    //character device support 
    dev_t dev_node; 
    struct cdev cdev;
    struct class *class_p;
    struct device *cdev_device;
    char name[16];				
};

static int role_open(struct inode *inode, struct file *filp) { 
    printk(KERN_ALERT "role chrdev open!\n");  
    return 0;
}

static long role_ioctl(struct file *file, unsigned int cmd , unsigned long arg)
{
	int ret;
    printk(KERN_ALERT "role chrdev ioctl!\n");
    /*
    //check dev type
    if (_IOC_TYPE(cmd) != IOC_MAGIC) {
        pr_err("[%s] command type [%c] error!\n",__func__, _IOC_TYPE(cmd));
        return -ENOTTY; 
    }
     // check cmd num
    if (_IOC_NR(cmd) > IOC_MAXNR) { 
        pr_err("[%s] command numer [%d] exceeded!\n",__func__, _IOC_NR(cmd));
        return -ENOTTY;
    } 
    // check acess mode
    if (_IOC_DIR(cmd) & _IOC_READ)
        ret= !access_ok((void __user *)arg,_IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        ret= !access_ok((void __user *)arg,_IOC_SIZE(cmd));
    if (ret)
        return -EFAULT;   
    */
    switch(cmd){
		case ROLE_ON:
			break;
		case ROLE_OFF:
			break;
		case ROLE_FREQ:
			break;
		default :
			return -EINVAL;
	}  
	return 0;
}

static struct file_operations role_fops = {
    .owner = THIS_MODULE,
    .open = role_open,
    .unlocked_ioctl = role_ioctl,
};

static int role_probe(struct platform_device *pdev)
{
    struct resource *io_reg, *io_mem;
    //get device dtb node
	//struct device_node *node = pdev->dev.of_node;
    struct role_dev *rdev;
    int rc = 0; 
    static int i = 0;
    static struct class *local_class_p = NULL;

	rdev = devm_kzalloc(&pdev->dev, sizeof(*rdev), GFP_KERNEL);
    if (!rdev)
		return -ENOMEM;
    
    rdev->dev = &pdev->dev;

	/* Request and map I/O memory */
	io_reg = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    io_mem = platform_get_resource(pdev, IORESOURCE_MEM, 1);
    rdev->regs = devm_ioremap_resource(&pdev->dev, io_reg);
    if (IS_ERR(rdev->regs))
		return PTR_ERR(rdev->regs);
    rdev->mems = devm_ioremap_resource(&pdev->dev, io_mem);
    if (IS_ERR(rdev->mems))
		return PTR_ERR(rdev->mems);
    
    platform_set_drvdata(pdev, rdev);
    dev_info(&pdev->dev, "Xilinx role %d Driver Probed!!\n",i);

    // alloc character device number
    sprintf(rdev->name, "%s%d", CHRDEV_NAME, i);  
    rc = alloc_chrdev_region(&rdev->dev_node, 0, 1,rdev->name); 
    if (rc) {  
        dev_err(rdev->dev, "unable to get a char device number\n"); 
        return rc; 
    }  
    //register char device driver
    cdev_init(&rdev->cdev, &role_fops);
    rdev->cdev.owner = THIS_MODULE;

    // add a character device  
    rc = cdev_add(&rdev->cdev, rdev->dev_node, 1);  
    if (rc) {
		dev_err(rdev->dev, "unable to add char device\n");
		goto init_error1;
	}
    //only create once for the class
    if (!local_class_p) {
        // create the device class  
		local_class_p = class_create(THIS_MODULE, CHRDEV_NAME);

		if (IS_ERR(local_class_p)) {
			dev_err(rdev->dev, "unable to create class\n");
			rc = ERROR;
			goto init_error2;
		}
	}
    rdev->class_p = local_class_p;

	// Create the device node in /dev so the device is accessible as a character device
	rdev->cdev_device = device_create(rdev->class_p, NULL, rdev->dev_node, NULL, rdev->name);

	if (IS_ERR(rdev->cdev_device)) {
		dev_err(rdev->dev, "unable to create the device\n");
		goto init_error3;
	}
    ++i;
	return 0;

init_error3: 
    class_destroy(rdev->class_p);
init_error2:
    cdev_del(&rdev->cdev);  
init_error1:  
    unregister_chrdev_region(rdev->dev_node, 1);  
    return rc;  
}

static int role_remove(struct platform_device *pdev)
{
    static int role_num = 0;
    struct role_dev *rdev = platform_get_drvdata(pdev);

    printk(KERN_ALERT "role %d chrdev remove!\n",role_num); 

    // Take everything down in the reverse order from how it was created for the char device
	if (rdev->cdev_device) {
		device_destroy(rdev->class_p,rdev->dev_node);
        if(role_num == LAST_ROLE)
		    class_destroy(rdev->class_p);
		cdev_del(&rdev->cdev);
		unregister_chrdev_region(rdev->dev_node, 1);
	} 
    ++role_num;
    return 0;  
}

static const struct of_device_id role_of_ids[] = {
	{ .compatible = "axi-role-1.0"},
	{}
};

static struct platform_driver role_driver = {
	.driver = {
		.name = "role_driver",
		.owner = THIS_MODULE,
		.of_match_table = role_of_ids,
	},
	.probe = role_probe,
	.remove = role_remove,
};

module_platform_driver(role_driver);

MODULE_AUTHOR("ict, Inc.");
MODULE_DESCRIPTION("role driver");
MODULE_LICENSE("GPL v2");
