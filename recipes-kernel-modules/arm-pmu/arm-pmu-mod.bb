SUMMARY = "arm pmu cycle counters enable for user apps module"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://arm-pmu.c \
          "

S = "${WORKDIR}"
