`timescale 1ns / 1ps

module RegFile (
    input         clk,
    input         reset,
    input         RegWr,// �Ĵ���д�ź�
    input  [4:0]  RsAddr,
    input  [4:0]  RtAddr,
    input  [4:0]  WrAddr,// �Ĵ���д��ַ
    input  [31:0] DataIn,// �Ĵ���д����
    output [31:0] RsData,
    output [31:0] RtData
);

// һ��32���Ĵ�����ÿ���Ĵ���32λ����32��32
reg [31:0] regs[0:31];

// ȡ������
assign RsData = (RsAddr == 5'b0) ? 32'b0 : regs[RsAddr]; 
assign RtData = (RtAddr == 5'b0) ? 32'b0 : regs[RtAddr]; 

integer i;
always @(posedge clk) begin
    // �������ݣ���������״̬��д��ʹ��Ϊ�ߵ�ƽ��
    if(!reset & RegWr) begin
        regs[WrAddr] = DataIn;
    end
    
    // ���üĴ���������
    else if(reset) begin
        for (i = 0; i < 32; i = i + 1) begin
            regs[i] = i;
        end
    end
end
endmodule