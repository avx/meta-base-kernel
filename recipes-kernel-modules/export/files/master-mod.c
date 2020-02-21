#include <linux/module.h>

/* Kernel module not necessary must to have
   init and cleanup functions */

/* this symbol available only for GPL modules */
int master_var = 0;
EXPORT_SYMBOL_GPL(master_var);

/* this symbol available for all modules */
int master_fn(int arg)
{
    printk("%s:%s():%d arg=%d\n", KBUILD_MODNAME, __func__, __LINE__, arg);
    return __LINE__ * arg + master_var;
}
EXPORT_SYMBOL(master_fn);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("master module with exported function");
