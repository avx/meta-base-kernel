#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>

static int __init mod_init(void)
{
    int ret;

    printk("[%s] module is loaded, jiffies=%lu\n", KBUILD_MODNAME, jiffies);

    ret = msleep_interruptible(30 * 1000);      /* sleep for 30 seconds. */

    printk("[%s] jiffies=%lu HZ=%d: %s\n", KBUILD_MODNAME, jiffies, HZ, ret ? "Interrupted" : "Timeout");

    return 0;
}

static void __exit mod_exit(void)
{
    printk("[%s] module is unloaded\n", KBUILD_MODNAME);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
