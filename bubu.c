#include <asm/desc.h>
#include <asm/io.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

// Info
#define DRIVER_AUTHOR "Robert Marciniak <robmarci@student.pg.edu.pl>"
#define DRIVER_DESC   "Character driver that displays something"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert Marciniak");
MODULE_DESCRIPTION("Character driver that displays something");
MODULE_VERSION("0.1");



// User defines
#define DEVICE_NAME "bubu_char"
#define CLASS_NAME "bubu_class"
#define MESSAGE_BUFFER_LENGTH 256

static struct class* bubuClass = NULL;
static struct device* bubuDevice = NULL;
static int deviceMajorNumber;
static char messageBuffer[MESSAGE_BUFFER_LENGTH];


// Mutex
static DEFINE_MUTEX(bubu_mutex);

// Prototypes for contracts
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static void clear_message_buffer(void);
static unsigned long pid_to_cr3(int);

static struct file_operations file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int __init bubu_init(void)
{   
    mutex_init(&bubu_mutex);
    printk(KERN_INFO "[init] bubu_init\n");
    clear_message_buffer();    


    deviceMajorNumber= register_chrdev(0, DEVICE_NAME, &file_ops);

    if(deviceMajorNumber < 0) {
        printk(KERN_WARNING "[init] alloc_chrdev_region failed\n");
        return deviceMajorNumber;
    }
    else {
        printk(KERN_INFO "[init] Registered device with major number %d\n",deviceMajorNumber);
    }

    bubuClass = class_create(THIS_MODULE, CLASS_NAME);

    if (IS_ERR(bubuClass)) {
        unregister_chrdev(deviceMajorNumber,DEVICE_NAME);
        printk(KERN_ALERT "[init] Failed to register device class\n");
        return PTR_ERR(bubuClass);
    }
    else {
        printk(KERN_INFO "[init] Correctly registered device class\n");
    }

    bubuDevice = device_create(bubuClass, NULL, MKDEV(deviceMajorNumber,0), NULL, DEVICE_NAME);
    if(IS_ERR(bubuDevice)) {
        class_destroy(bubuClass);
        unregister_chrdev(deviceMajorNumber,DEVICE_NAME);
        printk(KERN_ALERT "[init] Failed to create device\n");
        return PTR_ERR(bubuDevice);
    }
    else {
        printk(KERN_INFO "[init] Device created\n");   
    }
    return 0;
    // struct desc_ptr gtd_ptr; // gdt base addr and size
    // struct desc_struct *gdt; // gdt entry object
    // int i;

    // store_gdt(&gtd_ptr); // get gdt base and size

    // printk("===========\nGDT size %d\n=============", gtd_ptr.size);
    // printk("===========\nGDT address %x\n==========", gtd_ptr.address);

    // gdt = (struct desc_struct *)gtd_ptr.address;

    // for (i = 0; i < gtd_ptr.size; i++) {
    //     printk("-----------------------------------------\n");
    //     printk("%i\n",gdt_entry->limit0);
    //     printk("%i\n",gdt_entry->base0);
    //     printk("%i\n",gdt_entry->base1);
    //     printk("%i\n",gdt_entry->type);
    //     printk("%i\n",gdt_entry->s);
    //     printk("%i\n",gdt_entry->dpl);
    //     printk("%i\n",gdt_entry->p);
    //     printk("%i\n",gdt_entry->limit1);
    //     printk("%i\n",gdt_entry->avl);
    //     printk("%i\n",gdt_entry->l);
    //     printk("%i\n",gdt_entry->d);
    //     printk("%i\n",gdt_entry->g);
    //     printk("%i\n",gdt_entry->base2);
    //     printk("-----------------------------------------\n");
    //     gdt++;
    // }
    return 0;
}

/* 8 byte segment descriptor */
// struct desc_struct {
// 	u16	limit0;
// 	u16	base0;
// 	u16	base1: 8, type: 4, s: 1, dpl: 2, p: 1;
// 	u16	limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
// } __attribute__((packed));


static void __exit bubu_exit(void)
{
    printk(KERN_INFO "[exit] bubu_exit\n");
    printk(KERN_INFO "[exit] destroying device\n");
    device_destroy(bubuClass, MKDEV(deviceMajorNumber, 0));
    printk(KERN_INFO "[exit] unregistering class\n");
    class_unregister(bubuClass);
    printk(KERN_INFO "[exit] destroying class\n");
    class_destroy(bubuClass);
    printk(KERN_INFO "[exit] unregistering character device\n");
    unregister_chrdev(deviceMajorNumber, DEVICE_NAME);
    printk(KERN_INFO "[exit] unregistered class and device\n");
    mutex_destroy(&bubu_mutex);
}

static int device_open(struct inode *inode_ptr, struct file *file_ptr){
    if(!mutex_trylock(&bubu_mutex)){
        printk(KERN_ALERT "[device_open] "DEVICE_NAME" is busy");
        return -EBUSY;
    }
    return 0;
}

static int device_release(struct inode *inode_ptr, struct file *file_ptr){
    mutex_unlock(&bubu_mutex);
    return 0;
}

static ssize_t device_read(struct file *file_ptr, char *buffer, size_t length, loff_t *offset) {
    int errorCount = 0;
    errorCount = copy_to_user(buffer,messageBuffer,length);
    if(errorCount==0) {
        printk(KERN_INFO "[device_read] copied %pE buffer back to user",messageBuffer);
        return 0;
    }
    else {
        printk(KERN_NOTICE "[device_read] Failed to send %d characters to user\n",errorCount);
        return -EFAULT;
    }
}

static ssize_t device_write(struct file *file_ptr, const char *buffer, size_t length, loff_t *offset){
    long pid;
    int returnValue;
    
    clear_message_buffer();
    returnValue = kstrtol(buffer, 0, &pid);
    if(returnValue ==0)
        sprintf(messageBuffer,"%lu",pid_to_cr3(pid));
    else {
        printk(KERN_NOTICE "[device_write] Failed to parse input\n");
        return returnValue;
    }
    return 0;
}

static unsigned long pid_to_cr3(int pid)
{
    struct task_struct *task;
    struct mm_struct *mm;
    void *cr3_virt;
    unsigned long cr3_phys;

    task = pid_task(find_vpid(pid), PIDTYPE_PID);

    if (task == NULL)
        return 0; // pid has no task_struct

    mm = task->mm;

    // mm can be NULL in some rare cases (e.g. kthreads)
    // when this happens, we should check active_mm
    if (mm == NULL) {
        mm = task->active_mm;
    }

    if (mm == NULL)
        return 0;

    cr3_virt = (void *) mm->pgd;
    cr3_phys = virt_to_phys(cr3_virt);

    return cr3_phys;
}

static void clear_message_buffer() {
    int i;
    for(i=0;i<MESSAGE_BUFFER_LENGTH;i++){
        messageBuffer[i] =0;
    }
}

module_init(bubu_init);
module_exit(bubu_exit);
