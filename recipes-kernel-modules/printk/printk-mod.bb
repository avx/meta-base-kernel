SUMMARY = "debug methods"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://printk-mod.c \
          "

S = "${WORKDIR}"
