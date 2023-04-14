DTS修改规则：
1.QCOM 路径下的所有修改保持原生态，不做侵入式修改
2.项目base dts放在*mozart.dts，包含高通base dts
3.平台共性overlay放在*mozart-verlay.dtsi中。
4.项目之间的差异，放在*overlay.dts中，客制化修改只能放到*overlay.dtsi