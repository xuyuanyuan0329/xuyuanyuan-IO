#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static int major = 237;
static int minor = 0;
static dev_t devno;
static struct class *cls;
static struct device *dev;


static int hello_open (struct inode *inode, struct file *file){
	printk("hello_open\n");
	return 0;
}

static int hello_release (struct inode *inode, struct file *file){
	printk("hello_release\n");
	return 0;
}


static struct file_operations hello_ops = {
	.open = hello_open,
	.release = hello_release,
};

static int hello_init(void){

	int result;

	printk("hello_init\n");

	result = register_chrdev(major, "hello_dev", &hello_ops);
	if(result < 0){
		printk("register chrdev err\n");
		return result;
	}

	devno = MKDEV(major, minor);
	cls = class_create(THIS_MODULE, "hellocls");
	if(IS_ERR(cls)){
		printk("class create err\n");
		goto err_1;
	}
	dev = device_create(cls, NULL, devno, NULL, "hellodev");
	if(IS_ERR(dev)){
		printk("device create err\n");
		goto err_2;
	}


	
	return 0;
err_2:
	class_destroy(cls);
err_1:
	unregister_chrdev(major, "hello_dev");
	return -1;
	
}




static void hello_exit(void){
	printk("hello_exit\n");
	device_destroy(cls,devno);
	class_destroy(cls);
	unregister_chrdev(major, "hello_dev");
	return;
}




module_init(hello_init);
module_exit(hello_exit);






MODULE_LICENSE("GPL");
MODULE_AUTHOR("A");
