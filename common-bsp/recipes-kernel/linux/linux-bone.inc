require linux-dev.inc

DESCRIPTION = "Linux Kernel for BeagleBone (white or black)"
SECTION = "kernel"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

COMPATIBLE_MACHINE = "beaglebone"

S = "${WORKDIR}/git"

KERNEL_DEVICETREE_beaglebone = "am335x-bone.dtb am335x-boneblack.dtb"
KERNEL_DEVICETREE_FLAGS = "-R 8 -S 0x3000"

KERNEL_ALT_IMAGETYPE = "dtbs"
KERNEL_CONSOLE = "tty1"

KERNEL_DEFCONFIG = "defconfig"
OVERRIDE_DEFCONFIG ?= "0"

CORTEXA8FIXUP ?= "yes"

# CMDLINE for bone
CMDLINE_beaglebone = "ro root=/dev/mmcblk0p2 rootwait console=ttyO0,115200n8 console=tty1 rootfstype=ext4 net.ifnames=0"

UDEV_GE_141 ?= "1"

python __anonymous () {

    import bb
    
    devicetree = bb.data.getVar('KERNEL_DEVICETREE', d, 1) or ''
    if devicetree:
        depends = bb.data.getVar("DEPENDS", d, 1)
        bb.data.setVar("DEPENDS", "%s dtc-native" % depends, d)
        packages = bb.data.getVar("PACKAGES", d, 1)
        bb.data.setVar("PACKAGES", "%s kernel-devicetree" % packages, d)
}

do_configure_prepend() {
    if [ -e ${WORKDIR}/am335x-pm-firmware.bin ] ; then
        cp ${WORKDIR}/am335x-pm-firmware.bin ${S}/firmware
    fi
}

do_configure_append() {
    if test -e scripts/Makefile.fwinst ; then
        sed -i -e "s:-m0644:-m 0644:g" scripts/Makefile.fwinst
    fi
}

do_install_append() {
    oe_runmake headers_install INSTALL_HDR_PATH=${D}${exec_prefix}/src/linux-${KERNEL_VERSION} ARCH=$ARCH
    oe_runmake firmware_install INSTALL_MOD_PATH=${D} ARCH=$ARCH

    # Deploy device tree file and firmware
    install -d ${D}/boot
    install -d ${D}/lib/firmware
    install -m 0644 ${S}/firmware/capes/*.dts ${D}/lib/firmware || true
    for device_file in ${KERNEL_DEVICETREE} ; do
        install -m 0644 ${S}/arch/arm/boot/dts/$device_file ${D}/boot
    done

    if [ -n "${BONE_IS_BLACK}" ]; then
        if [ "${BONE_IS_BLACK}" = "1" ]; then
            rm -f ${D}/boot/am335x-bone.dtb
        else
            rm -f ${D}/boot/am335x-boneblack.dtb
        fi
    fi
}

do_deploy_append() {
    install -d ${DEPLOY_DIR_IMAGE}
    for device_file in ${KERNEL_DEVICETREE} ; do
        install -m 0644 ${S}/arch/arm/boot/dts/$device_file ${DEPLOY_DIR_IMAGE}
    done
}

PACKAGES =+ "kernel-firmware"
FILES_kernel-firmware = "/lib/firmware/*"

PACKAGES =+ "kernel-headers"
FILES_kernel-headers = "${exec_prefix}/src/linux*"

FILES_kernel-devicetree = "/boot/am335x-bone*.dtb"

RDEPENDS_kernel-image_beaglebone_append = " kernel-firmware kernel-devicetree"
RPROVIDES_kernel-firmware = "linux-firmware"


