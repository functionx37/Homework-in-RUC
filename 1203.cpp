#include <bits/stdc++.h>

int n;
unsigned long long int hashkey[105];

int get(){
    char x;
    std::cin>>x;
    if(x=='*') return 0;
    if(x=='k') return 1;
    if(x=='q') return 2;
    if(x=='r') return 3;
    if(x=='b') return 4;
    if(x=='n') return 5;
    if(x=='p') return 6;
    if(x=='K') return 7;
    if(x=='Q') return 8;
    if(x=='R') return 9;
    if(x=='B') return 10;
    if(x=='N') return 11;
    if(x=='P') return 12;
    return -1;
}

void hs(int k){
    unsigned long long int x = 0;
    for(int i = 0;i<64;i++){
        x *= 13;
        x += get(); 
    }
    hashkey[k] = x;
    int cnt = 1;
    for(int i=0;i<k;i++){
        if(hashkey[i]==x)
            cnt++;
    }
    printf("%d\n",cnt);
}

int main()
{
    std::cin>>n;
    for(int i=0;i<n;i++){
        hs(i);
    }
    return 0;
}