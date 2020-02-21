#define _GNU_SOURCE /* for sched affinity */
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>

static void init_pmu(int reset, int ena_div)
{
    /* enable all counters */
    int cr = 1;

    if (reset)
    {
        cr |= 2;        /* reset all counters to zero */
        cr |= 4;        /* reset cycle counter to zero */
    }

    if (ena_div)
        cr |= 8;        /* enable by 64 divider for ccnt */

    cr |= 16;

    /* init pmu */
    asm volatile ("mcr p15, 0, %0, c9, c12, 0" :: "r"(cr));

    /* enable all counters */
    asm volatile ("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));

    /* clear overflows */
    asm volatile ("mcr p15, 0, %0, c9, c12, 3" :: "r"(0x8000000f));
}

/* get pmu cpu counters */
static inline unsigned int get_cycles(void)
{
    unsigned int ccnt;
    asm volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r"(ccnt));
    return ccnt;
}

int main()
{
    volatile unsigned int t, i;
    cpu_set_t set;
    int cpu;

    CPU_ZERO(&set);
    CPU_SET(cpu = sched_getcpu(), &set);

    if (sched_setaffinity(0, sizeof(set), &set))
    {
        printf("sched affinity error: %m");
        exit(-1);
    }

    printf("bound to cpu#%d\n", cpu);

    /* init PMU for current cpu */
    init_pmu(0,0);

    t = get_cycles();
    t = get_cycles() - t;

    if (t<=0)
    {
        printf("pmu doesn't count cycles, i am quit...\n");
        exit(-1);
    }

    printf("pmu counts cycles, overhead is %u\n", t);

    t = get_cycles();

    /* some work */
    for (i=0; i<1234567; i++);

    t = get_cycles() - t;

    printf ("some work took %d cycles\n", t);

    return 0;
}
