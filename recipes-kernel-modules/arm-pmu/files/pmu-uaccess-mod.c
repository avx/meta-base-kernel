#include <linux/module.h>
#include <linux/printk.h>
#include <linux/smp.h>

static void _pmu_user_access(void *arg)
{
    long en = (long)arg;

    printk("[CPU#%d] %sable user-mode access to the performance counter\n", \
            smp_processor_id(), en ? "En" : "Dis");

    /* this for cortex-a9 */
    asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(en));
}

static int __init pmod_init(void)
{
    on_each_cpu(&_pmu_user_access, (void *)/* on */1, false);
    return 0;
}

static void __exit pmod_exit(void)
{
    on_each_cpu(&_pmu_user_access, (void *)/* off */0, false);
}

module_init(pmod_init);
module_exit(pmod_exit);

MODULE_LICENSE("GPL v2");
