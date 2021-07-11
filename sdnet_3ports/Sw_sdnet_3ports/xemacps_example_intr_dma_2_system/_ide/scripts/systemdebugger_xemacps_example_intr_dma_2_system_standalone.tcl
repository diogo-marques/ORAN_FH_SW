# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/xemacps_example_intr_dma_2_system/_ide/scripts/systemdebugger_xemacps_example_intr_dma_2_system_standalone.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/xemacps_example_intr_dma_2_system/_ide/scripts/systemdebugger_xemacps_example_intr_dma_2_system_standalone.tcl
# 
connect -url tcp:127.0.0.1:3121
source /tools/Xilinx/Vitis/2020.2/scripts/vitis/util/zynqmp_utils.tcl
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent JTAG-SMT2NC 210308ABAA67" && level==0 && jtag_device_ctx=="jsn-JTAG-SMT2NC-210308ABAA67-24738093-0"}
fpga -file /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/xemacps_example_intr_dma_2/_ide/bitstream/mb_es_design_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/sw_sdnet_platform/export/sw_sdnet_platform/hw/mb_es_design_wrapper.xsa -mem-ranges [list {0x80000000 0xbfffffff} {0x400000000 0x5ffffffff} {0x1000000000 0x7fffffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
set mode [expr [mrd -value 0xFF5E0200] & 0xf]
targets -set -nocase -filter {name =~ "*A53*#0"}
rst -processor
dow /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/sw_sdnet_platform/export/sw_sdnet_platform/sw/sw_sdnet_platform/boot/fsbl.elf
set bp_34_43_fsbl_bp [bpadd -addr &XFsbl_Exit]
con -block -timeout 60
bpremove $bp_34_43_fsbl_bp
targets -set -nocase -filter {name =~ "*A53*#0"}
rst -processor
dow /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/xemacps_example_intr_dma_2/Debug/xemacps_example_intr_dma_2.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A53*#0"}
con
