#include <linux/module.h>
#include <linux/percpu.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,4,0)
#define rdtsc_ordered rdtsc
unsigned long long rdtsc(void)
{
    unsigned int low, high;

    asm volatile("" ::: "memory");      // barrier
    asm volatile("rdtsc" : "=a" (low), "=d" (high));

    return low | ((unsigned long long)high) << 32;
}
#endif

#if defined(__x86_64__) || defined(__i386__)
# define WITH_RDTSC
#else
# warning arch has no support for rdtsc
#endif

#ifdef WITH_RDTSC
struct tsc_stat_s {
    u64 cp[4];
};
static DEFINE_PER_CPU(struct tsc_stat_s, tsc_stat);
#endif

static void hl_fn(void)
{
#ifdef WITH_RDTSC
    u64 tsc1, tsc2, tsc3, tsc4;
    struct tsc_stat_s *stat = this_cpu_ptr(&tsc_stat);
#endif
    cycles_t start, end;

    start = get_cycles();

#ifdef WITH_RDTSC
    tsc1 = rdtsc();
#endif

    printk("hello, world!\n");

#ifdef WITH_RDTSC
    tsc2 = rdtsc_ordered();
#endif

    printk("hello, world! hello, world! hello, world!\n");

#ifdef WITH_RDTSC
    tsc3 = rdtsc_ordered();
#endif

    printk("hello, world! hello, world! hello, world! hello, world! hello, world! hello, world! hello, world! hello, world! hello, world! hello, world! hello, world! hello, world!\n");

#ifdef WITH_RDTSC
    tsc4 = rdtsc_ordered();
#endif

    end = get_cycles();

    printk("printing took %llu cycles (get_cycles)\n", end - start);

#ifdef WITH_RDTSC
    printk("printing took %llu cycles (rdtsc)\n", tsc4-tsc1);

    stat->cp[0] += tsc2-tsc1;
    stat->cp[1] += tsc3-tsc2;
    stat->cp[2] += tsc4-tsc3;
    stat->cp[3] += tsc4-tsc1;
#endif
}

static int __init mod_init(void)
{
#ifdef WITH_RDTSC
    struct tsc_stat_s *stat = this_cpu_ptr(&tsc_stat);
#endif
    unsigned long a,b;
    int i;

    a = jiffies;

    for(i=0;i<100;i++) {
        hl_fn();
        hl_fn();
        hl_fn();
        hl_fn();
        hl_fn();
    }

    b = jiffies;

    printk("it took %lu jiffies\n", b - a);

#ifdef WITH_RDTSC
    printk("cp0 = %llu\ncp1 = %llu\ncp2 = %llu\ncp4 = %llu\n", stat->cp[0],stat->cp[1],stat->cp[2],stat->cp[3]);
#endif

    return -1;
}

module_init(mod_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("avx");
MODULE_DESCRIPTION("rdtsc usage example");
