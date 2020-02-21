#include <linux/module.h>

/* exported by master-mod symbols */
extern int master_fn(int);
extern int master_var;

static int __init mod_init(void)
{
    int ret;

    printk("Hello World from %s!\n", KBUILD_MODNAME);

    master_var = 55;

    ret = master_fn(__LINE__);

    printk("master_fn() returned %d\n", ret);

    return 0;
}

static void __exit mod_exit(void)
{
    printk("Goodbye World from %s!\n", KBUILD_MODNAME);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("slave module uses exported function from the master module");
