# Base this image on hwup-image and core-image-minimal
require hwup-image.bb

IMAGE_FEATURES += "ssh-server-dropbear splash"
