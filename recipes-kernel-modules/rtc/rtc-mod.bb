SUMMARY = "Virtual Real Time Clock"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://rtc.c \
          "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-vrtc"
