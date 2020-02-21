LICENSE = "CLOSED"
inherit module

SRC_URI = "file://Makefile              \
           file://irq-isr-mod.c         \
           file://irq-tasklet-mod.c     \
           file://irq-workq-mod.c       \
          "

S = "${WORKDIR}"
