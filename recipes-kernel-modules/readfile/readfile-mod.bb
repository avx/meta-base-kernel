SUMMARY = "read file from kernel space"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://readfile-mod.c \
          "
S = "${WORKDIR}"
