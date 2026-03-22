/*
请注意，你的代码不能出现任何 int/short/char/float/double/auto 等局部变量/函数传参，我们仅允许使用 reg 定义的寄存器变量。
其中 reg 等价于一个 int。

你不能自己申请额外的内存，即不能使用 new/malloc，作为补偿我们传入了一段 buffer，大小为 BUFFER_SIZE = 64，你可以视情况使用。

我们的数组按照 A, B, C, buffer 的顺序在内存上连续紧密排列，且 &A = 0x30000000（这是模拟的设定，不是 A 的真实地址）

如果你需要以更自由的方式访问内存，你可以以相对 A 的方式访问，比如 A[100]，用 *(0x30000000) 是无法访问到的。

如果你有定义常量的需求（更严谨的说法是，你想定义的是汇编层面的立即数，不应该占用寄存器），请参考下面这种方式使用宏定义来完成。
*/

#include "cachelab.h"

#define m case0_m
#define n case0_n
#define p case0_p

// 我们用这个 2*2*2 的矩阵乘法来演示寄存器是怎么被分配的
void gemm_case0(ptr_reg A, ptr_reg B, ptr_reg C, ptr_reg buffer)
{ // allocate 0 1 2 3
    for (reg i = 0; i < m; ++i)
    { // allocate 4
        for (reg j = 0; j < p; ++j)
        {                 // allocate 5
            reg tmpc = 0; // allocate 6
            for (reg k = 0; k < n; ++k)
            {                            // allocate 7
                reg tmpa = A[i * n + k]; // allocate 8
                reg tmpb = B[k * p + j]; // allocate 9
                tmpc += tmpa * tmpb;
            } // free 9 8
            // free 7
            C[i * p + j] = tmpc;
        } // free 6
        // free 5
    }
    // free 4
} // free 3 2 1 0

#undef m
#undef n
#undef p

#define m case1_m
#define n case1_n
#define p case1_p

void gemm_case1(ptr_reg A, ptr_reg B, ptr_reg C, ptr_reg buffer)
{
    // 16x16x16 矩阵乘法
    //
    // Cache 冲突分析：
    // A 和 B 相差 1024 字节 = 64 个 cache 行 = 2 倍 cache 容量
    // 所以 A[i] 和 B[i] 映射到相同的 cache 组
    //
    // 优化策略：调整 j 循环的遍历顺序
    // 对于每个 i 块，让 j 从 (i+8)%16 开始，错开 A 和 B 的 cache 映射

    for (reg i = 0; i < 16; i += 4) {
        // j 从偏移位置开始，绕一圈处理所有 j 块
        for (reg jj = 0; jj < 16; jj += 4) {
            // 计算实际的 j 值：(jj + i + 8) % 16
            // 这样当 i=0 时 j=8,12,0,4; i=4 时 j=12,0,4,8...
            reg j = (jj + i + 8) % 16;
            
            reg c00 = 0, c01 = 0, c02 = 0, c03 = 0;
            reg c10 = 0, c11 = 0, c12 = 0, c13 = 0;
            reg c20 = 0, c21 = 0, c22 = 0, c23 = 0;
            reg c30 = 0, c31 = 0, c32 = 0, c33 = 0;

            for (reg k = 0; k < 16; ++k) {
                reg a0 = A[(i + 0) * 16 + k];
                reg a1 = A[(i + 1) * 16 + k];
                reg a2 = A[(i + 2) * 16 + k];
                reg a3 = A[(i + 3) * 16 + k];

                reg b0 = B[k * 16 + (j + 0)];
                reg b1 = B[k * 16 + (j + 1)];
                reg b2 = B[k * 16 + (j + 2)];
                reg b3 = B[k * 16 + (j + 3)];

                c00 += a0 * b0; c01 += a0 * b1; c02 += a0 * b2; c03 += a0 * b3;
                c10 += a1 * b0; c11 += a1 * b1; c12 += a1 * b2; c13 += a1 * b3;
                c20 += a2 * b0; c21 += a2 * b1; c22 += a2 * b2; c23 += a2 * b3;
                c30 += a3 * b0; c31 += a3 * b1; c32 += a3 * b2; c33 += a3 * b3;
            }

            C[(i + 0) * 16 + (j + 0)] = c00; C[(i + 0) * 16 + (j + 1)] = c01;
            C[(i + 0) * 16 + (j + 2)] = c02; C[(i + 0) * 16 + (j + 3)] = c03;
            C[(i + 1) * 16 + (j + 0)] = c10; C[(i + 1) * 16 + (j + 1)] = c11;
            C[(i + 1) * 16 + (j + 2)] = c12; C[(i + 1) * 16 + (j + 3)] = c13;
            C[(i + 2) * 16 + (j + 0)] = c20; C[(i + 2) * 16 + (j + 1)] = c21;
            C[(i + 2) * 16 + (j + 2)] = c22; C[(i + 2) * 16 + (j + 3)] = c23;
            C[(i + 3) * 16 + (j + 0)] = c30; C[(i + 3) * 16 + (j + 1)] = c31;
            C[(i + 3) * 16 + (j + 2)] = c32; C[(i + 3) * 16 + (j + 3)] = c33;
        }
    }
}

