################################################################################
#
# bragov4ik
#
################################################################################

BRAGOV4IK_VERSION = 0.0.1
BRAGOV4IK_SITE = ${BR2_EXTERNAL_bragov4ik_kernel_mod_PATH}/package/bragov4ik/src
BRAGOV4IK_SITE_METHOD = local
BRAGOV4IK_LICENSE = GPL-2.0
BRAGOV4IK_LICENSE_FILES = COPYING

$(eval $(kernel-module))
$(eval $(generic-package))
