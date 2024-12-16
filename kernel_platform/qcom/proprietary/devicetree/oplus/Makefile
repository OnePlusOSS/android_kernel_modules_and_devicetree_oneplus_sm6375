BLAIR_BASE_DTB += ../qcom/blair.dtb
BLAIR_APQ_BASE_DTB += ../qcom/blairp.dtb

OPLUS_BLAIR_BOARDS += blair-larry-22821-overlay.dtbo\
                      blair-larry-22872-overlay.dtbo\
                      blair-larry-22871-overlay.dtbo\
                      blair-larry-22874-overlay.dtbo\
                      blair-camry-23071-overlay-EVB1.dtbo\
                      blair-camry-23881-overlay-EVB1.dtbo\
                      blair-camry-23882-overlay-EVB1.dtbo\
                      blair-camry-23071-overlay-T1.dtbo\
                      blair-camry-23881-overlay-T1.dtbo\
                      blair-camry-23882-overlay-T1.dtbo\
                      blair-camry-23071-overlay-EVT1.dtbo\
                      blair-camry-23881-overlay-EVT1.dtbo\
                      blair-camry-23882-overlay-EVT1.dtbo\
                      blair-camry-23071-overlay.dtbo\
                      blair-camry-23881-overlay.dtbo\
                      blair-camry-23882-overlay.dtbo\
                      blair-avatar-m-23091-overlay.dtbo\
                      blair-avatar-m-23291-overlay.dtbo

dtb-y += $(OPLUS_BLAIR_BOARDS) $(BLAIR_BASE_DTB) $(BLAIR_APQ_BASE_DTB)

always-y	:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb *.dtbo
