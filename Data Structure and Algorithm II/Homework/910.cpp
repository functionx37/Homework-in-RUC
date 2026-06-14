#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

long long gcd(long long a, long long b) {
    return b ? gcd(b, a % b) : a;
}

int main() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    
    long long ans = 0;
    // 对于每个右端点 r，维护 (gcd值, 最左端点) 的映射
    // 同一 gcd 只保留最左端点，因为长度越大权值越大
    map<long long, int> mp;  // gcd -> 最左端点
    
    for (int r = 0; r < n; r++) {
        map<long long, int> nmp;
        nmp[a[r]] = r;
        
        for (auto& [g, left] : mp) {
            long long ng = gcd(g, a[r]);
            if (nmp.find(ng) == nmp.end()) {
                nmp[ng] = left;
            } else {
                nmp[ng] = min(nmp[ng], left);
            }
        }
        
        mp = nmp;
        
        for (auto& [g, left] : mp) {
            long long len = r - left + 1;
            ans = max(ans, len * g);
        }
    }
    
    cout << ans << "\n";
    return 0;
}
