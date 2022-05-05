`timescale 1ns/1ps

module RegFile_test (
    output [31:0] RsData,
    output [31:0] RtData
);
    reg clk;
    reg reset;
    reg regWriteEn;
    reg [4:0] RsAddr;
    reg [4:0] RtAddr;
    reg [4:0] regWriteAddr;
    reg [31:0] regWriteData;

    parameter PERIOD = 10;

    always begin
        clk = 1'b0;
        #(PERIOD/2) clk = 1'b1;
        #(PERIOD/2) ;  
    end

    initial begin
    reset = 1;
    RsAddr = 5'h0;
    RtAddr = 5'h0;
    #15

    reset = 0;
    #30

    regWriteEn = 1;
    regWriteAddr = 5'h03;
    regWriteData = 32'h5aa5;
    #20

    RsAddr = 5'h03;
    RtAddr = 5'h03;
    end

    RegFile S2(clk,reset,regWriteEn,RsAddr,RtAddr,regWriteAddr,regWriteData,RsData,RtData);
 
endmodule