SUMMARY = "rdtsc example"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://busy-wait.c \
           file://sleep_interruptible.c \
           file://sleep_uninterruptible.c \
          "

S = "${WORKDIR}"
