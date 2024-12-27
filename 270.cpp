#include <bits/stdc++.h>

int gcd(int x,int y){
    if(x<y) return gcd(y,x);
    if(y==0) return x;
    return gcd(x%y,y);
}

int main()
{
    int a,b;
    scanf("%d/%d",&a,&b);
    int x = gcd(a,b);
    printf("%d/%d",a/gcd(a,b),b/gcd(a,b));
    return 0;
}