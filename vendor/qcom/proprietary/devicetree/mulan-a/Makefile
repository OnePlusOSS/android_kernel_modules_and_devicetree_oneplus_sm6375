ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += blair-mulan-a-22041-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-mulan-a-22042-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-mulan-a-22241-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-mulan-a-22242-overlay.dtbo

blair-mulan-a-22041-overlay.dtbo-base := blair-mulan-a-22041.dtb
blair-mulan-a-22042-overlay.dtbo-base := blair-mulan-a.dtb
blair-mulan-a-22241-overlay.dtbo-base := blair-mulan-a.dtb
blair-mulan-a-22242-overlay.dtbo-base := blair-mulan-a.dtb
endif

always := $(dtb-y)
subdir-y := $(dts-dirs)
clean-files := *.dtb *.dtbo
