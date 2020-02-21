SUMMARY = "rdtsc example"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://rdtsc.c \
          "

S = "${WORKDIR}"
