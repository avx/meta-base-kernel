SUMMARY = "kernel timers"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://timer-mod.c \
          "

S = "${WORKDIR}"

INSANE_SKIP += "arch"
