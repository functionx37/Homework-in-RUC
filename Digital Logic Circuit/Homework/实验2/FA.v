module full_adder_3bits(
    input [2:0] A,
    input [2:0] B,
    input cin,

    output [2:0] sum,
    output cout
);

    wire G0, G1, G2;
    wire P0, P1, P2;

    assign G0 = A[0] & B[0];
    assign P0 = A[0] ^ B[0];
    assign G1 = A[1] & B[1];
    assign P1 = A[1] ^ B[1];
    assign G2 = A[2] & B[2];
    assign P2 = A[2] ^ B[2];

    wire c1, c2;
    assign c1  = G0 | (P0 & cin);
    assign c2  = G1 | (P1 & G0) | (P1 & P0 & cin);
    assign cout = G2 | (P2 & G1) | (P2 & P1 & G0) | (P2 & P1 & P0 & cin);

    assign sum[0] = P0 ^ cin;
    assign sum[1] = P1 ^ c1;
    assign sum[2] = P2 ^ c2;

endmodule