#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int hello_proc_show(struct seq_file *m, void *v) {
    int i;

    seq_printf(m, "HEADER\n=========================\n");

    for (i=0;i<100000;i++)
      seq_printf(m, "%d: Hello proc!\n",i);

    seq_printf(m, "=========================\nTotal: %d\n\n", i+1);

    return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file) {
    return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations hello_proc_fops = {
    .owner   = THIS_MODULE,
    .open    = hello_proc_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};

struct proc_dir_entry *proc_hello_dir = NULL;

static int __init hello_proc_init(void)
{
    proc_hello_dir = proc_mkdir("hello", NULL);
    proc_create("hello", 0, proc_hello_dir /* or NULL if in /proc */, &hello_proc_fops);
    printk("/proc/hello/hello is created\n");
    return 0;
}

static void __exit hello_proc_exit(void)
{
    remove_proc_entry("hello/hello", NULL);
    proc_remove(proc_hello_dir);
}

module_init(hello_proc_init);
module_exit(hello_proc_exit);

MODULE_LICENSE("GPL");
