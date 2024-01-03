ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += blair-pikachu-a-22273-overlay.dtbo

blair-pikachu-a-22273-overlay.dtbo-base := blair-pikachu-a.dtb
endif

always := $(dtb-y)
subdir-y := $(dts-dirs)
clean-files := *.dtb *.dtbo
