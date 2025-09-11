DESCRIPTION = "TCP Packet Modifier Kernel Module"
LICENSE = "CLOSED"

SRC_URI = "file://src/packetmod_main.c \
           file://include/packetmod.h \
           file://src/filter_mac.c \
           file://include/filter_mac.h \
           file://src/filter_ipv4.c \
           file://include/filter_ipv4.h \
           file://Makefile"

S = "${WORKDIR}"

inherit module

KERNEL_MODULE_AUTOLOAD = "packetmod"
DEPENDS += "virtual/kernel"
EXTRA_OEMAKE += "KCFLAGS+=-Wno-error -I${S}/include"