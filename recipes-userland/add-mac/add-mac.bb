DESCRIPTION = "User-space utility to add MAC addresses to packetmod kernel module via Netlink"
LICENSE = "CLOSED"
SRC_URI = "file://add_mac.c \
           file://packetmod.h"

S = "${WORKDIR}"

DEPENDS = ""

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} add_mac.c -o add_mac
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 add_mac ${D}${bindir}/add_mac
}

FILES:${PN} += "${bindir}/add_mac"

RDEPENDS:${PN} += "packetmod"