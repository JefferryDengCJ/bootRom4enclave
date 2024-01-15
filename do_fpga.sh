#!/bin/bash
cat xlnx_coe_init.txt > xlnx_bootrom_h.coe
cut bootrom_64.hex -c 1-8 >> xlnx_bootrom_h.coe 
sed -i 's/$/&,/g' xlnx_bootrom_h.coe
sed -i '$ s/,$/;/' xlnx_bootrom_h.coe
sed -i '1,2 s/,$//' xlnx_bootrom_h.coe
mv xlnx_bootrom_h.coe ../../xilinx/xlnx_blk_mem_bootrom_h/tcl
cut bootrom_64.hex -c 1-8 > bootrom_h.mem
sed -i '1i\@0' bootrom_h.mem
#
cat xlnx_coe_init.txt > xlnx_bootrom_l.coe
cut bootrom_64.hex -c 9-16 >> xlnx_bootrom_l.coe 
sed -i 's/$/&,/g' xlnx_bootrom_l.coe
sed -i '$ s/,$/;/' xlnx_bootrom_l.coe
sed -i '1,2 s/,$//' xlnx_bootrom_l.coe
mv xlnx_bootrom_l.coe ../../xilinx/xlnx_blk_mem_bootrom_l/tcl
cut bootrom_64.hex -c 9-16 > bootrom_l.mem
sed -i '1i\@0' bootrom_l.mem
#
