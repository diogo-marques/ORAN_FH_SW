## 5G O-RAN Fronthaul Mini Switch

**Board used:** Zynq UltraScale+ MPSoC ZCU102 Evaluation Kit

**Software Version:** Vivado Design Suite and Vitis  2020.2

**Brief description of project:** High performance switch with two ports of 10Gbps and one Gbps, which is able to prioritize and recognize traffic from the various ORAN planes, capable of combining, in a single optical link, general purpose and high priority traffic between DU and RU of a 5G system. The switch data plane is  fully  programmable through  the  P4  language.

## Gigabit Ethernet  MAC (GEM) Controller - External FIFO Interface

**In order to use the GEM present in the PS-side on the PL side the following was done:**

* Enabled the option "External FIFO Interface" in the PS-PL Configuration tab.
* Changes have been made to the ["xemacps_example_intr_dma"](https://github.com/Xilinx/embeddedsw/blob/master/XilinxProcessorIPLib/drivers/emacps/examples/xemacps_example_intr_dma.c) example of the [XEMACPS](https://xilinx.github.io/embeddedsw.github.io/emacps/doc/html/api/index.html) driver in order to support the external FIFO interface. Note: The driver only supported the DMA option.
* Two modules were created that convert the FIFO_ENET3 interface, which is available in Zynq UltraScale+ MPSoC, into the AXI4-Stream interface in order to facilitate the use of the data received.
  * These modules (axis_to_enet” and ”enet_to_axis”) can be found in the folder: [sdnet_3ports/sdnet_3ports.srcs/sources_1/imports](https://github.com/diogo-marques/ORAN_FH_SW/tree/master/sdnet_3ports/sdnet_3ports.srcs/sources_1/imports/Vivado_projects)
