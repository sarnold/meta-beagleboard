# Base this image on basic-image
require basic-image.bb

IMAGE_FEATURES += "ssh-server-openssh package-management"

IMAGE_INSTALL_append = " packagegroup-ti-test"
