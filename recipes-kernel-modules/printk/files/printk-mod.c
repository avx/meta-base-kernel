
/* enable static debug */
// #define DEBUG

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ratelimit.h>

/* my debug macro */
#define DPRINTK(fmt, args...) \
    do { printk("[%s] %s(%d): " fmt "\n", KBUILD_MODNAME, __func__ , __LINE__,  ## args); } while(0)

static int __init mod_init(void)
{
    int i;

    /*
         $ cat /proc/sys/kernel/printk
            7       4       1       7
            current     default minimum boot-time-default
    */

    printk("\n=============================\n");

    DPRINTK("my debug macro");

    printk("\n=============================\n");

    printk(             "printk(\"some message ...");
    printk(KERN_CONT    " (KERN_CONT ...\n");

    printk(KERN_DEFAULT "printk(KERN_DEFAULT ...\n");
    printk(KERN_EMERG   "printk(KERN_EMERG ...\n");
    printk(KERN_ALERT   "printk(KERN_ALERT ...\n");
    printk(KERN_CRIT    "printk(KERN_CRIT ...\n");
    printk(KERN_ERR     "printk(KERN_ERR ...\n");
    printk(KERN_WARNING "printk(KERN_WARNING ...\n");
    printk(KERN_NOTICE  "printk(KERN_NOTICE ...\n");
    printk(KERN_INFO    "printk(KERN_INFO ...\n");
    printk(KERN_DEBUG   "printk(KERN_DEBUG ...\n");

    printk("\n=============================\n");

    pr_emerg("pr_emerg\n");
    pr_alert("pr_alert\n");
    pr_crit("pr_crit\n");
    pr_err("pr_err\n");
    pr_warning("pr_warning\n");
    pr_info("pr_info ...(no \\n)");
    pr_cont("pr_cont\n");

    printk("\n=============================\n");

    /* Options:
     * - CONFIG_DYNAMIC_DEBUG     : dynamic debug
     * - #define DEBUG or -DDEBUG : static debug enabled
     * - else no output */

    pr_debug("pr_debug\n");
    pr_devel("pr_devel\n");

    printk("\n=============================\n");

    printk("ratelimited example (linux/ratelimit.h is include file):\n\n");

    for (i=0;i<100000;i++)
        printk_ratelimited("flood message\n");

    printk("\n=============================\n");

    printk("dump stack example:\n\n");

    dump_stack();

    printk("\n=============================\n");

    printk("WARN macro:\n");

    WARN(/* condition = */ 1, "some warn message");

    printk("\n=============================\n");
    printk("WARN_ON:\n");

    WARN_ON(1);

    printk("\n=============================\n");

    BUG_ON(0);
//    BUG(...);

    return 0;
}

void mod_exit(void)
{
    printk("\n=============================\n");

    pr_debug("pr_debug\n");
    pr_devel("pr_devel\n");

    printk("\n=============================\n");

    WARN_ON(1);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("debug means");
