#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/desc.h>

#define DRIVER_AUTHOR "Robert Marciniak <robmarci@student.pg.edu.pl>"
#define DRIVER_DESC   "A module to display contents of GDT"
MODULE_LICENSE("GPL");

#define store_gdt(ptr) asm volatile("sgdt %0":"=m" (*ptr));

static int __init placeholderName_init(void)
{
	struct desc_ptr gtd_ptr; // gdt base addr and size
	struct desc_struct *gdt; // gdt entry object
	int i;

	store_gdt(&gtd_ptr); // get gdt base and size

	printk("===========\nGDT size %d\n=============", gtd_ptr.size);
	printk("===========\nGDT address %lx\n==========", gtd_ptr.address);

	gdt = (struct desc_struct *)gtd_ptr.address;

	for (i = 0; i < gtd_ptr.size; i++) {
		printk("-----------------------------------------\n");
		printk("%i\n",gdt->limit0);
		printk("%i\n",gdt->base0);
		printk("%i\n",gdt->base1);
		printk("%i\n",gdt->type);
		printk("%i\n",gdt->s);
		printk("%i\n",gdt->dpl);
		printk("%i\n",gdt->p);
		printk("%i\n",gdt->limit1);
		printk("%i\n",gdt->avl);
		printk("%i\n",gdt->l);
		printk("%i\n",gdt->d);
		printk("%i\n",gdt->g);
		printk("%i\n",gdt->base2);
		printk("-----------------------------------------\n");
		gdt++;
	}
return 0;
}

/* 8 byte segment descriptor */
// struct desc_struct {
// 	u16	limit0;
// 	u16	base0;
// 	u16	base1: 8, type: 4, s: 1, dpl: 2, p: 1;
// 	u16	limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
// } __attribute__((packed));


static void __exit placeholderName_exit(void)
{
	
}

module_init(placeholderName_init);
module_exit(placeholderName_exit);