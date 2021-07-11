# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/mb_es_design_wrapper/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports/mb_es_design_wrapper/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {mb_es_design_wrapper}\
-hw {/home/dmarques/tese/Vivado_projects/sdnet_3ports/mb_es_design_wrapper.xsa}\
-arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {/home/dmarques/tese/Vivado_projects/sdnet_3ports/Sw_sdnet_3ports}

platform write
domain create -name {standalone_psu_cortexa53_0} -display-name {standalone_psu_cortexa53_0} -os {standalone} -proc {psu_cortexa53_0} -runtime {cpp} -arch {64-bit} -support-app {hello_world}
platform generate -domains 
platform active {mb_es_design_wrapper}
domain active {zynqmp_fsbl}
domain active {zynqmp_pmufw}
domain active {standalone_psu_cortexa53_0}
platform generate -quick
domain active {zynqmp_pmufw}
bsp reload
domain active {standalone_psu_cortexa53_0}
bsp reload
platform generate
platform config -updatehw {/home/dmarques/tese/Vivado_projects/sdnet_3ports/mb_es_design_wrapper.xsa}
platform generate -domains 
platform config -updatehw {/home/dmarques/tese/Vivado_projects/sdnet_3ports/mb_es_design_wrapper.xsa}
platform generate -domains 
platform active {mb_es_design_wrapper}
platform config -updatehw {/home/dmarques/tese/Vivado_projects/sdnet_3ports/mb_es_design_wrapper.xsa}
platform generate -domains 
platform config -updatehw {/home/dmarques/tese/Vivado_projects/sdnet_3ports/mb_es_design_wrapper.xsa}
platform generate -domains 
platform active {mb_es_design_wrapper}
platform config -updatehw {/home/dmarques/tese/Vivado_projects/sdnet_3ports/mb_es_design_wrapper.xsa}
catch {platform remove mb_es_design_wrapper}
