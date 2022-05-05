`timescale 1ns / 1ps

module ALU_test(
    output [31:0] Res,
    output Zero
    );
    reg [31:0] In1,In2;
    reg [3:0] ALUCtr;
    
    ALU S1(In1,In2,ALUCtr,Res, Zero);

    initial begin
        In1 = 32'hffff0000;
        In2 = 32'h00ffff00;
        ALUCtr=4'h2;
        #10
        ALUCtr=4'h6;
        #10
        ALUCtr=4'h0;
        #10
        ALUCtr=4'h1;
        #10
        ALUCtr=4'h7;
    end 
    
endmodule