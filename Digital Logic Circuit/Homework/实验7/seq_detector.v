module seq_detector (
    input  wire clk,
    input  wire X,
    output reg  Y
);

    reg x_prev;           
    reg first_cycle = 1'b1;      

    always @(posedge clk) begin
        x_prev <= X;
        if (first_cycle) begin
            Y <= 1'b0;
            first_cycle <= 1'b0;
        end else begin
            Y <= (X == x_prev) ? 1'b1 : 1'b0;
        end
    end

endmodule