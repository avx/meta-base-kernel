#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>                          /* reference */

static int param1 = 0;

static int set_param_str(const char *val, const struct kernel_param *kp)
{
#if 0
    int * pvalue = (int *)kp->arg;

    /* Read new value */
    sscanf(val,"%d", pvalue);

    /* pvalue is the pointer to param1 */
    printk("___ new param1 = %d\n", param1);

    return 0;
#else
    int * pvalue = kp->arg;

    int res = param_set_int(val, kp); /*  Use helper for write variable */
    if(res == 0)
    {
        /* Here you may execute additional actions */
        printk(KERN_INFO "set param %d\n", *pvalue);
    }
    return res;
#endif
}

static int get_param_str(char *buffer, const struct kernel_param *kp)
{
    int * p = (int *)kp->arg;

    return sprintf(buffer,"param1=%d\n", *p);
}

static struct kernel_param_ops param_ops_str = {
    .set = set_param_str,
    .get = get_param_str,
//    .get = param_get_int,
};

//module_param_cb(param1, &param_ops_str, NULL, 0644);
module_param_cb(param1, &param_ops_str, &param1, 0644);
MODULE_PARM_DESC(param1, "param1 (int, rw)");

/* module code */
static int __init params_init(void)
{
    printk("module = \"%s\"\n", THIS_MODULE->name);
    return 0;
}

static void __exit params_exit(void)
{
    /* cleanup actions */
}

module_init(params_init);
module_exit(params_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("example of set/get callback for module's parameter");
MODULE_INFO(name, KBUILD_MODNAME);
