################################################################################
#
# Module test
#
################################################################################

MODULE_TEST_VERSION = 0.0.1
MODULE_TEST_SITE = ${BR2_EXTERNAL_bragov4ik_kernel_mod_PATH}/package/module_test/src
MODULE_TEST_SITE_METHOD = local
MODULE_TEST_LICENSE = GPL-2.0
MODULE_TEST_LICENSE_FILES = COPYING

define MODULE_TEST_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define MODULE_TEST_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/test_program $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/launch.sh $(TARGET_DIR)/root
endef

$(eval $(generic-package))
