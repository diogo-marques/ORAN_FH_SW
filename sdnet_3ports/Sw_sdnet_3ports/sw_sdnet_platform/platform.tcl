# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/sw_sdnet_platform/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/sw_sdnet_platform/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {sw_sdnet_platform}\
-hw {/home/dmarques/tese/Vivado_projects/sdnet_3ports/mb_es_design_wrapper.xsa}\
-proc {psu_cortexa53_0} -os {standalone} -arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {/home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports}

platform write
platform generate -domains 
platform active {sw_sdnet_platform}
platform generate
domain active {zynqmp_fsbl}
bsp reload
domain active {standalone_domain}
bsp reload
catch {platform remove sdnet_platform_2}
