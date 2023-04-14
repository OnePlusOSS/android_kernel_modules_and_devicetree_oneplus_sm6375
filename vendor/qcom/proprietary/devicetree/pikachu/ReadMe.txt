DTS修改规则：
1.QCOM 路径下的所有修改保持原生态，不做侵入式修改
2.项目base dts放在blair-pikachu.dts，其中包含高通base dtsi
3.平台共性overlay放在blair-pikachu-overlay-common.dtsi中。
4.项目之间的差异，以21291为例，放在blair-pikachu-21291-overlay.dtsi中，不要直接修改到blair-pikachu-21291-overlay.dts
