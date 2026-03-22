module vending_machine (
    input             clk,
    input             rst,     
    input      [3:0]  coin,     
    input      [3:0]  price,         

    output reg        insufficient,
    output reg        take_drink,
    output reg [3:0]  change
);

always @(posedge clk or negedge rst) begin
    if (!rst) begin
        insufficient   <= 1'b0;
        take_drink     <= 1'b0;
        change  <= 4'd0;
    end else begin
        insufficient   <= 1'b0;
        take_drink     <= 1'b0;
        change  <= 4'd0;

        if (coin != 4'd0) begin
            if (coin < price) begin
                insufficient  <= 1'b1;
                change <= coin;
            end else if (coin == price) begin
                take_drink <= 1'b1;
            end else begin
                take_drink    <= 1'b1;
                change <= coin - price;
            end
        end
    end
end

endmodule