#include<bits/stdc++.h>

int room[9] = {0,120,40,85,50,100,140,70,100};
bool emp[9];
bool flag = 0;

int main(){
    int a,b,c,d;
    std::cin>>a>>b>>c>>d;
    for(int i=1;i<=8;i++){
        if(emp[i]==0&&a<=room[i]){
            emp[i]=1;
            for(int j=1;j<=8;j++){
                if(emp[j]==0&&b<=room[j]){
                    emp[j]=1;
                    for(int k=1;k<=8;k++){
                        if(emp[k]==0&&c<=room[k]){
                            emp[k]=1;
                            for(int p=1;p<=8;p++){
                                if(emp[p]==0&&d<=room[p]){
                                    flag=1;
                                    printf("%d %d %d %d\n",i,j,k,p);
                                }
                            }
                            emp[k]=0;
                        }
                    }
                    emp[j]=0;  
                }
            }
            emp[i]=0;
        }
    }
    if(flag==0) std::cout<<"-1";
    return 0;
}