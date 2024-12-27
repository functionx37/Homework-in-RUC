#include<bits/stdc++.h>

void day(int i){
        if(i==0) printf("Sunday");
        else if(i==1) printf("Monday");
        else if(i==2) printf("Tuesday");
        else if(i==3) printf("Wednesday");
        else if(i==4) printf("Thursday");
        else if(i==5) printf("Friday");
        else if(i==6) printf("Saturday");
}

int main(){
    int a,b;
    std::cin>>a>>b;
    a = (a+1)%7;
    b = (b-1)%7;
    day(a);
    std::cout<<std::endl;
    day(b);
    return 0;
}