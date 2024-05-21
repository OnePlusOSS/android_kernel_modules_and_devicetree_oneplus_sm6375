ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += blair-larry-22821-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-larry-22872-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-larry-22871-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-larry-22874-overlay.dtbo

blair-larry-22821-overlay.dtbo-base := blair-larry.dtb
blair-larry-22872-overlay.dtbo-base := blair-larry.dtb
blair-larry-22871-overlay.dtbo-base := blair-larry.dtb
blair-larry-22874-overlay.dtbo-base := blair-larry.dtb
endif

always := $(dtb-y)
subdir-y := $(dts-dirs)
clean-files := *.dtb *.dtbo
