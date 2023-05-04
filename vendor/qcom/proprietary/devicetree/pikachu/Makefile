ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += blair-pikachu-21291-overlay.dtbo

blair-pikachu-21291-overlay.dtbo-base := blair-pikachu.dtb
endif

always := $(dtb-y)
subdir-y := $(dts-dirs)
clean-files := *.dtb *.dtbo
