SUMMARY = "net device"
LICENSE = "CLOSED"

inherit module

PR = "r0"
PV = "0.1"

SRC_URI = "file://Makefile \
           file://avxnet.c \
          "

S = "${WORKDIR}"
