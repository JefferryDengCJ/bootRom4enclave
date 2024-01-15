#!/bin/bash
echo "bootrom_h"
updatemem -debug -meminfo bootrom_h.mmi -data bootrom_h.mem \
	-bit ../../work-fpga/ariane_xilinx.bit -proc i_bootrom -out bootrom_h.bit -force
echo "bootrom_l"
updatemem -debug -meminfo bootrom_l.mmi -data bootrom_l.mem \
	-bit bootrom_h.bit -proc i_bootrom -out bootrom_l.bit -force
echo "update i_bootrom_h_l bit file"
rm bootrom_h.bit
mv bootrom_l.bit bootrom.bit
