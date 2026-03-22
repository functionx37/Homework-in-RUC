#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;

inline int read() {
    int x = 0, f = 1;
    char c = getchar();
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = getchar();
    }
    return x * f;
}

// 基数排序 (LSD, 8位一组，处理有符号int)
void radix_sort(vector<int>& a) {
    int n = a.size();
    vector<int> b(n);
    unsigned int* ua = (unsigned int*)a.data();
    unsigned int* ub = (unsigned int*)b.data();
    
    for (int shift = 0; shift < 32; shift += 8) {
        int cnt[256] = {};
        for (int i = 0; i < n; i++)
            cnt[(ua[i] >> shift) & 255]++;
        for (int i = 1; i < 256; i++)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) {
            int d = (ua[i] >> shift) & 255;
            ub[--cnt[d]] = ua[i];
        }
        for (int i = 0; i < n; i++) ua[i] = ub[i];
    }
}

int main() {
    int n = read(), k = read();
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = read();
    
    // 符号位翻转，使有符号数按无符号序排列后即有序
    const unsigned int MSB = 1u << 31;
    for (int i = 0; i < n; i++)
        a[i] ^= (int)MSB;
    
    radix_sort(a);
    
    for (int i = 0; i < n; i++)
        a[i] ^= (int)MSB;
    
    printf("%d\n", a[k - 1]);
    return 0;
}
