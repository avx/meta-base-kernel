#include <linux/module.h>

int init_module(void)
{
    printk("Hello World %p!\n", init_module);
    return 0;
}

void cleanup_module(void)
{
    printk("Goodbye World!\n");
}

MODULE_LICENSE("GPL");
