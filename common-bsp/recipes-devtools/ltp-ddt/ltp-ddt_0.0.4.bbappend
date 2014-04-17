FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

# this is HEAD as of 4/15/2014
PV = "0.0.6pre"
SRCREV = "dbc8c3b346f8cc2549f00978d6b2206c7c4de7d0"

do_configure_prepend() {
    # remove dependency on meta-ti packages
    sed -i -e '/edma/d' "${S}"/platforms/beaglebone-black || bbfatal
}

