module mips_test (

);
    reg clk, reset;
    mips U0(clk, reset);

    parameter PERIOD = 10;

    always begin
        clk = 1'b0;
        # (PERIOD/2) clk = ~clk;
        # (PERIOD/2) ; // do nothing
    end

    initial begin
    reset = 1;
    #7.5
    reset = 0;
    end
endmodule
