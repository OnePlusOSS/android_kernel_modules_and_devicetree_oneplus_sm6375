ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += blair-colombo-22045-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-colombo-22247-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-colombo-22248-overlay.dtbo

blair-colombo-22045-overlay.dtbo-base := blair-colombo.dtb
blair-colombo-22247-overlay.dtbo-base := blair-colombo.dtb
blair-colombo-22248-overlay.dtbo-base := blair-colombo.dtb
endif

always := $(dtb-y)
subdir-y := $(dts-dirs)
clean-files := *.dtb *.dtbo
