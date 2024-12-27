#include <bits/stdc++.h>

struct ju{
    int x1;
    int x2;
    int y1;
    int y2;
} a,b;

int nei1(){
    int ans1 = 0,ans2=0;
    if(a.x1<b.x1&&a.x1>b.x2&&a.y1<b.y1&&a.y1>b.y2) ans1++;
    if(a.x1<b.x1&&a.x1>b.x2&&a.y2<b.y1&&a.y2>b.y2) ans1++;
    if(a.x2<b.x1&&a.x2>b.x2&&a.y1<b.y1&&a.y1>b.y2) ans1++;
    if(a.x2<b.x1&&a.x1>b.x2&&a.y2<b.y1&&a.y2>b.y2) ans1++;
    if(b.x1<a.x1&&b.x1>a.x2&&b.y1<a.y1&&b.y1>a.y2) ans2++;
    if(b.x1<a.x1&&b.x1>a.x2&&b.y2<a.y1&&b.y2>a.y2) ans2++;
    if(b.x2<a.x1&&b.x2>a.x2&&b.y1<a.y1&&b.y1>a.y2) ans2++;
    if(b.x2<a.x1&&b.x1>a.x2&&b.y2<a.y1&&b.y2>a.y2) ans2++;

    return std::max(ans1,ans2);
}

int nei2(){
    int ans1 = 0,ans2=0;
    if(a.x1<=b.x1&&a.x1>=b.x2&&a.y1<=b.y1&&a.y1>=b.y2) ans1++;
    if(a.x1<=b.x1&&a.x1>=b.x2&&a.y2<=b.y1&&a.y2>=b.y2) ans1++;
    if(a.x2<=b.x1&&a.x2>=b.x2&&a.y1<=b.y1&&a.y1>=b.y2) ans1++;
    if(a.x2<=b.x1&&a.x1>=b.x2&&a.y2<=b.y1&&a.y2>=b.y2) ans1++;
    if(b.x1<=a.x1&&b.x1>=a.x2&&b.y1<=a.y1&&b.y1>=a.y2) ans2++;
    if(b.x1<=a.x1&&b.x1>=a.x2&&b.y2<=a.y1&&b.y2>=a.y2) ans2++;
    if(b.x2<=a.x1&&b.x2>=a.x2&&b.y1<=a.y1&&b.y1>=a.y2) ans2++;
    if(b.x2<=a.x1&&b.x1>=a.x2&&b.y2<=a.y1&&b.y2>=a.y2) ans2++;

    return std::max(ans1,ans2);
}



int main(){
    std::cin>>a.x1>>a.y1>>a.x2>>a.y2>>b.x1>>b.y1>>b.x2>>b.y2;
    int p;
    if(a.x1<a.x2) std::swap(a.x1,a.x2);
    if(a.y1<a.y2) std::swap(a.y1,a.y2);
    if(b.x1<b.x2) std::swap(b.x1,b.x2);
    if(b.y1<b.y2) std::swap(b.y1,b.y2);

    if(a.x2>b.x1||a.x1<b.x2||a.y2>b.y1||a.y1<b.y2){ 
        p=0; 
        std::cout<<p;
        return 0;
    }
    if(a.x2==b.x1&&a.y2==b.y1||a.x1==b.x2&&a.y1==b.y2)
    { 
        p=1; 
        std::cout<<p;
        return 0;
    }
    if(a.x2==b.x1||a.y2==b.y1||a.x1==b.x2||a.y1==b.y2){ 
        p=2; 
        std::cout<<p;
        return 0;
    }
    if(a.x1==b.x1&&a.x2==b.x2&&a.y1==b.y1&&a.y2==b.y2){ 
        p=12; 
        std::cout<<p;
        return 0;
    }

    if((a.x1<b.x1&&a.x2>b.x2&&a.y1>b.y1&&a.y2<b.y2)||(a.x1>b.x1&&a.x2<b.x2&&a.y1<b.y1&&a.y2>b.y2)||(b.x1<a.x1&&b.x2>a.x2&&b.y1>a.y1&&b.y2<a.y2)||(b.x1>a.x1&&b.x2<a.x2&&b.y1<a.y1&&b.y2>a.y2)) { 
        p=10; 
        std::cout<<p;
        return 0;
    }
    
    int x=nei1();
    int y=nei2();
    if(y==4){ 
        p=8; 
        std::cout<<p;
        return 0;
    }

    if(x==1){ 
        p=4; 
        std::cout<<p;
        return 0;
    }
    if((a.x1==b.x1||a.x1==b.x2||a.y1==b.y1||a.y1==b.y2||a.x2==b.x1||a.x2==b.x2||a.y2==b.y1||a.y2==b.y2)) p=6;
    if(x==2) p=6;

    
    
    std::cout<<p;
    return 0;
}