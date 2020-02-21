#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/notifier.h>

extern struct blocking_notifier_head my_notifier;

int my_not3_call(struct notifier_block *, unsigned long, void *);

struct notifier_block my_not3 = {
    .notifier_call = my_not3_call,
};

int init_module(void)
{
    printk("Hello World from not3!\n");

    blocking_notifier_chain_register(&my_notifier, &my_not3);

    return 0;
}

void cleanup_module(void)
{
    blocking_notifier_chain_unregister(&my_notifier, &my_not3);

    printk("Goodbye World!\n");
}

int my_not3_call(struct notifier_block *nb, unsigned long l, void *buf) {
    printk("%s\n",__func__);
    return 0;
}

MODULE_LICENSE("GPL");
