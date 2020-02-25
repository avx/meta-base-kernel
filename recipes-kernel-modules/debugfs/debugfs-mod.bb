SUMMARY = "Example for debugfs ops"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://debugfs.c \
          "

S = "${WORKDIR}"
