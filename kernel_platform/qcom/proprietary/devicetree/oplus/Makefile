BLAIR_BASE_DTB += ../qcom/blair.dtb
BLAIR_APQ_BASE_DTB += ../qcom/blairp.dtb

OPLUS_BLAIR_BOARDS += blair-camry-23881-overlay.dtbo\
                      blair-camry-23882-overlay.dtbo\
                      blair-camry-23882-overlay-EU.dtbo

dtb-y += $(OPLUS_BLAIR_BOARDS) $(BLAIR_BASE_DTB) $(BLAIR_APQ_BASE_DTB)

always-y	:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb *.dtbo
