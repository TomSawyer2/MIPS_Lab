`timescale 1ns / 1ps

module dram_IP(
    input clk,
    input MemWR,
    input [4:0] addr,
    input [31:0] writedata,
    output [31:0] readdata    
    );
    reg[31:0] regs[0:31];
    assign readdata = regs[addr];
    always@(posedge clk)
        // 每个时钟下降沿都判断一次，如果需要写内存直接写
        if (MemWR) 
        regs[addr] <= writedata;
    
    // 初始化数据寄存器内部的数据，防止PC到达32时变为X
    integer i;
    initial
    for (i = 0; i < 32; i = i + 1)
    // Q:为什么不设置成i?
    // A:如果之后对第i个寄存器赋值为i没法观察到
    // Q:为什么不设置成0或者其他值？
    // A:同上，容易混淆，需要区分
    regs[i] = 32'haaaaaaaa; // 初始化寄存器
endmodule
