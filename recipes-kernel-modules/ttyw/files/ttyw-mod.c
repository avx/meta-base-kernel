#include <linux/module.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/sched.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,10,0)
#include <linux/sched/signal.h>
#endif

static void tty_console_puts(char *msg)
{
    struct tty_struct *tty;

    tty = current->signal->tty;
    if (tty == NULL)
        return;

    ((tty->driver->ops)->write)(tty, msg, strlen(msg));
    ((tty->driver->ops)->write)(tty, "\r", 1);
}

static int ttyw_init(void)
{
    printk("ttyw: this messages is written by kernel's printk\n");

    tty_console_puts("ttyw: This message is written by ttyw module via tty\n");

    return 0;
}

static void ttyw_exit(void)
{
    printk("ttyw: module unloaded\n");
}

module_init(ttyw_init);
module_exit(ttyw_exit);

MODULE_LICENSE("GPL");
