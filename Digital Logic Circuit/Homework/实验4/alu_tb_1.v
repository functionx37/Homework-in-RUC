`timescale 1ns / 1ps

module alu_tb_detailed;
    reg [7:0] A;
    reg [7:0] B;
    reg [3:0] Op;
    wire [7:0] C;
    
    alu uut (.A(A), .B(B), .Op(Op), .C(C));
    
    integer total = 0;
    integer passed = 0;
    
    // 详细测试记录
    task automatic run_detailed_test;
        input [7:0] exp_result;
        input [255:0] description;
        input [7:0] test_A;
        input [7:0] test_B;
        input [3:0] test_Op;
        begin
            total = total + 1;
            A = test_A;
            B = test_B;
            Op = test_Op;
            #15;
            
            $write("测试%0d: %s -> ", total, description);
            if (C === exp_result) begin
                passed = passed + 1;
                $display("? 通过");
                $display("   输入: A=%h, B=%h, Op=%b", A, B, Op);
                $display("   输出: 期望=%h, 实际=%h", exp_result, C);
            end else begin
                $display("? 失败");
                $display("   输入: A=%h, B=%h, Op=%b", A, B, Op);
                $display("   输出: 期望=%h, 实际=%h", exp_result, C);
                $display("   二进制: 期望=%b, 实际=%b", exp_result, C);
            end
            $display("");
        end
    endtask
    
    initial begin
        $display("=================== ALU 详细测试报告 ===================");
        $display("");
        
        // 算术运算
        run_detailed_test(8'h1E, "10 + 20 = 30", 8'd10, 8'd20, 4'b1000);
        run_detailed_test(8'h0F, "30 - 15 = 15", 8'd30, 8'd15, 4'b1001);
        
        // 移位运算
        run_detailed_test(8'h34, "13 << 2 = 52", 8'd2, 8'd13, 4'b0000);
        run_detailed_test(8'h34, "208 >> 2 = 52", 8'd2, 8'd208, 4'b0010);
        run_detailed_test(8'hF4, "-48 >>> 2 = -12", 8'd2, 8'hD0, 4'b0011);
        
        // 逻辑运算
        run_detailed_test(8'h88, "0xAA & 0xCC = 0x88", 8'hAA, 8'hCC, 4'b1100);
        run_detailed_test(8'hEE, "0xAA | 0xCC = 0xEE", 8'hAA, 8'hCC, 4'b1101);
        run_detailed_test(8'h66, "0xAA ^ 0xCC = 0x66", 8'hAA, 8'hCC, 4'b1110);
        run_detailed_test(8'h11, "~(0xAA | 0xCC) = 0x11", 8'hAA, 8'hCC, 4'b1111);
        
        // 显示总结
        $display("=================== 测试总结 ===================");
        $display("?? 通过率: %0d/%0d (%.1f%%)", passed, total, (passed*100.0)/total);
        
        if (passed == total) begin
            $display("?? 恭喜！所有测试用例都通过了！");
            $display("?? 你的ALU设计完全正确！");
        end else begin
            $display("?? 有 %0d 个测试失败，请检查ALU实现", total - passed);
        end
        
        $stop;
    end
endmodule