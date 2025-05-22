#include <bits/stdc++.h>

struct typ{
    char name[30];
    int siz;
    int num;
}a[15];

int siz(int x){
    for(int i=0;i<30;i++){
        if(a[x].name[i]=='*') return 8;
    }
    if(a[x].name[0]=='c') return 1;
    if(a[x].name[0]=='b') return 1;
    if(a[x].name[0]=='s') return 2;
    if(a[x].name[0]=='i') return 4;
    if(a[x].name[0]=='f') return 4;
    if(a[x].name[0]=='l'&&a[x].name[5]=='l') return 8;    
    if(a[x].name[0]=='l'&&a[x].name[5]=='d') return 16;    
    if(a[x].name[0]=='l') return 4;    
    if(a[x].name[0]=='d') return 8;
    return -1;
}

bool cmp(typ a,typ b){
    if(a.siz>b.siz) return 1;
    return 0;
}

int main()
{
    int n;
    std::cin>>n;
    getchar();
    for(int i=0;i<n;i++){
        std::cin.getline(a[i].name,30,'\n');
        for(int j=strlen(a[i].name)-1;j>=0;j--){
            if(a[i].name[j]<48||a[i].name[j]>57){
                break;
            }
            a[i].num += (a[i].name[j]-'0')*std::pow(10,strlen(a[i].name)-1-j);
        }
        a[i].siz=siz(i);
    }
    std::sort(a,a+n,cmp);
    int mem = 0;
    for(int i=0;i<n;i++){
        while(mem%a[i].siz!=0){
            mem++;
        }
        mem+=(a[i].siz*a[i].num);
    }
    while(mem%a[0].siz!=0){
            mem++;
    }
    printf("%d\n",mem);
    for(int i=0;i<n;i++){
        std::cout<<a[i].name<<std::endl;
    }
    return 0;
}