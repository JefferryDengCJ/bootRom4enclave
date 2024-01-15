/* Copyright 2018 ETH Zurich and University of Bologna.
 * Copyright and related rights are licensed under the Solderpad Hardware
 * License, Version 0.51 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 * http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
 * or agreed to in writing, software, hardware and materials distributed under
 * this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * File: $filename.v
 *
 * Description: Auto-generated bootrom
 */

// Auto-generated code
module bootrom (
   input  logic         clk_i,
   input  logic         req_i,
   input  logic [63:0]  addr_i,
   output logic [63:0]  rdata_o
);
  wire [31:0] rdata_o_h;
  wire [31:0] rdata_o_l;

  xlnx_blk_mem_bootrom_h xlnx_blk_mem_bootrom_h(
    .clka            (clk_i),
    .ena             (req_i),
    .addra           (addr_i[13:3]),
    .douta           (rdata_o_h)
  );

  xlnx_blk_mem_bootrom_l xlnx_blk_mem_bootrom_l(
    .clka            (clk_i),
    .ena             (req_i),
    .addra           (addr_i[13:3]),
    .douta           (rdata_o_l)
  );
  assign rdata_o = {rdata_o_h, rdata_o_l};

endmodule
