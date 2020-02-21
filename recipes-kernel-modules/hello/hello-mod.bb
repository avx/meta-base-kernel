SUMMARY = "Example of how to build an external Linux kernel module"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"

inherit module

SRC_URI = "file://COPYING \
           file://Makefile \
           file://hello1.c \
           file://hello2-init.c \
           file://hello2-main.c \
"

S = "${WORKDIR}"

INSANE_SKIP += "arch"
