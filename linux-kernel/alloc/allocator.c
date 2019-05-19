#include <linux/init.h>          
#include <linux/module.h>        
#include <linux/device.h>        
#include <linux/kernel.h>        
#include <linux/fs.h>            
#include <linux/uaccess.h>   
#include <linux/slab.h>

#define  DEVICE_NAME "cha"   
#define  CLASS_NAME  "ch_class"    
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ilya Kompaniets");
MODULE_DESCRIPTION("Allocator with char device");
MODULE_VERSION("0.1");
 
static int majorNumber;
static struct class* myClass = NULL;
static struct device* myCharDevice = NULL;
 
static int dev_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "allocator: chardev open\n");

    return 0;
}
 
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    void *ptr = NULL;

    printk(KERN_INFO "allocator: chardev read %zuB\n", len);
    printk(KERN_INFO "allocator: kmalloc(%zuB, GFP_KERNEL)\n", len);

    ptr = kmalloc(len, GFP_KERNEL);
    printk(KERN_INFO "allocator: ptr = 0x%p\n", ptr);
    
    if (ptr) {
        kfree(ptr);
    }
    return len;
}

static loff_t cha_llseek(struct file *flip, loff_t count, int whence)
{
    void *ptr = NULL;

    printk(KERN_INFO "allocator: chardev llseek %llu\n", count);
    printk(KERN_INFO "allocator: vmalloc(%llu)\n", count);

    ptr = vmalloc(count);
    printk(KERN_INFO "allocator: ptr = 0x%p\n", ptr);
    
    if (ptr) {
        vfree(ptr);
    }

    return count;
}
 
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    void *ptr = NULL;
        
    printk(KERN_INFO "allocator: chardev write %zuB\n", len);
    printk(KERN_INFO "allocator: kmalloc(%zuB, GFP_ATOMIC)\n", len);

    ptr = kmalloc(len, GFP_ATOMIC);
    printk(KERN_INFO "allocator: ptr = 0x%p\n", ptr);

    if (ptr) {
        kfree(ptr);
    }
    return len;
}
 
static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "allocator: chardev release\n");

    return 0;
}
 
static struct file_operations fops = {
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .llseek = cha_llseek,
   .release = dev_release,
};
 
static int __init allocator_init(void)
{
    printk(KERN_INFO "allocator: init\n");

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
       printk(KERN_ALERT "allocator: failed to register a major number\n");
    
       return -1;
    }
    printk(KERN_INFO "allocator: registered correctly with major number %d\n", majorNumber);
    
    myClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(myClass)) {
        printk(KERN_ALERT "allocator: failed to register device class\n");
        
        goto out_chrdev;
    }
    printk(KERN_INFO "allocator: device class registered correctly\n");
    
    myCharDevice = device_create(myClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(myCharDevice)) {
        printk(KERN_ALERT "allocator: failed to create the device\n");
    
        goto out_class;
    }
    printk(KERN_INFO "allocator: device class created correctly\n");
    
    return 0;
    
out_class:
    class_destroy(myClass);
out_chrdev:
    unregister_chrdev(majorNumber, DEVICE_NAME);
    
    return -1;
}
 
static void __exit allocator_exit(void)
{
    device_destroy(myClass, MKDEV(majorNumber, 0));
    class_unregister(myClass);
    class_destroy(myClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "allocator: exit\n");
}
 
module_init(allocator_init);
module_exit(allocator_exit);
