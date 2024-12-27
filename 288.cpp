#include <bits/stdc++.h>

int main()
{
    char a[105],b[105];
    int s=0;
    std::cin.getline(a,105,'\n');
    std::cin.getline(b,105,'\n');
    for(int i=0;;i++){
        if(a[i]==0||b[i]==0) break;
        if(a[i]!=b[i]){
            s+=a[i]-b[i];
            break;
        }
    }
    std::cout<<s;
    return 0;
}