#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static int timeout = 5;
module_param(timeout, int, 0644);

static struct task_struct *task;

static int thread_main(void * data)
{
    printk(KERN_INFO "thread: child process [%d] is running\n", current->pid);

    while(1)
    {
        set_current_state(TASK_UNINTERRUPTIBLE);

        if (kthread_should_stop())
        {
            printk(KERN_INFO "thread: child process [%d] should stop!\n", current->pid);
            break;
        }

        printk(KERN_INFO "thread: child process [%d] is still running\n", current->pid);

        schedule_timeout(HZ*timeout);
    }

    printk(KERN_INFO "thread: child process [%d] is completed\n", current->pid);

    return 0;
}

int mod_init(void)
{
    int err;
//    pid_t pid;

    printk(KERN_INFO "thread: main process [%d] is running\n", current->pid);

//    pid = kernel_thread(thread_main, NULL, CLONE_FS);
    task = kthread_create(thread_main, NULL, "kthread-mod");
    if(IS_ERR(task))
    {
        printk("Unable to start kernel thread.\n");
        err = PTR_ERR(task);
        task = NULL;
        return err;
    }

    printk(KERN_INFO "running kthread, pid = %d\n", task->pid);
    wake_up_process(task);

    return 0;
}

void mod_exit(void)
{
    if (task)
    {
        kthread_stop(task);
        task = NULL;
    }
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
