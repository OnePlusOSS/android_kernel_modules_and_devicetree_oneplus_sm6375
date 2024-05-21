ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += \
	odin-21220-mtp-overlay.dtbo \
        odin-20121-mtp-overlay.dtbo \
        odin-20281-mtp-overlay.dtbo \
        odin-20283-mtp-overlay.dtbo \
        odin-21227-mtp-overlay.dtbo \

odin-20121-mtp-overlay.dtbo-base := holi.dtb
odin-20281-mtp-overlay.dtbo-base := holi.dtb
odin-20283-mtp-overlay.dtbo-base := holi.dtb
odin-21227-mtp-overlay.dtbo-base := holi.dtb
odin-21220-mtp-overlay.dtbo-base := holi.dtb

else
dtb-$(CONFIG_ARCH_HOLI) += holi-rumi.dtb \
	holi-mtp.dtb \

endif


always		:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb *.dtbo
