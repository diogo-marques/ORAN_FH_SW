#ZCU102 constraints

#GT ref clocl P/N
set_property PACKAGE_PIN C7 [get_ports gt_refclk_n]
set_property PACKAGE_PIN C8 [get_ports gt_refclk_p]

#SFP 0 TX DISABLE

set_property PACKAGE_PIN A12 [get_ports {sfp_tx_disable[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sfp_tx_disable[0]}]

#SFP 1 TX DISABLE

set_property PACKAGE_PIN A13      [get_ports {sfp_tx_disable[1]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {sfp_tx_disable[1]}] 

#CPU_RESET

set_property PACKAGE_PIN AM13 [get_ports sys_reset]
set_property IOSTANDARD LVCMOS33 [get_ports sys_reset]

