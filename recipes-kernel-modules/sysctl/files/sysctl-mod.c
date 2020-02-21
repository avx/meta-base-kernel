/*
 * /proc/sys/net/test/value1
 * /proc/sys/net/test/value2
 * /proc/sys/net/test/value3 - default value is 0, can be changed to 1 only once
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysctl.h>

static struct ctl_table_header *test_sysctl_header;

int value1 = 40;
EXPORT_SYMBOL(value1);

int value2 = 50;
EXPORT_SYMBOL(value2);

/* this value can be enable only */
int value3 = 0;
int value3_min=1;
int value3_max=1;
EXPORT_SYMBOL_GPL(value3);

int min = 0;
int max = 100;

/*
 * only values between min and max can be written to value1 and value2
 * respectively.
 */
static struct ctl_table test_table[] = {
    {
     .procname          = "value1",
     .data              = &value1,
     .maxlen            = sizeof(int),
     .mode              = 0644,
     .proc_handler      = &proc_dointvec_minmax,
     .extra1            = &min,
     .extra2            = &max
    },
    {
     .procname          = "value2",
     .data              = &value2,
     .maxlen            = sizeof(int),
     .mode              = 0644,
     .proc_handler      = &proc_dointvec_minmax,
     .extra1            = &min,
     .extra2            = &max
    },
    {
     .procname          = "only_enable",
     .data              = &value3,
     .maxlen            = sizeof(int),
     .mode              = 0644,
     .proc_handler      = &proc_dointvec_minmax,
     .extra1            = &value3_min,
     .extra2            = &value3_max
    },
    {
     .procname         = NULL,
     .data             = NULL,
    }
};

static struct ctl_table test_net_table[] = {
    {
     .procname          = "test",
     .mode              = 0555,
     .child             = test_table
    },
    {
     .procname         = NULL,
     .data             = NULL,
    }
};

static struct ctl_table test_root_table[] = {
    {
     .procname          = "net",
     .mode              = 0555,
     .child             = test_net_table
    },
    {
     .procname         = NULL,
     .data             = NULL,
    }

};

static int __init sysctl_module_init(void)
{
    test_sysctl_header = register_sysctl_table(test_root_table);
    return 0;
}

static void __exit sysctl_module_exit(void)
{
    unregister_sysctl_table(test_sysctl_header);
}

module_init(sysctl_module_init);
module_exit(sysctl_module_exit);
MODULE_LICENSE("GPL");
