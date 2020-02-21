#include <linux/module.h>

int kmain(void)
{
    printk("%s():%d doint something serious\n", __func__, __LINE__);
    return 0;
}
