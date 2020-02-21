#include <linux/module.h>
#include <linux/version.h>
#include <linux/netdevice.h>
#include <linux/ip.h>

#define NUM	65536
static struct sk_buff * skbs[NUM];
static int cidx;

static struct net_device *dev;

static int avxdev_open(struct net_device *dev) {
	printk(KERN_INFO "! Hit: my_open(%s)\n", dev->name);

	/* start up the transmission queue */
	netif_start_queue(dev);

	return 0;
}

static int avxdev_close(struct net_device *dev) {
	printk(KERN_INFO "! Hit: my_close(%s)\n", dev->name);

	/* shutdown the transmission queue */
	netif_stop_queue(dev);

	return 0;
}

static netdev_tx_t avxdev_start_xmit(struct sk_buff *skb, struct net_device *dev) {
//	struct iphdr *iph = (struct iphdr *)skb_network_header(skb);
	struct iphdr *iph; // = ip_hdr(skb);
	struct ethhdr *eh = eth_hdr(skb);
//	(skb->pkt_type == PACKET_HOST) &&

	printk("skb=%p: %pM -> %pM", skb, &eh->h_source[0], &eh->h_dest[0]);
	if (skb->protocol == htons(ETH_P_IP)) {
		iph = ip_hdr(skb); // access ip header.

		if (iph)
		{
			printk( KERN_INFO "avx : ip packet: %pI4 -> %pI4\n", &iph->saddr, &iph->daddr); // 0x%x -> 0x%x
		}
	}

#if 1
	if (cidx<NUM) {
		printk(KERN_INFO "avx : NOT drop skb=%p\n", skb);
		skbs[cidx++]=skb;
	}
	else {
		printk(KERN_INFO "avx : drop skb=%p\n", skb);
		dev_kfree_skb(skb);
	}
#else
	printk(KERN_INFO "avx : drop skb=%p\n", skb);

	dev_kfree_skb(skb);
#endif
	return 0;
}

static struct net_device_ops ndo = {
	.ndo_open            = avxdev_open,
	.ndo_stop            = avxdev_close,
	.ndo_start_xmit      = avxdev_start_xmit,
};

static void avxdev_setup(struct net_device *dev) {
	int j;

	/* Fill in the MAC address */
	for(j = 0; j < ETH_ALEN; j++)
		dev->dev_addr[ j ] = (char)j;

	ether_setup(dev);

	dev->netdev_ops = &ndo;
}

static int __init avxdev_init(void) {
	printk(KERN_INFO "avx : Loading network module...");

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 17, 0))
	dev = alloc_netdev(0, "avx%d", avxnet_setup);
#else
	dev = alloc_netdev(0, "avx%d", NET_NAME_ENUM, avxdev_setup);
#endif

	if(register_netdev(dev)) {
		printk(KERN_INFO "avx : Failed to register\n");
		free_netdev(dev);
		return -1;
	}

	printk(KERN_INFO "avx: Succeeded in loading %s!\n", dev_name( &dev->dev ));

	return 0;
}

static void __exit avxdev_exit(void) {
	int i;
	printk(KERN_INFO "avx : unloading network module\n");

	for (i=0;i<cidx;i++)
		if (skbs[i]) {
			printk(KERN_INFO "avx : drop skb=%p\n", skbs[i]);
			kfree_skb(skbs[i]);
		}

	unregister_netdev(dev);
	free_netdev(dev);
}

module_init(avxdev_init);
module_exit(avxdev_exit);

MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("Bogus network device");
MODULE_LICENSE("GPL");
