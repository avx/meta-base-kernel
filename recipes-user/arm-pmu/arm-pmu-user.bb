SUMMARY = "arm pmu user test"
SECTION = "base"
LICENSE = "CLOSED"

RDEPENDS_${PN} = "arm-pmu-mod"

SRC_URI = "file://arm-pmu-user.c"

do_compile () {
	${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/arm-pmu-user.c -o ${WORKDIR}/arm-pmu-user
}

do_install () {
	install -d ${D}/usr/bin/
	install -m 0755 ${WORKDIR}/arm-pmu-user ${D}/usr/bin/
}

FILES_${PN} = "/usr/bin/arm-pmu-user"
