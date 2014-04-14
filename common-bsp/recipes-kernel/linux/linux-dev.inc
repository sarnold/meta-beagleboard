DESCRIPTION = "Linux Kernel"
SECTION = "kernel"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

inherit kernel siteinfo machine_kernel_pr

# Enable OABI compat for people stuck with obsolete userspace
ARM_KEEP_OABI ?= "0"

DISTRO_TYPE ?= "${@base_contains("IMAGE_FEATURES", "debug-tweaks", "debug", "",d)}"

# Set the verbosity of kernel messages during runtime
# You can define CMDLINE_DEBUG in your local.conf or distro.conf to override this behaviour
CMDLINE_DEBUG ?= '${@base_conditional("DISTRO_TYPE", "release", "quiet", "debug", d)}'
CMDLINE_append = " ${CMDLINE_DEBUG}"

do_configure_prepend() {
    # Clean .config and use default upstream (RNC) config
    echo "" > ${S}/.config
    cp -f ${WORKDIR}/${KERNEL_DEFCONFIG} ${S}/.config

    if [ "${OVERRIDE_DEFCONFIG}" = "1" ]; then
        cp -f ${WORKDIR}/custom_defconfig ${S}/.config
    fi

    yes '' | oe_runmake oldconfig
}

# Automatically depend on lzop-native if CONFIG_KERNEL_LZO is enabled
python () {
    try:
        defconfig = bb.fetch2.localpath('file://defconfig', d)
    except bb.fetch2.FetchError:
        return

    try:
        configfile = open(defconfig)
    except IOError:
        return

    if 'CONFIG_KERNEL_LZO=y\n' in configfile.readlines():
        depends = d.getVar('DEPENDS', False)
        d.setVar('DEPENDS', depends + ' lzop-native')
}
