DESCRIPTION = "TCP Packet Modifier Kernel Module"
LICENSE = "CLOSED"

SRC_URI = "file://src/packetmod_main.c \
           file://include/packetmod.h \
           file://src/filter_mac.c \
           file://include/filter_mac.h \
           file://src/filter_ipv4.c \
           file://include/filter_ipv4.h \
           file://Makefile"


inherit module

S = "${WORKDIR}"

KERNEL_MODULE_AUTOLOAD = "packetmod"
DEPENDS += "virtual/kernel"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_PACKAGE_STRIP = "1"
