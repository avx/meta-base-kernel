#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

/* see /sys/kernel/debug/hello/ after module inserting */

/*
 * see linux/debugfs.h and fs/debugfs/ *.c:
 * for instance, linux-5.5 gives next APIs:
 *
 * debugfs_create_file - create a file in the debugfs filesystem
 * debugfs_create_file_unsafe - create a file in the debugfs filesystem
 * debugfs_create_file_size - create a file in the debugfs filesystem
 * debugfs_create_dir - create a directory in the debugfs filesystem
 * debugfs_create_automount - create automount point in the debugfs filesystem
 * debugfs_create_symlink- create a symbolic link in the debugfs filesystem
 * debugfs_create_u8 - create a debugfs file that is used to read and write an unsigned 8-bit value
 * debugfs_create_u16 - create a debugfs file that is used to read and write an unsigned 16-bit value
 * debugfs_create_u32 - create a debugfs file that is used to read and write an unsigned 32-bit value
 * debugfs_create_u64 - create a debugfs file that is used to read and write an unsigned 64-bit value
 * debugfs_create_ulong - create a debugfs file that is used to read and write
 * debugfs_create_x{8,16,32,64} - create a debugfs file that is used to read and write an unsigned {8,16,32,64}-bit value
 * debugfs_create_x8 - create a debugfs file that is used to read and write an unsigned 8-bit value
 * debugfs_create_x16 - create a debugfs file that is used to read and write an unsigned 16-bit value
 * debugfs_create_x32 - create a debugfs file that is used to read and write an unsigned 32-bit value
 * debugfs_create_x64 - create a debugfs file that is used to read and write an unsigned 64-bit value
 * debugfs_create_size_t - create a debugfs file that is used to read and write an size_t value
 * debugfs_create_atomic_t - create a debugfs file that is used to read and
 * debugfs_create_bool - create a debugfs file that is used to read and write a boolean value
 * debugfs_create_blob - create a debugfs file that is used to read a binary blob
 * debugfs_create_u32_array - create a debugfs file that is used to read u32
 * debugfs_create_regset32 - create a debugfs file that returns register values
 * debugfs_create_devm_seqfile - create a debugfs file that is bound to device.
 */

static struct dentry * debugfs_dir;

static uint32_t var_u32 = 0;
static atomic_t var_atomic = ATOMIC_INIT(0);
static char var_string[] = "Hello world!\n";

#define FILE_SIZE	512
static char file_body[FILE_SIZE];

static struct debugfs_blob_wrapper var_string_blob_wrapper;

static int file_open(struct inode *inode, struct file *filp)
{
	filp->private_data = inode->i_private;
	return 0;
}

static ssize_t file_read(struct file *filp, char __user *buffer, size_t count, loff_t *ppos)
{
	if (*ppos >= FILE_SIZE)
		return 0;

	if (*ppos + count > FILE_SIZE)
		count = FILE_SIZE - *ppos;

	if (copy_to_user(buffer, file_body + *ppos, count))
		return -EFAULT;

	*ppos += count;

	return count;
}

static ssize_t file_write(struct file *filp, const char __user *buffer, size_t count, loff_t *ppos)
{
	if (*ppos >= FILE_SIZE)
		return 0;

	if (*ppos + count > FILE_SIZE)
		count = FILE_SIZE - *ppos;

	if (copy_from_user(file_body + *ppos, buffer, count))
		return -EFAULT;

	*ppos += count;

	return count;
}

struct file_operations file_fops = {
	.open = file_open,
	.read = file_read,
	.write = file_write,
};

static int __init debugfs_init(void)
{
	struct dentry *pdentry_u32, *pdentry_atomic,
		      *pdentry_string, *pdentry_file;

	printk(KERN_INFO "sample debug fs module init\n");

	debugfs_dir = debugfs_create_dir("hello", NULL);
	if (!debugfs_dir)
		return -ENOENT;

	pdentry_u32 = debugfs_create_u32("var_u32", S_IRUGO | S_IWUSR, debugfs_dir, &var_u32);
	if (!pdentry_u32)
		goto fail;

	pdentry_atomic = debugfs_create_atomic_t("var_atomic", S_IRUGO | S_IWUSR, debugfs_dir, &var_atomic);
	if (!pdentry_atomic)
		goto fail;

	var_string_blob_wrapper.data = (void *)var_string;
	var_string_blob_wrapper.size = sizeof(var_string);
	pdentry_string = debugfs_create_blob("var_string", S_IRUGO, debugfs_dir, &var_string_blob_wrapper);
	if (!pdentry_string)
		goto fail;

//	pdentry_file = debugfs_create_file("file", 0644, debugfs_dir, NULL, &file_fops); // file will seen as 0
	pdentry_file = debugfs_create_file_size("file", S_IRUGO | S_IWUSR, debugfs_dir, NULL, &file_fops, FILE_SIZE);
	if (!pdentry_file)
		goto fail;

	return 0;
fail:
	debugfs_remove_recursive(debugfs_dir);
	return -ENOENT;
}

static void __exit debugfs_exit(void)
{
	printk(KERN_INFO "sample debug fs module exit\n");
	debugfs_remove_recursive(debugfs_dir);
	return;
}

module_init(debugfs_init);
module_exit(debugfs_exit);

MODULE_LICENSE("GPL");
