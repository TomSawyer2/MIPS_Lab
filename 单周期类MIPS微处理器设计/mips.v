`timescale 1ns / 1ps

module mips (
    input clk, 
    input reset 
);

    wire [31:0] TempPC, JumpPC, MuxPC, SquencePC, BranchPC, Imm32, ImmL2, regWriteData, RsData, RtData, ALUin2, ALUres, memReadData, Instr;
    wire [4:0] regWriteAddr;
    wire [27:0] PsudeoPC;
    wire [1:0] ALUop;
    wire [3:0] ALUctr;
    wire BranchZ, regDst, ALUsrc, M2R, regWriteEn, zero, memWriteEn, B, J;
    reg [31:0] PC;

    assign PsudeoPC = {Instr[25:0], 2'b00};
    assign JumpPC = {SquencePC[31:28], PsudeoPC};
    assign TempPC = J ? JumpPC : MuxPC;
    assign MuxPC = BranchZ ? BranchPC : SquencePC;
    assign BranchPC = ImmL2 + SquencePC;
    assign ImmL2 = {Imm32[29:0], 2'b00};
    assign Imm32 = {Instr[15] ? 16'hffff : 16'h0000, Instr[15:0]};
    assign BranchZ = B & zero;
    assign regWriteAddr = regDst ? Instr[15:11] : Instr[20:16];
    assign ALUin2 = ALUsrc ? Imm32 : RtData;
    assign regWriteData = M2R ? memReadData : ALUres;
    assign SquencePC = PC + 4;
       
    always @(negedge clk) begin
        if(reset)
            PC <= 32'h0;
        else 
            PC <= TempPC;
    end

    ALU U0(RsData,ALUin2,ALUctr,ALUres,zero);
    // 需要时钟下降沿
    dram_IP U1(~clk, memWriteEn, ALUres[6:2], RtData, memReadData);
    irom_IP U2(clk, PC[6:2], Instr);

    RegFile U3(~clk, reset, regWriteEn, Instr[25:21], Instr[20:16], regWriteAddr, regWriteData, RsData, RtData);
    MainCtr U4(Instr[31:26], regDst, ALUsrc, M2R, regWriteEn, memWriteEn, B, J, ALUop);
    ALUCtr U5(ALUop, Instr[5:0], ALUctr);
    
endmodule