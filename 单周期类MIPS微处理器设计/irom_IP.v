`timescale 1ns / 1ps

module irom_IP(
    input clk,
    input [4:0] addr,
    output [31:0] instr
    );
    reg[31:0] regs [0:31];
    assign instr = regs[addr];
    initial
    $readmemh("locate", regs, 0, 11);
endmodule
