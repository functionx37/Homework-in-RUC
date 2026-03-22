module jk (
    input      clr,   
    input      set,   

    input      clk,
    input      j,
    input      k,

    output reg q,
    output     qn
);

assign qn = ~q;

always @(posedge clk or negedge clr or negedge set) begin
    if (!clr) begin
        q <= 1'b0;
    end else if (!set) begin
        q <= 1'b1;
    end else begin
        case ({j, k})
            2'b00: q <= q;       // 保持
            2'b01: q <= 1'b0;    // 复位
            2'b10: q <= 1'b1;    // 置位
            2'b11: q <= ~q;      // 翻转
        endcase
    end
end

endmodule