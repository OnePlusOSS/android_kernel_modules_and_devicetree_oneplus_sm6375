ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += blair-hepburn-22035-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-hepburn-22253-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-hepburn-22254-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-hepburn-22255-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += blair-hepburn-22035-overlay-v2.dtbo

blair-hepburn-22035-overlay.dtbo-base := blair-hepburn.dtb
blair-hepburn-22253-overlay.dtbo-base := blair-hepburn.dtb
blair-hepburn-22254-overlay.dtbo-base := blair-hepburn.dtb
blair-hepburn-22255-overlay.dtbo-base := blair-hepburn.dtb
blair-hepburn-22035-overlay-v2.dtbo-base := blair-hepburn.dtb
endif

always := $(dtb-y)
subdir-y := $(dts-dirs)
clean-files := *.dtb *.dtbo
