#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// 大整数加法
string add(string a, string b) {
    if (a.length() < b.length()) swap(a, b);
    int n = a.length(), m = b.length();
    string res = "";
    int carry = 0;
    for (int i = 0; i < n; i++) {
        int sum = (a[n - 1 - i] - '0') + (i < m ? b[m - 1 - i] - '0' : 0) + carry;
        res += char(sum % 10 + '0');
        carry = sum / 10;
    }
    if (carry) res += char(carry + '0');
    reverse(res.begin(), res.end());
    return res;
}

// 大整数减法 (a >= b)
string sub(string a, string b) {
    int n = a.length(), m = b.length();
    string res = "";
    int borrow = 0;
    for (int i = 0; i < n; i++) {
        int x = a[n - 1 - i] - '0' - borrow;
        int y = i < m ? b[m - 1 - i] - '0' : 0;
        if (x < y) x += 10, borrow = 1;
        else borrow = 0;
        res += char(x - y + '0');
    }
    while (res.length() > 1 && res.back() == '0') res.pop_back();
    reverse(res.begin(), res.end());
    return res;
}

// 大整数乘 10^k
string shift(string a, int k) {
    return a + string(k, '0');
}

// Karatsuba 乘法
string karatsuba(string x, string y) {
    int n = x.length(), m = y.length();
    if (n == 0 || m == 0) return "0";
    
    // 补齐到相同长度，且为 2 的幂
    int len = max(n, m);
    while (len & (len - 1)) len++;
    while ((int)x.length() < len) x = "0" + x;
    while ((int)y.length() < len) y = "0" + y;
    
    // 小规模直接乘
    if (len <= 4) {
        long long a = stoll(x), b = stoll(y);
        return to_string(a * b);
    }
    
    int half = len / 2;
    string xl = x.substr(0, half), xr = x.substr(half);
    string yl = y.substr(0, half), yr = y.substr(half);
    
    string p1 = karatsuba(xl, yl);
    string p2 = karatsuba(xr, yr);
    string p3 = karatsuba(add(xl, xr), add(yl, yr));
    string mid = sub(sub(p3, p1), p2);
    
    string res = add(add(shift(p1, len), shift(mid, half)), p2);
    
    // 去除前导零
    int i = 0;
    while (i < (int)res.length() && res[i] == '0') i++;
    return i == (int)res.length() ? "0" : res.substr(i);
}

int main() {
    string a, b;
    cin >> a >> b;
    
    // 去除前导零
    int i = 0;
    while (i < (int)a.length() && a[i] == '0') i++;
    a = (i == (int)a.length()) ? "0" : a.substr(i);
    i = 0;
    while (i < (int)b.length() && b[i] == '0') i++;
    b = (i == (int)b.length()) ? "0" : b.substr(i);
    
    if (a == "0" || b == "0") {
        cout << "0\n";
        return 0;
    }
    
    cout << karatsuba(a, b) << "\n";
    return 0;
}
