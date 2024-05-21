ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += blair-mozart-21221-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-mozart-21223-overlay.dtbo

blair-mozart-21221-overlay.dtbo-base := blair-mozart.dtb
blair-mozart-21223-overlay.dtbo-base := blair-mozart.dtb
endif

always := $(dtb-y)
subdir-y := $(dts-dirs)
clean-files := *.dtb *.dtbo
