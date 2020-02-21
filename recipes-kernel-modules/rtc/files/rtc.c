/*
 * Driver for virtual RTC devices
 * Creates the vrtc device
 *
 * Time dilation factor can be readed/changed via:
 *   /sys/class/rtc/rtcN/device/time_dilation_factor
 *
 * (!) Dilation factor is thousandths of multiplier for one second tick
 *
 * Interactions:
 *  # cat /sys/class/rtc/rtcN/date
 *  # cat /sys/class/rtc/rtcN/time
 *  # cat /sys/class/rtc/rtcN/since_epoch
 *  # hwclock -r -f /dev/rtcN
 *
 */
#include <linux/module.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>

struct vrtc_priv {
	struct rtc_device *rtc;
	struct timer_list dilation_timer;
	unsigned int time_dilation_factor;
	time64_t time_seconds;
};

static struct platform_device *vrtc_pdev;

static int vrtc_read_time(struct device *dev, struct rtc_time *tm)
{
	struct vrtc_priv *priv = dev_get_drvdata(dev);
	rtc_time64_to_tm(priv->time_seconds, tm);
	return 0;
}

static int vrtc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct vrtc_priv *priv = dev_get_drvdata(dev);
	priv->time_seconds = rtc_tm_to_time64(tm);
	return 0;
}

static const struct rtc_class_ops vrtc_ops = {
	.read_time	= vrtc_read_time,
	.set_time	= vrtc_set_time,
};

static void vrtc_dilation_timer_handler(struct timer_list *t)
{
	struct vrtc_priv *priv = from_timer(priv, t, dilation_timer);
	unsigned int delay = ((unsigned int)HZ * priv->time_dilation_factor) / 1000;

	priv->time_seconds += 1;
	mod_timer(t, jiffies + (delay>0?delay:1));
}

static ssize_t show_time_dilation_factor(struct device *dev,
		    struct device_attribute *attr, char *buf)
{
	struct vrtc_priv *priv = dev_get_drvdata(dev);
	return sprintf(buf, "%u\n", priv->time_dilation_factor);
}

static ssize_t store_time_dilation_factor(struct device *dev,
		    struct device_attribute *attr, const char *buf,
		    size_t size)
{
	struct vrtc_priv *priv = dev_get_drvdata(dev);
	int ret;
	unsigned int new;

	ret = kstrtouint(buf, 0, &new);
	if (ret)
		return ret;

	priv->time_dilation_factor = new;

	dev_printk(KERN_INFO, dev, "new time_dilation_factor = %u\n", priv->time_dilation_factor);

	return size;
}

static DEVICE_ATTR(time_dilation_factor, S_IRUGO | S_IWUSR, show_time_dilation_factor, store_time_dilation_factor);

struct device_attribute *vrtc_attrs[] = {
	&dev_attr_time_dilation_factor,
	NULL
};

static struct attribute_group vrtc_attribute_group = {
	.name = NULL,	/* put in device directory */
	.attrs = (struct attribute **)vrtc_attrs,
};

static int vrtc_probe(struct platform_device *pdev)
{
	struct rtc_device *rtc;
	struct vrtc_priv *priv;
	int ret;

	priv = devm_kzalloc(&pdev->dev, sizeof(struct vrtc_priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	platform_set_drvdata(pdev, priv);
	rtc = devm_rtc_device_register(&pdev->dev, "vrtc", &vrtc_ops,
					THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	/* dilation factor,
	   default value is 1000 of thousandths which is equal to multiply by 1 */
	priv->time_dilation_factor = 1000;

	ret = sysfs_create_group(&pdev->dev.kobj, &vrtc_attribute_group);
	if (ret) {
		printk(KERN_ERR "vrtc: failed to create sysfs device attributes.\n");
		return -1;
	}

	/* start time */
	priv->time_seconds = ktime_get_real_seconds();

	timer_setup(&priv->dilation_timer, vrtc_dilation_timer_handler, 0);

	priv->rtc = rtc;

	priv->dilation_timer.expires = HZ * priv->time_dilation_factor / 1000;

	add_timer(&priv->dilation_timer);

	return 0;
}

static int vrtc_remove(struct platform_device *pdev)
{
	struct vrtc_priv *priv = dev_get_drvdata(&pdev->dev);
	del_timer_sync(&priv->dilation_timer);
	return 0;
}

static struct platform_driver vrtc_driver = {
	.probe  = vrtc_probe,
	.remove = vrtc_remove,
	.driver = {
		.name = "vrtc",
	},
};

static int __init vrtc_init(void)
{
	int err;

	err = platform_driver_register(&vrtc_driver);
	if (err)
		return err;

	err = -ENOMEM;
	vrtc_pdev = platform_device_alloc("vrtc", 0);
	if (!vrtc_pdev)
		goto exit_free_mem;

	err = platform_device_add(vrtc_pdev);
	if (err)
		goto exit_device_del;

	return 0;

exit_device_del:
	platform_device_del(vrtc_pdev);
exit_free_mem:
	platform_device_put(vrtc_pdev);
	platform_driver_unregister(&vrtc_driver);
	return err;
}

static void __exit vrtc_exit(void)
{
	platform_device_unregister(vrtc_pdev);
	platform_driver_unregister(&vrtc_driver);
}

module_init(vrtc_init);
module_exit(vrtc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("virtual rtc device driver");
MODULE_AUTHOR("avx");
