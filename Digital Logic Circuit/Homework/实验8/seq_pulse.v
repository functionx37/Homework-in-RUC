module seq_pulse (
    input      clk,
    output reg [7:0] out
);

initial begin
    out = 8'b0000_0001;
end

always @(posedge clk) begin
    out <= {out[6:0], out[7]};
end

endmodule