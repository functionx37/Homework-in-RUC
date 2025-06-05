#include <bits/stdc++.h>

int a[1005]={0};
int b[1005]={0};

int main()
{
    int n;
    int sum=0;
    std::cin>>n;
    for(int i=0;i<n;i++){
        std::cin>>a[i];
    }
    for(int i=0;i<n;i++){
        std::cin>>b[i];
    }
    std::sort(a,a+n);
    std::sort(b,b+n);
    int j=n;
    for(int i=n-1;i>=0;i--){
        for(;j>0;){
            j--;
            if(b[i]>a[j]){
                sum++;
                break;
            }
        }
    }
    std::cout<<sum;
    return 0;
}