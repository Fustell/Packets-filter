DESCRIPTION = "User-space utility to add IPv4 addresses to packetmod kernel module via Netlink"
LICENSE = "CLOSED"
SRC_URI = "file://add_ipv4.c \
           file://packetmod.h"

S = "${WORKDIR}"

DEPENDS = ""

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} add_ipv4.c -o add_ipv4
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 add_ipv4 ${D}${bindir}/add_ipv4
}

FILES:${PN} += "${bindir}/add_ipv4"

RDEPENDS:${PN} += "packetmod"