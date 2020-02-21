SUMMARY = "notifiers"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://not1.c \
           file://not2.c \
           file://not3.c \
          "

S = "${WORKDIR}"
