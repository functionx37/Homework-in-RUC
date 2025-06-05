#include <bits/stdc++.h>

int goal[21];
long long int tot = 0;
int n;

long long jie(int k){
    long long ans=1;
    while(k>0){
        ans*=k;k--;
    }
    return ans;
}

void check(int k){
    if(k==n){
        tot++;
        return;
    }
    tot += (goal[k]-1) * jie(n-k);
    for(int i=k;i<=n;i++){
        if(goal[i]>goal[k])
            goal[i]--;
    }
}

int main()
{
    std::cin>>n;
    for(int i=1;i<=n;i++){
        std::cin>>goal[i];
    }
    for(int i=1;i<=n;i++){
        check(i);
    }
    std::cout<<tot;
    return 0;
}