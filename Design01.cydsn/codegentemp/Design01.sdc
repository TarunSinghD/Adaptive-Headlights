# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\rohi2\Desktop\HRLV-EZ4.cywrk.Archive01\Backup 23 Aug 2017 1_42pm\HRLV-EZ4 - Indexing second motor\Design01.cydsn\Design01.cyprj
# Date: Wed, 23 Aug 2017 23:00:59 GMT
#set_units -time ns
create_clock -name {Clock_2(routed)} -period 1000 -waveform {0 500} [list [get_pins {ClockBlock/dclk_2}]]
create_clock -name {Clock_1(routed)} -period 1000 -waveform {0 500} [list [get_pins {ClockBlock/dclk_3}]]
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {Clock_3} -source [get_pins {ClockBlock/clk_sync}] -edges {1 3 5} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock_4} -source [get_pins {ClockBlock/clk_sync}] -edges {1 3 5} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 25 49} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 25 49} [list [get_pins {ClockBlock/dclk_glb_3}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\rohi2\Desktop\HRLV-EZ4.cywrk.Archive01\Backup 23 Aug 2017 1_42pm\HRLV-EZ4 - Indexing second motor\Design01.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\rohi2\Desktop\HRLV-EZ4.cywrk.Archive01\Backup 23 Aug 2017 1_42pm\HRLV-EZ4 - Indexing second motor\Design01.cydsn\Design01.cyprj
# Date: Wed, 23 Aug 2017 23:00:52 GMT
