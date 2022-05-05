`timescale 1ns / 1ps

module ALU (
    input signed [31:0] In1,
    input signed [31:0] In2,  
    input [3:0] ALUCtr,
    output reg [31:0] Res,
    output reg Zero
);

// 根据符号数In1、In2以及对应的操作名称ALUCtr执行对应操作

always @(In1 or In2 or ALUCtr) begin
    case (ALUCtr)
        4'b0110://sub
            begin
            Res = In1 - In2; 
            Zero = (Res == 0) ? 1 : 0;
            end

        4'b0010://add
            begin
            Res = In1 + In2; 
            Zero = 0;
            end

        4'b0000://and和andi
            begin
            Res = In1 & In2; 
            Zero = 0;
            end

        4'b0001://or
            begin
            Res = In1 | In2; 
            Zero = 0;
            end

        4'b0111://slt
            begin
            Res = (In1 < In2) ? 1 : 0; 
            Zero = 0;
            end

        default: 
            begin
            Res = 0;
            Zero = 0;
            end
    endcase
end  
endmodule
