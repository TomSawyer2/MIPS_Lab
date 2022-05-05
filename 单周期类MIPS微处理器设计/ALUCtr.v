`timescale 1ns / 1ps

module ALUCtr (
    input [1:0] ALUOp,
    input [5:0] Funct,
    output reg [3:0] ALUCtr
);
    // ����ALUOp��Funct��ֵƴ�ӳ�ALU�Ŀ����ź�
    always @(ALUOp or Funct) begin
        casex ({ALUOp, Funct})
            8'b00xxxxxx: ALUCtr[3:0] = 4'b0010;// lw, sw
            8'b01xxxxxx: ALUCtr[3:0] = 4'b0110;// beq
            8'b10xx0000: ALUCtr[3:0] = 4'b0010;// add
            8'b10xx0010: ALUCtr[3:0] = 4'b0110;// sub
            // andi��ʵ���ǰ�һ���Ĵ�����ֵ��һ�����������棬������ͬһ�ײ���
            8'b10xx0100: ALUCtr[3:0] = 4'b0000;// and 
            8'b11xxxxxx: ALUCtr[3:0] = 4'b0000;// andi
            8'b10xx0101: ALUCtr[3:0] = 4'b0001;// or
            8'b10xx1010: ALUCtr[3:0] = 4'b0111;// slt
            default:     ALUCtr[3:0] = 4'b0000;// initial
        endcase
    end
endmodule