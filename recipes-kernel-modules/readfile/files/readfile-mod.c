#include <linux/module.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>

MODULE_LICENSE("GPL");
MODULE_PARM_DESC(filename, "file to read");

static char filename[256] = "/none";
module_param_string(filename, filename, sizeof(filename), S_IRUGO);

int __init init_module(void) {
    struct file *f;
    void *buf;
    long res = 0;
    loff_t pos = 0;
    size_t n, len;

    printk("opening file: %s\n", filename);

    f = filp_open(filename, O_RDONLY, 0);
    if (IS_ERR(f)) {
        printk("file open error\n");
        res = -ENOENT;
        goto end;
    }

    len = vfs_llseek(f, 0L, 2); /* SEEK_END = 2 */
    if(len <= 0) {
        printk("vfs_llseek error\n");
        res = -EINVAL;
        goto close;
    }

    printk("file size is %ld\n", len);

    buf = vmalloc(len);
    if (buf == NULL) {
        printk("can not allocate memory\n");
        res = -ENOMEM;
        goto close;
    };

//    vfs_llseek(f, 0L, 0);       /* SEEK_SET = 0 */

    n = kernel_read(f, buf, len, &pos);
    printk("read from file %ld bytes\n", n);

    if(n != len) {
        printk("file read error\n");
        res = -EIO;
        goto free;
    }

free:
   vfree(buf);
close:
   filp_close(f, NULL);
end:
   return res;
}

void __exit cleanup_module(void)
{
    return;
}
