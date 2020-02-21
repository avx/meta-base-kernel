#include <linux/module.h>
#include <linux/version.h>

int kmain(void);

static int __init hello_init(void)
{
#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,4,0)
    printk("You have old kernel!\n\n");
#else
    printk("New kernel! Nice!\n\n");
#endif

    printk("Hello World from %s!\n", KBUILD_MODNAME);
    printk(" name = %s\n", THIS_MODULE->name);
    printk(" version = %s\n", THIS_MODULE->version);
    printk(" srcversion = %s\n", THIS_MODULE->srcversion);

    return kmain();

    /* return NON-ZERO if you wanna just a run hello_init function
       in kernel space without loading module */
#if 1
    return -1;
#else
    return 0;
#endif
}

static void __exit hello_exit(void)
{
    printk("Goodbye World!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("just a hello module");
MODULE_VERSION("2.1");
MODULE_ALIAS("hello-mod");
MODULE_INFO(whatever, "whatever");
