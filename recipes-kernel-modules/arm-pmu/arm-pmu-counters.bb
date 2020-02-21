SUMMARY = "Example of user application for ARM which use PMU cycle counters with support kernel module for enable user access to PMU"
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://Makefile \
           file://pmu-uaccess-mod.c \
           file://pmu-uapp.c \
          "

S = "${WORKDIR}"

do_compile_append () {
        ${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/pmu-uapp.c -o ${WORKDIR}/pmu-uapp
}

do_install_append () {
        install -d ${D}/usr/bin/
        install -m 0755 ${WORKDIR}/pmu-uapp ${D}/usr/bin/
}

FILES_${PN} += "/usr/bin/pmu-uapp"
