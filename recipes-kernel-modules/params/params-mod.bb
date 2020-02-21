SUMMARY = "module parameters"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://params-mod.c \
           file://params-cb-mod.c \
          "

S = "${WORKDIR}"
