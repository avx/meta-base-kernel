#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define SHARED_IRQ      1

static int irq = SHARED_IRQ;
module_param(irq, int, S_IRUGO);

unsigned int my_irq_counter = 0;

static irqreturn_t my_isr(int irq, void *dev_id)
{
    my_irq_counter++;
    if (printk_ratelimit()) {
        printk(KERN_INFO "%s(%d, %p): in_atomic=%d, in_irq=%ld, in_softirq=%ld, in_interrupt=%ld: counter = %d\n", \
                        __func__, irq, dev_id, in_atomic(), in_irq(), in_softirq(), in_interrupt(), my_irq_counter);
    }
    return IRQ_NONE;
}

static int my_dev_id;

int __init irq_mod_init(void)
{
    if(request_irq(irq, my_isr, IRQF_SHARED, "my_isr", &my_dev_id))
        return -1;

    printk(KERN_INFO "register ISR handler for IRQ %d, in_atomic=%d\n", irq, in_atomic());

    return 0;
}

static void __exit irq_mod_exit(void)
{
   synchronize_irq(irq);
   free_irq(irq, &my_dev_id);
   printk(KERN_INFO "ISR for %d IRQ is unloaded, irq_counter = %d, in_atomic=%d\n", irq, my_irq_counter, in_atomic());
}

module_init(irq_mod_init);
module_exit(irq_mod_exit);

MODULE_LICENSE("GPL");
