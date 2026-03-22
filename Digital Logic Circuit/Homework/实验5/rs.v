module rs (
    input      clr,   
    input      set,   

    input      clk,    
    input      s,       
    input      r,      

    output reg q,
    output     qn
);

assign qn = ~q;

always @(posedge clk or negedge clr or negedge set) begin

    if (!clr) begin
        q <= 1'b0;
    end 

    else if (!set) begin
        q <= 1'b1;
    end 

    else begin
        if (s && !r) begin
            q <= 1'b1;
        end
        else if (!s && r) begin
            q <= 1'b0;
        end
        else if (s && r) begin //非法输入
            q <= q; 
        end
        else begin
            q <= q;
        end
    end

end

endmodule