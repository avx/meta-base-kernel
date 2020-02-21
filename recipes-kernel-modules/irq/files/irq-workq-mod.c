#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

/*
 * Create/destroy workqueue API:
 * =============================
 *  struct workqueue_struct *create_workqueue( name );
 *  void destroy_workqueue( struct workqueue_struct * );
 *
 * Work initialization API:
 * ========================
 *  INIT_WORK( work, func );
 *  INIT_DELAYED_WORK( work, func );
 *  INIT_DELAYED_WORK_DEFERRABLE( work, func );
 *
 * Put the work into work queue API:
 * =================================
 *  int queue_work( struct workqueue_struct *wq, struct work_struct *work );
 *  int queue_work_on( int cpu, struct workqueue_struct *wq, struct work_struct *work );
 *  int queue_delayed_work( struct workqueue_struct *wq, struct delayed_work *dwork, unsigned long delay );
 *  int queue_delayed_work_on( int cpu, struct workqueue_struct *wq, struct delayed_work *dwork, unsigned long delay );
 *
 * Kernel-global work queue functions API:
 * =======================================
 *    int schedule_work( struct work_struct *work );
 *    int schedule_work_on( int cpu, struct work_struct *work );
 *    int scheduled_delayed_work( struct delayed_work *dwork, unsigned long delay );
 *    int scheduled_delayed_work_on( int cpu, struct delayed_work *dwork, unsigned long delay );
 *
 * Work flushing:
 * ==============
 *    int flush_work( struct work_struct *work );
 *    int flush_workqueue( struct workqueue_struct *wq );
 *
 * Kernel-global work queue flush:
 * ===============================
 *    void flush_scheduled_work( void );
 *
 * Work cancel:
 * ============
 *    int cancel_work_sync( struct work_struct *work );
 *    int cancel_delayed_work_sync( struct delayed_work *dwork );
 *
 * Test if work is pending:
 * ========================
 *    work_pending( work );
 *    delayed_work_pending( work );
 *
 */

#define SHARED_IRQ      1

static int irq = SHARED_IRQ;
module_param(irq, int, S_IRUGO);

unsigned int my_irq_counter = 0;

struct work_struct workq;

struct workqueue_struct * my_wq;

static int my_dev_id;

void my_workq_fn(struct work_struct *work)
{
    printk("%s(%p): in_irq=%ld, in_softirq=%ld, in_interrupt=%ld, in_atomic=%d, counter=%d, pid=%d\n",
                __func__, work, in_irq(), in_softirq(), in_interrupt(), in_atomic(), my_irq_counter, current->pid);
}

static irqreturn_t my_isr(int irq, void *dev_id)
{
    my_irq_counter++;
    if (printk_ratelimit()) {
        printk(KERN_INFO "%s(%d, %p): in_irq=%ld, in_softirq=%ld, in_interrupt=%ld, in_atomic=%d: counter=%d\n", \
                        __func__, irq, dev_id, in_irq(), in_softirq(), in_interrupt(), in_atomic(), my_irq_counter);


        schedule_work(&workq);          // kernel-global work queue

        queue_work(my_wq, &workq);      // put into my_wq queue
    }

    return IRQ_NONE;
}

int __init irq_mod_init(void)
{
    my_wq = create_workqueue("my_wq");  // create separate work queue

    INIT_WORK(&workq, my_workq_fn);     // work task with function

    if (request_irq(irq, my_isr, IRQF_SHARED, "my_isr", &my_dev_id)) {
        return -1;
    }

    printk(KERN_INFO "register ISR handler for IRQ %d\n", irq);

    return 0;
}

static void __exit irq_mod_exit(void)
{
    synchronize_irq(irq);
    free_irq(irq, &my_dev_id);
    printk(KERN_INFO "ISR for %d IRQ is unloaded, irq_counter = %d\n", irq, my_irq_counter);

    flush_scheduled_work();             // flush kernel-global workqueue

    flush_workqueue(my_wq);             // flush my_wq queue

    destroy_workqueue(my_wq);           // destroy my_wq queue

    printk(KERN_INFO "sheduled work is flushed\n");

}

module_init(irq_mod_init);
module_exit(irq_mod_exit);

MODULE_LICENSE("GPL");
