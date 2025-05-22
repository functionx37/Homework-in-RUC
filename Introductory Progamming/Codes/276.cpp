#include<bits/stdc++.h>

struct allday{
    int year;
    int week;
    int month;
    int day;
    int weekday;
}x[25000];

int countday(int year, int month){
    bool g = 0;
    if(year % 4 == 0) g =1;
    if(year %100 == 0) g = 0;
    if(year % 400 == 0) g = 1;
    if(month == 4 ||month == 6 ||month == 9 ||month == 11) return 30;
    else if(month == 2 && g == 1) return 29;
    else if(month == 2 && g == 0) return 28;
    else return 31;
}

int main(){
    x[0].year = 1980;
    x[0].month = 1;
    x[0].day = 1;
    x[0].week = 1;
    x[0].weekday = 2;
    for(int i=1;i<24000;i++){
        x[i].year = x[i-1].year;
        x[i].month = x[i-1].month;
        x[i].day = x[i-1].day + 1;
        x[i].week = x[i-1].week;
        x[i].weekday = x[i-1].weekday + 1;
        if(x[i].weekday == 7){
            x[i].week += 1;
        }
        if(x[i].weekday == 8){
            x[i].weekday = 1;
        }
        if(x[i].day > countday(x[i].year,x[i].month)){
            x[i].day = 1;
            x[i].month += 1;
        }
        if(x[i].month == 13){
            x[i].month = 1;
            x[i].year += 1;
            x[i].week = 1;
        }
    }
    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        bool f = 0;
        int y,m;
        std::cin>>y>>m;
        for(int j=0;j<24000;j++){
            if(x[j].weekday == 2){
                if(x[j].year == y){
                    if(x[j].week == m){
                        f = 1;
                        printf("%d%02d%02d\n",x[j].year,x[j].month,x[j].day);
                        break;
                    }
                }
            }
        }
        if(f==0){
            printf("-1\n");
        }
    }
    return 0;
}