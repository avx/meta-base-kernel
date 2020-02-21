LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://master-mod.c \
           file://slave-mod.c \
          "

S = "${WORKDIR}"
