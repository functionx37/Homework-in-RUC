module sequential (
    input  CP,
    input  X,
    output Y,
    output reg Q0,
    output reg Q1
);

always @(posedge CP) begin
    Q0 <= ~Q0;
    Q1 <= Q1 ^ (X ^ Q0);
end

assign Y = ~X | Q1;

endmodule