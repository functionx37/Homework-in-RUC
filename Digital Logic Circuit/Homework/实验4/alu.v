module alu(
    input [7:0] A,
    input [7:0] B,
    input [3:0] Op,
    
    output reg [7:0] C
);

always @(*) begin
    case (Op)
        4'b1000: //ADDU
            C = A + B;
        4'b1001: //SUBU
            C = A - B;
        4'b0000: //SLL
            C = B << A[2:0];
        4'b0010: //SRL
            C = B >> A[2:0];
        4'b0011: //SRA
            C = $signed(B) >>> A[2:0];
        4'b1100: //AND
            C = A & B;
        4'b1101: //OR
             C = A | B;
        4'b1110: //XOR
            C = A ^ B;
        4'b1111: //NOR
            C = ~(A | B);
        default:
            C = 8'b00000000;
    endcase    
end

endmodule