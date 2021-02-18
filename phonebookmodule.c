#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>

// create a structure for our fake device

struct fake_device {
	char data[100];
	struct semaphore sem;

} virtual_device;

// to later register our device we need a cdev object ans some other variables
struct cdev *mcdev;
int major_number;
int ret;

dev_t dev_num;

#define DEVICE_NAME "phonebookdevice"


//step(7)
// called on device file open inode reference to the file on disk
int device_open(struct inode *inode, struct file *filp) {
	//only allow oe process to open device by using a semaphore as mutual exclusive lock- mutex
	if(down_interruptible(&virtual_device.sem) != 0) {
		printk(KERN_ALERT "code: could now lock device during open");
		return -1;
	}

	printk(KERN_INFO "code: opened device");
	return 0;
}

// step(8) called when user wants to get information from the device
ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* cur0ffset) {
	printk(KERN_INFO "code: reading from device");
	ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
	return ret;
}

//step(9) called when user wants to send information to the device
ssize_t device_write(struct file* filp, const char* bufSourceData, size_t bufCount, loff_t* cur0offset){
	//send data from user to kernel
	// copy from user (dest, source, count)

	printk(KERN_INFO "code: writing to device");
	ret = copy_from_user(virtual_device.data, bufSourceData, bufCount);
	return ret;
}

//ste(10)
int device_close(struct inode *inode, struct file *filp) {
	// by calling up, which is opposite of down
	up(&virtual_device.sem);
	printk(KERN_INFO "code: close device");
	return 0;
}
//HERE
//step(6) tell the kernel which functions to call when user operates on our device file
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,
	.write = device_write,
	.read = device_read
};

static int driver_entry(void) {
	//register our device with the system
	//step(1) use dynamic allocation to assign out device
	ret =  alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if(ret < 0) {
		printk(KERN_ALERT "code failed");
		return ret;
	}
	major_number = MAJOR(dev_num);
	printk(KERN_INFO "code: major number is %d", major_number);
	printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME, major_number);
	//step(2)
	mcdev = cdev_alloc(); // create our cdev structure, initialized our cdev
	mcdev->ops = &fops;
	mcdev->owner = THIS_MODULE;
	//now that we created cdev we have to add it to the kernel
	//int cdev_add
	ret = cdev_add(mcdev, dev_num, 1);
	if(ret < 0) {
		printk(KERN_ALERT "code: unable to add cdev to kernel");
		return ret;
	}

	sema_init(&virtual_device.sem,1); //initial value of one
	return 0;
}


static void driver_exit(void) {
	//step(5)
	cdev_del(mcdev);
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "code: unloaded module");
}

// inform the kernel where to start and stop with out module/driver
module_init(driver_entry);
module_exit(driver_exit);

