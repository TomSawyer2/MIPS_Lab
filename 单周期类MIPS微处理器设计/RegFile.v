`timescale 1ns / 1ps

module RegFile (
    input         clk,
    input         reset,
    input         RegWr,// 寄存器写信号
    input  [4:0]  RsAddr,
    input  [4:0]  RtAddr,
    input  [4:0]  WrAddr,// 寄存器写地址
    input  [31:0] DataIn,// 寄存器写数据
    output [31:0] RsData,
    output [31:0] RtData
);

// 一共32个寄存器，每个寄存器32位，共32×32
reg [31:0] regs[0:31];

// 取出数据
assign RsData = (RsAddr == 5'b0) ? 32'b0 : regs[RsAddr]; 
assign RtData = (RtAddr == 5'b0) ? 32'b0 : regs[RtAddr]; 

integer i;
always @(posedge clk) begin
    // 存入数据（不是重置状态且写入使能为高电平）
    if(!reset & RegWr) begin
        regs[WrAddr] = DataIn;
    end
    
    // 重置寄存器的数据
    else if(reset) begin
        for (i = 0; i < 32; i = i + 1) begin
            regs[i] = i;
        end
    end
end
endmodule