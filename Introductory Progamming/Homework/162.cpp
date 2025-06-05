#include <bits/stdc++.h>

int main()
{
    char a[15];
    int sum=0;
    scanf("%c-%c%c%c-%c%c%c%c%c-%c",&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7],&a[8],&a[9],&a[10]);
    for(int i=1;i<10;i++){
        sum+=((a[i]-'0')*i);
    }
    sum%=11;
    char b;
    if(sum<10) b='0'+sum;
    else b='X';
    if(b==a[10]){
        printf("Right");
    }
    else{
        printf("%c-%c%c%c-%c%c%c%c%c-%c",a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],b);
    }
    return 0;
}