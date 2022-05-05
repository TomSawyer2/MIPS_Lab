`timescale 1ns / 1ps

module MainCtr (
    input [5:0] OpCode,// 对应Instr的26~31位
    output RegDst,
    output ALUSrc,
    output Mem2Reg,
    output RegWr,
    output MemWr,
    output B,
    output J, 
    output [1:0] ALUOp
);
    reg [8:0] outputtemp;
    
    // 信息解码
    assign RegDst = outputtemp[8];
    assign ALUSrc = outputtemp[7];  
    assign Mem2Reg = outputtemp[6];  
    assign RegWr = outputtemp[5];  
    assign MemWr = outputtemp[4];  
    assign B = outputtemp[3];  
    assign J = outputtemp[2];  
    assign ALUOp = outputtemp[1:0];
    
    // 根据传入的OpCode判断需要输出的各个信号，并进行编码
    always @(OpCode) begin
        case (OpCode)
            6'b000010: outputtemp = 9'bxxx0001xx;// j
            6'b000000: outputtemp = 9'b100100010;// R
            6'b100011: outputtemp = 9'b011100000;// lw
            6'b101011: outputtemp = 9'bx1x010000;// sw
            6'b000100: outputtemp = 9'bx0x001001;// beq
            6'b001100: outputtemp = 9'b010100011;// andi
            default:   outputtemp = 9'b000000000;// 默认全为0
        endcase
    end
endmodule