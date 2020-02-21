#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>

static int __init mod_init(void)
{
    printk("[%s] module is loaded, jiffies=%lu\n", KBUILD_MODNAME, jiffies);

    ssleep(30); /* sleep for 30 seconds. */

    printk("[%s] jiffies=%lu HZ=%d\n", KBUILD_MODNAME, jiffies, HZ);

    return 0;
}

static void __exit mod_exit(void)
{
    printk("[%s] module is unloaded\n", KBUILD_MODNAME);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
