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
        // ÿ��ʱ���½��ض��ж�һ�Σ������Ҫд�ڴ�ֱ��д
        if (MemWR) 
        regs[addr] <= writedata;
    
    // ��ʼ�����ݼĴ����ڲ������ݣ���ֹPC����32ʱ��ΪX
    integer i;
    initial
    for (i = 0; i < 32; i = i + 1)
    // Q:Ϊʲô�����ó�i?
    // A:���֮��Ե�i���Ĵ�����ֵΪiû���۲쵽
    // Q:Ϊʲô�����ó�0��������ֵ��
    // A:ͬ�ϣ����׻�������Ҫ����
    regs[i] = 32'haaaaaaaa; // ��ʼ���Ĵ���
endmodule
