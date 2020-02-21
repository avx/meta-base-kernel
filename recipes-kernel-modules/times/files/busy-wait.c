#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>

static int __init mod_init(void)
{
    printk("[%s] module is loaded, jiffies=%lu\n", KBUILD_MODNAME, jiffies);

    /* busy-wait delays (safe for atomic context) */

    mdelay(1000);       /* 1000 milliseconds == 1 second      */
    udelay(1000);       /* 1000 microsecond  == 1 millisecond */
    ndelay(1000);       /* 1000 nanosecond   == 1 microsecond */

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
