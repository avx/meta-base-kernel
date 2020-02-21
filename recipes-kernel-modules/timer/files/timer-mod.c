#include <linux/module.h>
#include <linux/version.h>
#include <linux/timer.h>
#include <linux/moduleparam.h>                          /* reference */
#include <linux/delay.h>

/* module parameter : timeout */
static unsigned int timeout = 5000;
module_param(timeout, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "timer timeout in milliseconds");

/*
 *  struct timer_list {
 *      struct list_head entry;
 *      unsigned long expires;
 *      spinlock_t lock;
 *      void (*function)(unsigned long);
 *      unsigned long data;
 *      struct tvec_t_base_s * base;
 *  };
 *
 *  void init_timer (struct timer_list *timer);	// < 4.15
 *  void add_timer (struct timer_list *timer);
 *  void mod_timer (struct timer_list *timer, unsigned long expires);
 *  void del_timer (struct timer_list *timer);
 *  void del_timer_sync (struct timer_list *timer);
 *  int timer_pending(struct timer_list *timer);
 *
 */

static struct timer_list my_timer;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,15,0)
static void timer_fn(struct timer_list *t)
{
//	struct something *priv = from_timer(priv, t, timer_in_struct_something);
#else
static void timer_fn(unsigned long param)
{
	unsigned int * arg = (unsigned int *)param;
#endif
	if (printk_ratelimit()) {
		printk("%s(timeout=%u) is shot, in_atomic=%d, in_irq=%ld, in_softirq=%ld, in_interrupt=%ld!\n", \
			__func__, timeout, in_atomic(), in_irq(), in_softirq(), in_interrupt());
	}

	if (timeout)
		mod_timer(&my_timer, jiffies + (timeout * HZ / 1000));     /* prolong timer activity */
}

static int __init mod_init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,15,0)
	timer_setup(&my_timer, timer_fn, 0);
#else
	init_timer(&my_timer);
	my_timer.function = timer_fn;
	my_timer.data = &timeout;
#endif

	my_timer.expires = jiffies + (timeout * HZ / 1000);

	add_timer(&my_timer);

	printk("module is loaded and timer is started, timeout is %u!\n", timeout);

	return 0;
}

static void __exit mod_exit(void)
{
	if (!timer_pending(&my_timer))
		del_timer(&my_timer);
	else
		del_timer_sync(&my_timer);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("timers kernel module example");
