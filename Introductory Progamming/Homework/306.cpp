#include <bits/stdc++.h>

int main()
{
    int a[10005];
    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        std::cin>>a[i];
    }
    std::sort(a,a+n);
    int k=1;
    int max1=a[0];
    int max2=1;
    for(int i=1;i<n;i++){
        if(a[i]==a[i-1]){
            k++;
        }
        else{
            k=1;
        }
        if(k>=max2){
            max2=k;
            max1=a[i];
        }
    }
    if(max2>1){
        printf("%d %d", max1,max2);
    }
    else {
        printf("NO");
    }
    return 0;
}