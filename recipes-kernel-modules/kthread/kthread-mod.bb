SUMMARY = "kernel threads example"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://kthread-mod.c \
          "

S = "${WORKDIR}"
