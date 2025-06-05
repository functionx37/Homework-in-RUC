#include<bits/stdc++.h>

struct num {
    long long x;
    long long y;
    long long z;
    int ans;
} a[10];

int wei(long long k) {
    int count = 0;
    do {
        count++;
        k /= 10;
    } while (k != 0);
    return count;
}

void rev(int k) {
    long long x = a[k].y;
    long long q = 0;
    int nn = wei(a[k].y);
    for (int i = 0; i < nn; i++) {
        int p = x % 10;
        x /= 10;
        q += p * static_cast<long long>(std::pow(10, nn - i - 1));
    }
    a[k].z = q;
}

bool cmp(const num &a, const num &b) {
    if (a.ans > b.ans) return true;
    if (a.ans < b.ans) return false;
    if (a.y > b.y) return false;
    if (a.y < b.y) return true;
    if (a.x > b.x) return false;
    if (a.x < b.x) return true;
    return false;
}

int main(){
    for(int i=0;i<10;i++){
        std::cin>>a[i].x;
        a[i].y=a[i].x;
        rev(i);
        while(a[i].y!=a[i].z){
            a[i].ans++;
            a[i].y+=a[i].z;
            rev(i);
        }
    }
    std::sort(a,a+10,cmp);
    for(int i=0;i<10;i++){
        printf("%lld %d %lld\n",a[i].x,a[i].ans,a[i].y);
    }
    return 0;
}