#undef m
#undef n
#undef p

#define m case2_m
#define n case2_n
#define p case2_p

void gemm_case2(ptr_reg A, ptr_reg B, ptr_reg C, ptr_reg buffer)
{
    // 32x32x32 矩阵乘法
    //
    // Cache 冲突分析：
    // A: 1024 int = 4096 字节, B 紧随其后
    // A 和 B 间隔 4096 字节 = 256 个 cache 行
    // 256 % 32 = 0，所以 A[i] 和 B[i] 映射到相同 cache 组
    //
    // 优化策略：j 偏移

    for (reg i = 0; i < 32; i += 4) {
        for (reg jj = 0; jj < 32; jj += 4) {
            // j 偏移：错开 A 和 B 的 cache 映射
            reg j = (jj + i + 16) % 32;
            
            reg c00 = 0, c01 = 0, c02 = 0, c03 = 0;
            reg c10 = 0, c11 = 0, c12 = 0, c13 = 0;
            reg c20 = 0, c21 = 0, c22 = 0, c23 = 0;
            reg c30 = 0, c31 = 0, c32 = 0, c33 = 0;

            for (reg k = 0; k < 32; ++k) {
                reg a0 = A[(i + 0) * 32 + k];
                reg a1 = A[(i + 1) * 32 + k];
                reg a2 = A[(i + 2) * 32 + k];
                reg a3 = A[(i + 3) * 32 + k];

                reg b0 = B[k * 32 + (j + 0)];
                reg b1 = B[k * 32 + (j + 1)];
                reg b2 = B[k * 32 + (j + 2)];
                reg b3 = B[k * 32 + (j + 3)];

                c00 += a0 * b0; c01 += a0 * b1; c02 += a0 * b2; c03 += a0 * b3;
                c10 += a1 * b0; c11 += a1 * b1; c12 += a1 * b2; c13 += a1 * b3;
                c20 += a2 * b0; c21 += a2 * b1; c22 += a2 * b2; c23 += a2 * b3;
                c30 += a3 * b0; c31 += a3 * b1; c32 += a3 * b2; c33 += a3 * b3;
            }

            C[(i + 0) * 32 + (j + 0)] = c00; C[(i + 0) * 32 + (j + 1)] = c01;
            C[(i + 0) * 32 + (j + 2)] = c02; C[(i + 0) * 32 + (j + 3)] = c03;
            C[(i + 1) * 32 + (j + 0)] = c10; C[(i + 1) * 32 + (j + 1)] = c11;
            C[(i + 1) * 32 + (j + 2)] = c12; C[(i + 1) * 32 + (j + 3)] = c13;
            C[(i + 2) * 32 + (j + 0)] = c20; C[(i + 2) * 32 + (j + 1)] = c21;
            C[(i + 2) * 32 + (j + 2)] = c22; C[(i + 2) * 32 + (j + 3)] = c23;
            C[(i + 3) * 32 + (j + 0)] = c30; C[(i + 3) * 32 + (j + 1)] = c31;
            C[(i + 3) * 32 + (j + 2)] = c32; C[(i + 3) * 32 + (j + 3)] = c33;
        }
    }
}

#undef m
#undef n
#undef p

#define m case3_m
#define n case3_n
#define p case3_p

