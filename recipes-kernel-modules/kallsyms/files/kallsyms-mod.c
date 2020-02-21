#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/delay.h>

/* CONFIG_KALLSYMS = y (?) */

static void (*machine_power_off_p)(void);

int init_module(void)
{
    machine_power_off_p = (void*)kallsyms_lookup_name("machine_power_off");

    if (!machine_power_off_p)
        return -1;

    printk("%s: found machine_power_off() function -- %p\n" \
           "...... WILL POWER OFF IN 5 SECONDS ......\n", \
           KBUILD_MODNAME,  machine_power_off_p);

    ssleep(5);

    /* power off */
    (*machine_power_off_p)();

    return 0;
}

void cleanup_module(void)
{
    return;
}

MODULE_LICENSE("GPL");
