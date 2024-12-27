#include<bits/stdc++.h>

int save[6];

int main(){
    for(int i=0;i<5;i++){
        save[i] = i + 1;
    }
    int n;
    std::cin>>n;
    while(1){
        if(save[0]+save[1]*4+save[2]*6+save[3]*4+save[4]==n){
            for(int i=0;i<5;i++){
                std::cout<<save[i]<<" ";
            }
            break;
        }
        std::next_permutation(save,save+5);
    }
    return 0;
}