void gemm_case3(ptr_reg A, ptr_reg B, ptr_reg C, ptr_reg buffer)
{
    for (reg i = 0; i < 28; i += 4) {
        for (reg j = 0; j < 25; j += 5) {
            reg c00 = 0, c01 = 0, c02 = 0, c03 = 0, c04 = 0;
            reg c10 = 0, c11 = 0, c12 = 0, c13 = 0, c14 = 0;
            reg c20 = 0, c21 = 0, c22 = 0, c23 = 0, c24 = 0;
            reg c30 = 0, c31 = 0, c32 = 0, c33 = 0, c34 = 0;
            
            for (reg k = 0; k < 35; ++k) {
                reg a0 = A[(i + 0) * 35 + k];
                reg a1 = A[(i + 1) * 35 + k];
                reg a2 = A[(i + 2) * 35 + k];
                reg a3 = A[(i + 3) * 35 + k];
                
                reg b0 = B[k * 29 + j + 0];
                reg b1 = B[k * 29 + j + 1];
                reg b2 = B[k * 29 + j + 2];
                reg b3 = B[k * 29 + j + 3];
                reg b4 = B[k * 29 + j + 4];
                
                c00 += a0 * b0; c01 += a0 * b1; c02 += a0 * b2; c03 += a0 * b3; c04 += a0 * b4;
                c10 += a1 * b0; c11 += a1 * b1; c12 += a1 * b2; c13 += a1 * b3; c14 += a1 * b4;
                c20 += a2 * b0; c21 += a2 * b1; c22 += a2 * b2; c23 += a2 * b3; c24 += a2 * b4;
                c30 += a3 * b0; c31 += a3 * b1; c32 += a3 * b2; c33 += a3 * b3; c34 += a3 * b4;
            }
            
            C[(i + 0) * 29 + j + 0] = c00; C[(i + 0) * 29 + j + 1] = c01; C[(i + 0) * 29 + j + 2] = c02;
            C[(i + 0) * 29 + j + 3] = c03; C[(i + 0) * 29 + j + 4] = c04;
            C[(i + 1) * 29 + j + 0] = c10; C[(i + 1) * 29 + j + 1] = c11; C[(i + 1) * 29 + j + 2] = c12;
            C[(i + 1) * 29 + j + 3] = c13; C[(i + 1) * 29 + j + 4] = c14;
            C[(i + 2) * 29 + j + 0] = c20; C[(i + 2) * 29 + j + 1] = c21; C[(i + 2) * 29 + j + 2] = c22;
            C[(i + 2) * 29 + j + 3] = c23; C[(i + 2) * 29 + j + 4] = c24;
            C[(i + 3) * 29 + j + 0] = c30; C[(i + 3) * 29 + j + 1] = c31; C[(i + 3) * 29 + j + 2] = c32;
            C[(i + 3) * 29 + j + 3] = c33; C[(i + 3) * 29 + j + 4] = c34;
        }
        
        {
            reg c00 = 0, c01 = 0, c02 = 0, c03 = 0;
            reg c10 = 0, c11 = 0, c12 = 0, c13 = 0;
            reg c20 = 0, c21 = 0, c22 = 0, c23 = 0;
            reg c30 = 0, c31 = 0, c32 = 0, c33 = 0;
            
            for (reg k = 0; k < 35; ++k) {
                reg a0 = A[(i + 0) * 35 + k];
                reg a1 = A[(i + 1) * 35 + k];
                reg a2 = A[(i + 2) * 35 + k];
                reg a3 = A[(i + 3) * 35 + k];
                reg b0 = B[k * 29 + 25];
                reg b1 = B[k * 29 + 26];
                reg b2 = B[k * 29 + 27];
                reg b3 = B[k * 29 + 28];
                
                c00 += a0 * b0; c01 += a0 * b1; c02 += a0 * b2; c03 += a0 * b3;
                c10 += a1 * b0; c11 += a1 * b1; c12 += a1 * b2; c13 += a1 * b3;
                c20 += a2 * b0; c21 += a2 * b1; c22 += a2 * b2; c23 += a2 * b3;
                c30 += a3 * b0; c31 += a3 * b1; c32 += a3 * b2; c33 += a3 * b3;
            }
            
            C[(i + 0) * 29 + 25] = c00; C[(i + 0) * 29 + 26] = c01; C[(i + 0) * 29 + 27] = c02; C[(i + 0) * 29 + 28] = c03;
            C[(i + 1) * 29 + 25] = c10; C[(i + 1) * 29 + 26] = c11; C[(i + 1) * 29 + 27] = c12; C[(i + 1) * 29 + 28] = c13;
            C[(i + 2) * 29 + 25] = c20; C[(i + 2) * 29 + 26] = c21; C[(i + 2) * 29 + 27] = c22; C[(i + 2) * 29 + 28] = c23;
            C[(i + 3) * 29 + 25] = c30; C[(i + 3) * 29 + 26] = c31; C[(i + 3) * 29 + 27] = c32; C[(i + 3) * 29 + 28] = c33;
        }
    }
    
    // i=28
    for (reg j = 0; j < 29; ++j) {
        reg c = 0;
        for (reg k = 0; k < 35; ++k) {
            reg a = A[28 * 35 + k];
            reg b = B[k * 29 + j];
            c += a * b;
        }
        C[28 * 29 + j] = c;
    }
}

#undef m
#undef n
#undef p