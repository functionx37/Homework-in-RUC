#include <bits/stdc++.h>

int main()
{
    int n;
    bool f=0;
    std::cin>>n;
    for(int x=1;x<51;x++){
        for(int y=1;y<17;y++){
            for(int z=8;z<100;z+=10){
                if(x!=y&&y!=z){
                    if((x>y&&x<z)||(x>z&&x<y)){
                        if(x%2==0){
                            if(y>=10||(y<10&&x>=10)){
                                if(2*x+6*y+z>n*0.9&&2*x+6*y+z<=n){
                                    printf("%d %d %d\n",x,y,z);
                                    f=1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(f==0){
        printf("no answer");
    }
    return 0;
}