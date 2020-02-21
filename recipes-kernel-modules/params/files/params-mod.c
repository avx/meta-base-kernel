#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>                          /* reference */

/* debug macros */
#define DPRINTK(fmt, args...) do { if (debug) printk("[%s] %s(%d): " fmt "\n", KBUILD_MODNAME, __func__ , __LINE__,  ## args); } while(0)

/* params */

static int debug = 0;
module_param(debug, int, S_IRUGO | S_IWUSR);            // can be changed in /sys/modules/params-mod/parameters/debug
module_param_named(dbg, debug, int, S_IRUGO);           // name of parameter and variable are different ... just an alias to debug (ro in sysfs)
MODULE_PARM_DESC(debug, "enable debug (int, rw)");
MODULE_PARM_DESC(dbg,   "enable debug (int, ro)");


static int hidden_param = 0;
module_param(hidden_param, int, 0);                     // invisible in /sys/modules/params-mod/parameters
MODULE_PARM_DESC(hidden_param, "hidden for sysfs parameter (int, )");


static char *motd = "hello, world!";
module_param(motd, charp, S_IRUGO);                     // can not be changed in /sys/modules/params-mod/parameters/motd
MODULE_PARM_DESC(motd, "motd string (charp, ro)");


static bool bflag = true;
module_param(bflag, invbool, 0);                        // ... types: bool, charp, int, long, short, uint, ulong, ushort
MODULE_PARM_DESC(bflag, "inverse boolean (invisible)");


static char str[256];
module_param_string(string, str, sizeof(str), S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(string, "changable strings");


static int arr[8];
static int arr_size = sizeof(arr)/sizeof(arr[0]);
module_param_array(arr, int, &arr_size, S_IRUGO | S_IWUSR );
MODULE_PARM_DESC(arr, "array (8 ints, rw)");


/* helpers for hex */
int param_set_hex(const char *val, const struct kernel_param *kp)
{
    return kstrtoul(val, 16, (unsigned long*)kp->arg);
}

int param_get_hex(char *buffer, const struct kernel_param *kp)
{
    return scnprintf(buffer, PAGE_SIZE, "%lx", *((unsigned long*)kp->arg));
}

const struct kernel_param_ops param_ops_hex = {
    .set = param_set_hex,
    .get = param_get_hex
};

#define param_check_hex(name, p) param_check_ulong(name, p)

static unsigned long hex_var;
module_param(hex_var, hex, 0644);
MODULE_PARM_DESC(hwb_var, "hexadecimal variable (rw)");


/* module code */

static void show_params(void)
{
    int i;

    DPRINTK("started");

    printk("+ debug = %d\n", debug);
    printk("+ hex_var = 0x%lx\n", hex_var);
    printk("+ motd = \"%s\"\n", motd);
    printk("+ str = \"%s\"\n", str);
    printk("+ bflag = %d\n", bflag);
    printk("+ hidden_param = %d\n", hidden_param);

    for (i=0;i<arr_size;i++)
        printk("+ arr[%d] = %d\n", i, arr[i]);

    DPRINTK("finished");
}

static int __init params_init(void)
{
    printk("module = \"%s\"\n", THIS_MODULE->name);
    show_params();
    return 0;
}

static void __exit params_exit(void)
{
    show_params();
}

module_init(params_init);
module_exit(params_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("example of passing parameters to module");
