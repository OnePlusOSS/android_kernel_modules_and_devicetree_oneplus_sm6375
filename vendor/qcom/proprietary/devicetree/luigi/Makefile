ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
dtbo-$(CONFIG_ARCH_HOLI) += holi-luigi-22667-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += holi-luigi-22668-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += holi-luigi-22602-overlay.dtbo
dtbo-$(CONFIG_ARCH_HOLI) += holi-luigi-22601-overlay.dtbo


holi-luigi-22667-overlay.dtbo-base := holi-luigi.dtb
holi-luigi-22668-overlay.dtbo-base := holi-luigi.dtb
holi-luigi-22602-overlay.dtbo-base := holi-luigi.dtb
holi-luigi-22601-overlay.dtbo-base := holi-luigi.dtb
endif

always := $(dtb-y)
subdir-y := $(dts-dirs)
clean-files := *.dtb *.dtbo
