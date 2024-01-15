#!/bin/bash
cat xlnx_coe_init.txt > xlnx_bootrom_h.coe
cut bootrom_64.hex -c 1-8 >> xlnx_bootrom_h.coe 
sed 's/$/&,/g' xlnx_bootrom_h.coe > xlnx_bootrom_h1.coe
sed '$ s/,$/;/' xlnx_bootrom_h1.coe > xlnx_bootrom_h2.coe
sed '1,2 s/,$//' xlnx_bootrom_h2.coe > xlnx_bootrom_h3.coe
mv xlnx_bootrom_h3.coe xlnx_bootrom_h.coe
rm xlnx_bootrom_h1.coe xlnx_bootrom_h2.coe
mv xlnx_bootrom_h.coe ../../xilinx/xlnx_blk_mem_bootrom_h/tcl
#
cat xlnx_coe_init.txt > xlnx_bootrom_l.coe
cut bootrom_64.hex -c 9-16 >> xlnx_bootrom_l.coe 
sed 's/$/&,/g' xlnx_bootrom_l.coe > xlnx_bootrom_l1.coe
sed '$ s/,$/;/' xlnx_bootrom_l1.coe > xlnx_bootrom_l2.coe
sed '1,2 s/,$//' xlnx_bootrom_l2.coe > xlnx_bootrom_l3.coe
mv xlnx_bootrom_l3.coe xlnx_bootrom_l.coe
rm xlnx_bootrom_l1.coe xlnx_bootrom_l2.coe
mv xlnx_bootrom_l.coe ../../xilinx/xlnx_blk_mem_bootrom_l/tcl
#
