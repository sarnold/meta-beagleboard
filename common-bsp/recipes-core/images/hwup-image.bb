LICENSE = "MIT"

# Base this image on core-image-minimal
include recipes-core/images/core-image-minimal.bb

# Include modules in rootfs (avahi for networking)
IMAGE_INSTALL += " \
	kernel-modules \
	avahi-utils \
	"
