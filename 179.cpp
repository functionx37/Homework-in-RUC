#include<bits/stdc++.h>

int main(){
    int n;
    std::cin>>n;
    for(int i=1;i<=2*n-1;i++){
        if(i<=n){
            if(i==1){
                for(int j=1;j<n;j++){
                    printf(" ");
                }
                for(int j=0;j<n;j++){
                    printf("*");
                }
            }
            else{
                for(int j=i;j<n;j++){
                    printf(" ");
                }
                printf("*");
                for(int j=0;j<n+2*(i-2);j++){
                    printf(" ");
                }
                printf("*");
            }
        }
        else{
            if(i==2*n-1){
                for(int j=1;j<n;j++){
                    printf(" ");
                }
                for(int j=0;j<n;j++){
                    printf("*");
                }
            }
            else{
                for(int j=2*n-i;j<n;j++){
                    printf(" ");
                }
                printf("*");
                for(int j=0;j<n+2*(2*n-i-2);j++){
                    printf(" ");
                }
                printf("*");
            }
        }
        printf("\n");
    }
    return 0;
}    