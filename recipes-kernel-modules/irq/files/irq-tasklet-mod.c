#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

/*
 *  Tasklet creation, enable/disable API:
 *  =====================================
 *    DECLARE_TASKLET( name, func, data );
 *    DECLARE_TASKLET_DISABLED( name, func, data);
 *    void tasklet_init( struct tasklet_struct *, void (*func)(unsigned long), unsigned long data );
 *    void tasklet_disable_nosync( struct tasklet_struct * );
 *    void tasklet_disable( struct tasklet_struct * );
 *    void tasklet_enable( struct tasklet_struct * );
 *    void tasklet_hi_enable( struct tasklet_struct * );
 *
 *  Tasklet sheduling API:
 *  ======================
 *    void tasklet_schedule( struct tasklet_struct * );
 *    void tasklet_hi_schedule( struct tasklet_struct * );
 *
 *  Tasklet kill API:
 *  =================
 *    void tasklet_kill( struct tasklet_struct * );
 *    void tasklet_kill_immediate( struct tasklet_struct *, unsigned int cpu );
 *
 */

#define SHARED_IRQ      1

static int irq = SHARED_IRQ;
module_param(irq, int, S_IRUGO);

unsigned int my_irq_counter = 0;

struct tasklet_struct tasklet;

static void my_tasklet(unsigned long arg)
{
    printk("%s(%lu): in_atomic=%d in_irq=%ld, in_softirq=%ld, in_interrupt=%ld, counter=%d\n",
                __func__, arg, in_atomic(), in_irq(), in_softirq(), in_interrupt(), my_irq_counter);
}

static irqreturn_t my_isr(int irq, void *dev_id)
{
    my_irq_counter++;
    if (printk_ratelimit()) {
        printk(KERN_INFO "%s(%d, %p): in_atomic=%d, in_irq=%ld, in_softirq=%ld, in_interrupt=%ld: counter=%d\n", \
                        __func__, irq, dev_id, in_atomic(), in_irq(), in_softirq(), in_interrupt(), my_irq_counter);

        tasklet_schedule(&tasklet);
    }

    return IRQ_NONE;
}

static int my_dev_id;

int __init irq_mod_init(void)
{
    tasklet_init(&tasklet, my_tasklet, 0);

    if (request_irq(irq, my_isr, IRQF_SHARED, "my_isr", &my_dev_id)) {
        tasklet_kill(&tasklet);
        return -1;
    }

    printk(KERN_INFO "register ISR handler for IRQ %d, in_atomic=%d\n", irq, in_atomic());

    return 0;
}

static void __exit irq_mod_exit(void)
{
    synchronize_irq(irq);
    free_irq(irq, &my_dev_id);
    printk(KERN_INFO "ISR for %d IRQ is unloaded, irq_counter = %d, in_atomic=%d\n", irq, my_irq_counter, in_atomic());

    tasklet_kill(&tasklet);
    printk(KERN_INFO "tasklet is stopped\n");
}

module_init(irq_mod_init);
module_exit(irq_mod_exit);

MODULE_LICENSE("GPL");
