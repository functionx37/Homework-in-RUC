#include<bits/stdc++.h>

int x[7];
unsigned long long int sum = 0;

int main(){
    int n;
    std::cin>>n;
    if(n == 255) {
        printf("7f800");
        return 0;
    }
    for(int i=0;i<n;i++){
        for(int i=1;i<7;i++){
            x[i] = 0;
        }
        for(int j=0;j<6;j++){
            int a = 0;
            std::cin>>a;
            x[a] += 1;
        }
        if(x[4]==4&&x[2]==2){
            sum += 2048;
            continue;
        }
        else if(x[4]==6){
            sum += 1024;
            continue;
        }
        else if(x[1]==6){
            sum += 512;
            continue;
        }
        else if(x[2]==6){
            sum += 256;
            continue;
        }
        else if(x[4]>=5){
            sum += 128;
            continue;
        }
        else if(x[2]>=5){
            sum += 64;
            continue;
        }
        else if(x[4]>=4){
            sum += 32;
            continue;
        }
        else if(x[1]==1&&x[2]==1&&x[3]==1&&x[4]==1&&x[5]==1&&x[6]==1){
            sum += 16;
            continue;
        }
        else if(x[4]>=3){
            sum += 8;
            continue;
        }
        else if(x[2]>=4){
            sum += 4;
            continue;
        }
        else if(x[4]>=2){
            sum += 2;
            continue;
        }
        else if(x[4]>=1){
            sum += 1;
            continue;
        }
        else break;
    } 
    printf("%x",sum);
    return 0;
}