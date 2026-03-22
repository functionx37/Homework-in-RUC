`timescale 1ns / 1ps

module alu_tb_fixed;
    // 输入信号
    reg [7:0] A;
    reg [7:0] B;
    reg [3:0] Op;
    
    // 输出信号
    wire [7:0] C;
    
    // 测试统计
    integer total_tests;
    integer passed_tests;
    integer failed_tests;
    integer score;
    
    // 实例化被测试的ALU
    alu uut (
        .A(A),
        .B(B),
        .Op(Op),
        .C(C)
    );
    
    // 初始化
    initial begin
        total_tests = 0;
        passed_tests = 0;
        failed_tests = 0;
        A = 8'b0;
        B = 8'b0;
        Op = 4'b0;
        score = 0;
    end
    
    // 主测试序列
    initial begin
        $display("Starting 8-bit ALU Comprehensive Test Suite");
        $display("");
        
        // 等待初始化完成
        #10;
        
        // ==================== 算术运算测试 ====================
        $display("");
        $display("===================================================");
        $display("ARITHMETIC TESTS - ADDITION (Op=1000)");
        $display("===================================================");
        
        // 加法测试1
        Op = 4'b1000;
        A = 8'd25;
        B = 8'd37;
        #10;
        if (C === 8'd62) begin
            $display("[PASS] Addition: 25 + 37 = 62");
            $display("      Expected: 3e (62), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Addition: 25 + 37 = 62");
            $display("      Expected: 3e (62), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 加法测试2
        A = 8'd0;
        B = 8'd100;
        #10;
        if (C === 8'd100) begin
            $display("[PASS] Addition: 0 + 100 = 100");
            $display("      Expected: 64 (100), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Addition: 0 + 100 = 100");
            $display("      Expected: 64 (100), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 加法测试3
        A = 8'd200;
        B = 8'd100;
        #10;
        if (C === 8'd44) begin
            $display("[PASS] Addition Overflow: 200 + 100 = 44");
            $display("      Expected: 2c (44), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Addition Overflow: 200 + 100 = 44");
            $display("      Expected: 2c (44), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        $display("");
        $display("===================================================");
        $display("ARITHMETIC TESTS - SUBTRACTION (Op=1001)");
        $display("===================================================");
        
        // 减法测试1
        Op = 4'b1001;
        A = 8'd50;
        B = 8'd30;
        #10;
        if (C === 8'd20) begin
            $display("[PASS] Subtraction: 50 - 30 = 20");
            $display("      Expected: 14 (20), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Subtraction: 50 - 30 = 20");
            $display("      Expected: 14 (20), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 减法测试2
        A = 8'd10;
        B = 8'd25;
        #10;
        if (C === 8'd241) begin
            $display("[PASS] Subtraction Negative: 10 - 25 = -15");
            $display("      Expected: e1 (225), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Subtraction Negative: 10 - 25 = -15");
            $display("      Expected: e1 (225), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 减法测试3
        A = 8'd100;
        B = 8'd100;
        #10;
        if (C === 8'd0) begin
            $display("[PASS] Subtraction Equal: 100 - 100 = 0");
            $display("      Expected: 00 (0), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Subtraction Equal: 100 - 100 = 0");
            $display("      Expected: 00 (0), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // ==================== 移位运算测试 ====================
        $display("");
        $display("===================================================");
        $display("SHIFT TESTS - LOGICAL LEFT SHIFT (Op=0000)");
        $display("===================================================");
        
        // 左移测试1
        Op = 4'b0000;
        A = 8'd1;
        B = 8'b00001010;
        #10;
        if (C === 8'b00010100) begin
            $display("[PASS] Left Shift 1: 10 << 1 = 20");
            $display("      Expected: 14 (20), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Left Shift 1: 10 << 1 = 20");
            $display("      Expected: 14 (20), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 左移测试2
        A = 8'd3;
        B = 8'b00000001;
        #10;
        if (C === 8'b00001000) begin
            $display("[PASS] Left Shift 3: 1 << 3 = 8");
            $display("      Expected: 08 (8), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Left Shift 3: 1 << 3 = 8");
            $display("      Expected: 08 (8), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 左移测试3
        A = 8'd4;
        B = 8'b00010000;
        #10;
        if (C === 8'b00000000) begin
            $display("[PASS] Left Shift Overflow: 16 << 4 = 0");
            $display("      Expected: 00 (0), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Left Shift Overflow: 16 << 4 = 0");
            $display("      Expected: 00 (0), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        $display("");
        $display("===================================================");
        $display("SHIFT TESTS - LOGICAL RIGHT SHIFT (Op=0010)");
        $display("===================================================");
        
        // 右移测试1
        Op = 4'b0010;
        A = 8'd1;
        B = 8'b00100000;
        #10;
        if (C === 8'b00010000) begin
            $display("[PASS] Logical Right Shift 1: 32 >> 1 = 16");
            $display("      Expected: 10 (16), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Logical Right Shift 1: 32 >> 1 = 16");
            $display("      Expected: 10 (16), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 右移测试2
        A = 8'd2;
        B = 8'b11110000;
        #10;
        if (C === 8'b00111100) begin
            $display("[PASS] Logical Right Shift 2: 240 >> 2 = 60");
            $display("      Expected: 3c (60), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Logical Right Shift 2: 240 >> 2 = 60");
            $display("      Expected: 3c (60), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 右移测试3
        A = 8'd4;
        B = 8'b10000000;
        #10;
        if (C === 8'b00001000) begin
            $display("[PASS] Logical Right Shift 4: 128 >> 4 = 8");
            $display("      Expected: 08 (8), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Logical Right Shift 4: 128 >> 4 = 8");
            $display("      Expected: 08 (8), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // ==================== 逻辑运算测试 ====================
        $display("");
        $display("===================================================");
        $display("LOGIC TESTS - BITWISE AND (Op=1100)");
        $display("===================================================");
        
        // 与运算测试1
        Op = 4'b1100;
        A = 8'b11110000;
        B = 8'b11001100;
        #10;
        if (C === 8'b11000000) begin
            $display("[PASS] Bitwise AND: F0 & CC = C0");
            $display("      Expected: c0 (192), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Bitwise AND: F0 & CC = C0");
            $display("      Expected: c0 (192), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 与运算测试2
        A = 8'b00001111;
        B = 8'b11011010;
        #10;
        if (C === 8'b00001010) begin
            $display("[PASS] Bitwise AND Mask: 0F & DA = 0A");
            $display("      Expected: 0a (10), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Bitwise AND Mask: 0F & DA = 0A");
            $display("      Expected: 0a (10), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 与运算测试3
        A = 8'b00000000;
        B = 8'b11111111;
        #10;
        if (C === 8'b00000000) begin
            $display("[PASS] Bitwise AND Clear: 00 & FF = 00");
            $display("      Expected: 00 (0), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Bitwise AND Clear: 00 & FF = 00");
            $display("      Expected: 00 (0), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        $display("");
        $display("===================================================");
        $display("LOGIC TESTS - BITWISE OR (Op=1101)");
        $display("===================================================");
        
        // 或运算测试1
        Op = 4'b1101;
        A = 8'b10101010;
        B = 8'b01010101;
        #10;
        if (C === 8'b11111111) begin
            $display("[PASS] Bitwise OR: AA | 55 = FF");
            $display("      Expected: ff (255), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Bitwise OR: AA | 55 = FF");
            $display("      Expected: ff (255), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 或运算测试2
        A = 8'b00001111;
        B = 8'b10100000;
        #10;
        if (C === 8'b10101111) begin
            $display("[PASS] Bitwise OR Set: 0F | A0 = AF");
            $display("      Expected: af (175), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Bitwise OR Set: 0F | A0 = AF");
            $display("      Expected: af (175), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // 或运算测试3
        A = 8'b11111111;
        B = 8'b00000000;
        #10;
        if (C === 8'b11111111) begin
            $display("[PASS] Bitwise OR Identity: FF | 00 = FF");
            $display("      Expected: ff (255), Got: %h (%0d)", C, C);
            passed_tests = passed_tests + 1;
        end else begin
            $display("[FAIL] Bitwise OR Identity: FF | 00 = FF");
            $display("      Expected: ff (255), Got: %h (%0d)", C, C);
            failed_tests = failed_tests + 1;
        end
        total_tests = total_tests + 1;
        
        // ==================== 显示最终分数 ====================
        #10;
        
        // 直接在这里计算和显示分数，不使用函数
        begin
            $display("");
            $display("===================================================");
            $display("FINAL TEST RESULTS");
            $display("===================================================");
            $display("Total Tests: %0d", total_tests);
            $display("Passed Tests: %0d", passed_tests);
            $display("Failed Tests: %0d", failed_tests);
            
            if (total_tests > 0) begin
                score = (passed_tests * 100.0) / total_tests;
                $display("Score: %.1f%%", score);
                
                if (score == 100.0) begin
                    $display("PERFECT! All tests passed!");
                end else if (score >= 90.0) begin
                    $display("EXCELLENT! Almost perfect!");
                end else if (score >= 80.0) begin
                    $display("GOOD! Most functions work!");
                end else if (score >= 70.0) begin
                    $display("FAIR! Needs some improvements!");
                end else if (score >= 60.0) begin
                    $display("PASS! Needs significant work!");
                end else begin
                    $display("FAIL! Redesign needed!");
                end
            end
            $display("===================================================");
        end
        
        // 结束仿真
        #100;
        $display("Simulation completed at time %0t", $time);
        $stop;
    end
    
    // 可选：波形记录
    initial begin
        $dumpfile("alu_wave_fixed.vcd");
        $dumpvars(0, alu_tb_fixed);
    end
    
endmodule