module vote(
    input A,
    input B,
    input C,

    output V
);

    assign V = (A & B) | (A & C) | (B & C);

endmodule