#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/notifier.h>
#include <linux/timer.h>

BLOCKING_NOTIFIER_HEAD(my_notifier);

EXPORT_SYMBOL(my_notifier);

unsigned int timeout = 10000;

static struct timer_list my_timer;

static void timer_fn(struct timer_list *t)
{
    printk("call my_notifier's\n");

    blocking_notifier_call_chain(&my_notifier, 0, NULL);

    mod_timer(t, jiffies + (timeout * HZ / 1000));
}


int init_module(void)
{
    timer_setup(&my_timer, timer_fn, 0);

    my_timer.expires = jiffies + (timeout * HZ / 1000);

    add_timer(&my_timer);

    printk("Hello World!\n");

    return 0;
}

void cleanup_module(void)
{
    printk("Goodbye World!\n");
}

MODULE_LICENSE("GPL");
