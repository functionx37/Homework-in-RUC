/* WARNING: Do not include any other libraries here,
 * otherwise you will get an error while running test.py
 * You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 *
 * Using printf will interfere with our script capturing the execution results.
 * At this point, you can only test correctness with ./btest.
 * After confirming everything is correct in ./btest, remove the printf
 * and run the complete tests with test.py.
 */

/*
 * bitAnd - x & y using only ~ and |
 * Example: bitAnd(4, 5) = 4
 * Legal ops: ~ |
 * Max ops: 7
 * Difficulty: 1
 */
int bitAnd(int x, int y) {
    return ~(~x | ~y);  // 德摩根律
}

/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    return ~(x & y) & ~(~x & ~y);  // 都为1前者为0，都为0后者为0
}

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    if (!x & !y)  // 都为0
        return 1;
    if (!x ^ !y)  // 一者为0
        return 0;
    if ((x ^ y) >> 31)  // 反号为 -1
        return 0;
    return 1;
}

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Note: You may assume that v > 0
 *   Example: logtwo(32) = 5
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    // 即求最高位1的位置
    int temp = ((v >> 16) > 0) << 4;  // 前半部分判断高16位是否有1
    int rst = temp;                   // 若有 rst += 16
    v = v >> temp;                    // 若有扔去v后16位，将前16位移到后16位

    temp = ((v >> 8) > 0) << 3;  // 以下同理
    rst |= temp;                 // |= 相当于 +=
    v = v >> temp;

    temp = ((v >> 4) > 0) << 2;
    rst |= temp;
    v = v >> temp;

    temp = ((v >> 2) > 0) << 1;
    rst |= temp;
    v = v >> temp;

    temp = (v >> 1) > 0;
    rst |= temp;

    return rst;
}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
    n = n << 3;  // 一个byte8位，乘以8
    m = m << 3;
    int p = (x >> n) & 0xFF;  // 提取对应位
    int q = (x >> m) & 0xFF;
    int xor = p ^ q;  // 利用异或构建差值实现交换 x ^ (x ^ y) = x ^ x ^ y = 0 ^ y = y
    x = x ^ (xor << n) ^ (xor << m);
    return x;
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
unsigned reverse(unsigned v) {
    unsigned rst = 0;
    int cnt = 32;
    while (cnt--) {
        rst = (rst << 1) | (v & 1);  // 逐步提取v最低位
        v >>= 1;
    }
    return rst;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */
int logicalShift(int x, int n) {
    return (x >> n) & (0xFFFFFFFF >> n);  // 把算术右移得到的1全部消掉即可
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
int leftBitCount(int x) {
    int temp = (!~(x >> 16)) << 4;  // 与 logtwo 完全相同的二分思路
    int rst = temp;
    x = x << temp;

    temp = (!~(x >> 24)) << 3;
    rst += temp;
    x = x << temp;

    temp = (!~(x >> 28)) << 2;
    rst += temp;
    x = x << temp;

    temp = (!~(x >> 30)) << 1;
    rst += temp;
    x = x << temp;

    temp = !~(x >> 31);
    rst += temp;
    x = x << temp;

    temp = !~(x >> 31);  // 与 logtwo 不同的是还需要检查第32位
    rst += temp;         // 不能使用 |= 代替此处的 += ，因为 31 | 1 = 31

    return rst;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned float_i2f(int x) {
    if (x) {
        unsigned rst = x & 0x80000000;  // 符号位部分
        if (rst) {
            x = -x;  // 获得绝对值
        }
        int exp = 0;
        unsigned temp = x;  // hack x == 0x80000000 ，所以要用unsigned
        while (temp >>= 1) {
            ++exp;
        }
        rst = rst + ((exp + 127) << 23);       // 指数部分
        unsigned frac = x & ((1 << exp) - 1);  // 隐藏首1
        int k = exp - 23;
        if (k > 0) {                          // 处理舍入规则
            int half = 1 << (k - 1);          // 中间点
            int dst = frac & ((1 << k) - 1);  // 舍弃位
            frac >>= k;
            int last = frac & 1;
            if ((dst > half) | ((dst == half) & last)) {  // 超过一半或者末位为奇数
                ++frac;                                   // 进位
            }
        } else {
            frac = frac << (-k);  // 补足23位
        }
        rst = rst + frac;
        return rst;
    }
    return 0;  // hack x == 0
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatScale2(unsigned uf) {
    int exp = uf & 0x7F800000;
    if (exp == 0x7F800000)  // 处理指数部分全为1的数（Inf 和 NaN）
        return uf;
    if (exp == 0)
        return uf + (uf & 0x007FFFFF);  // 处理指数部分全为0的数（±0和极小的数）
    return uf + 0x00800000;             // 直接指数 + 1
}

/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer.
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    int sgn = uf2 & 0x80000000;                   // 获取符号位
    int exp = ((uf2 & 0x7FF00000) >> 20) - 1023;  // 获取指数部分
    if (exp < 0)                                  // 下溢
        return 0;
    if (exp > 30)  // 上溢
        return 0x80000000;
    int frac = (uf2 & 0x000FFFFF) << 10;  // 获取最高20位尾数
    frac |= (uf1 & 0xFFC00000) >> 22;     // 获取接下来10位尾数
    frac |= 0x70000000;                   // 加上隐藏1
    frac = frac >> (30 - exp);            // 舍掉小数部分
    if (sgn)
        return -frac;
    return frac;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
    if (x < -149)  // 过小非规格也无法表示
        return 0;
    else if (x >= 128)  // 过大超出上限
        x = 255;
    else {
        x = x + 127;  // 偏移量
        if (x <= 0)
            return 1 << (x + 22);  // 非规格能表示，指数为0改变尾数
    }
    return x << 23;  // 规格表示，尾数为改变指数即可
}