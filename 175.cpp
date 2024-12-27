#include<bits/stdc++.h>

int main(){
    int n,m;
    std::cin>>n>>m;
    char a[26];
    for(int i=0;i<26;i++){
        a[i]='A'+i;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            std::cout<<a[j];
        }
        std::cout<<std::endl;
        for(int k=25;k>0;k--){
            a[k]=a[k-1];
        }
        a[0]='A'+i+1;
    }
    return 0;
}    