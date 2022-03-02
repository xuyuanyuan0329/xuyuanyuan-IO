#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

MODULE_LICENSE("WTFPL");
MODULE_AUTHOR("Proc Dev");
MODULE_DESCRIPTION("A char driver");
MODULE_VERSION("0.1");

#define BASEMINOR 0

static struct proc_dir_entry* entry;
dev_t devno;
#define COUNT     3
#define NAME      "var5"
struct cdev *cdevp = NULL;
#define  PROCFS_MAX_SIZE   256
char proc_buffer[PROCFS_MAX_SIZE];
char g_buffer[PROCFS_MAX_SIZE*10];

int g_count = 0;
int g_flag = 0;

static ssize_t proc_write(struct file *file, const char __user * ubuf, size_t count, loff_t* ppos) 
{
	//printk(KERN_DEBUG "Attempt to write proc file");
	size_t proc_buffer_size = count;
	g_flag = 1;
	if(proc_buffer_size > PROCFS_MAX_SIZE)
	{
		proc_buffer_size = PROCFS_MAX_SIZE;
	}
	memset(proc_buffer, 0x0, PROCFS_MAX_SIZE);
	if(copy_from_user(proc_buffer, ubuf, proc_buffer_size))
	{
		printk(KERN_INFO "write1 : proc_buffer : [%s]\n", proc_buffer);
		return -EFAULT;
	}
	printk(KERN_INFO "write2  : proc_buffer : [%s]\n", proc_buffer);
	return proc_buffer_size;
}

static ssize_t proc_read(struct file *file, char __user * ubuf, size_t count, loff_t* ppos) 
{
	int len = strlen(proc_buffer);
	int i = 0;
	g_count = 0;
	
	if (*ppos > 0 || count < len)
	{
		return 0;
	}

	if (g_flag == 1)
	{
		for (i = 0; i < len; i++)
		{
			if (proc_buffer[i] >= 'a' && proc_buffer[i] <= 'z')
			{
				g_count++;
			}
		}

		sprintf(proc_buffer, "%d", g_count);
		if (strlen(g_buffer) != 0)
			strcat(g_buffer, " ");
		strcat(g_buffer, proc_buffer);

		memset(proc_buffer, 0x0, PROCFS_MAX_SIZE);
		strcpy(proc_buffer, g_buffer);
		g_flag = 0;
	}

	
	if (copy_to_user(ubuf, proc_buffer, strlen(proc_buffer)) != 0)
	{
		return -EFAULT;
	}
	*ppos = len;
	return len;
}

loff_t proc_llseek (struct file *filp,loff_t offset,int whence)
{
	loff_t newpos;
	
	switch(whence)
	{
		case 0:
			newpos = offset;
			break;
		case 1:
			newpos = filp->f_pos + offset;
			break;
		case 2:
			newpos = PROCFS_MAX_SIZE - 1 + offset;
			break;
		default:
			return -EINVAL;
	}
	
	if((newpos < 0) || (newpos > PROCFS_MAX_SIZE))
	{
		return -EINVAL;
	}
	
	filp->f_pos = newpos;
	
	return newpos;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = proc_read,
	.write = proc_write,
	.llseek = proc_llseek,
};
static struct proc_ops proc_fops = {
	.proc_read = proc_read,
	.proc_write = proc_write,
	.proc_lseek = proc_llseek,
};


static int __init proc_var5_init(void)
{	
	int ret = 0;
	entry = proc_create("var5", 0444, NULL, &proc_fops);
	printk(KERN_INFO "%s: proc file is created\n", THIS_MODULE->name);

	ret = alloc_chrdev_region(&devno,BASEMINOR,COUNT,NAME);
	if(ret < 0)
	{
		printk(KERN_ERR "alloc_chrdev_region failed...\n");
		goto err1;
	}
	cdevp = cdev_alloc();
	if(NULL == cdevp)
	{
		printk(KERN_ERR "cdev_alloc failed...\n");
		ret = -ENOMEM;
		goto err2;
	}
	cdev_init(cdevp,&fops);
	ret = cdev_add(cdevp,devno,COUNT);
	if(ret < 0)
	{
		printk(KERN_ERR "cdev_add failed...\n");
		goto err2;
	}

	printk(KERN_INFO "---%s---%s---%d---\n",__FILE__,__func__,__LINE__);
	return 0;
err2:
	unregister_chrdev_region(devno, COUNT);
err1:
	return ret;

	
	return 0;
}

static void __exit proc_var5_exit(void)
{
	proc_remove(entry);
	cdev_del(cdevp);
	unregister_chrdev_region(devno,COUNT);
	printk(KERN_INFO "%s: proc file is deleted\n", THIS_MODULE->name);
}

module_init(proc_var5_init);
module_exit(proc_var5_exit);
