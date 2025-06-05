#include <bits/stdc++.h>

bool run(int x)
{
    if (x % 4 != 0)
        return 0;
    if (x % 400 == 0)
        return 1;
    if (x % 100 == 0)
        return 0;
    return 1;
}

struct week
{
    int bm;
    int bd;
    int bsd;
    int esd;
    int em;
    int ed;
    int bab = 0;
    int boy = 0;
} b[60];

bool cmp(week p,week q){
    if(p.bab>q.bab) return 1;
    if(p.bab<q.bab) return 0;
    if(p.bsd<q.bsd) return 1;
    return 0;
}

int dcou(int ye, int mo)
{
    if (mo == 2)
    {
        if (run(ye) == 1)
            return 29;
        else
            return 28;
    }
    if (mo == 4 || mo == 6 || mo == 9 || mo == 11)
    {
        return 30;
    }
    return 31;
}

int n, m;
int main()
{
    int y;
    std::cin >> y;
    int ld = 0;
    for (int i = 1969; i < y; i++)
    {
        if (run(i) == 0)
            ld += 365;
        if (run(i) == 1)
            ld += 366;
    }
    int wd = (3 + ld) % 7;
    int mm = 1, dd = 1, ww = 1;
    b[1].bm = 1;
    b[1].bd = 1;
    while (mm < 13)
    {
        wd++;
        bool f = 0;
        if (wd == 7)
        {
            wd = 0;
            b[ww].em = mm;
            b[ww].ed = dd;
            ww++;
            f = 1;
        }
        dd++;
        if (dd > dcou(y, mm))
        {
            mm++;
            dd = 1;
        }
        if (f == 1)
        {
            b[ww].bm = mm;
            b[ww].bd = dd;
        }
    }
    b[ww].em = 12;
    b[ww].ed = 31;
    for(int j=1;j<60;j++){
        b[j].bsd = b[j].bm * 100 + b[j].bd;
        b[j].esd = b[j].em * 100 + b[j].ed;
    }
    std::cin >> n >> m;
    for (int i = 0; i < n; i++)
    {
        int id, m, d, sex;
        std::cin >> id >> m >> d >> sex;
        int sd = m * 100 + d;
        for (int j = 1; j < 60; j++)
        {
           

            if (sd >= b[j].bsd && sd <= b[j].esd)
            {
                b[j].bab++;
                b[j].boy += sex;
                break;
            }
        }
    }
    for (int i = 1; i < 60; i++)
    {
        for (int j = i + 1; j < 60; j++)
        {
            if(cmp(b[i],b[j])==0){
                std::swap(b[i],b[j]);
            }
        }
    }
    for (int i = 1; i <= m; i++)
    {
        printf("[%2d.%2d-%2d.%2d]:%d(B%d)\n", b[i].bm, b[i].bd, b[i].em, b[i].ed, b[i].bab, b[i].boy);
    }
    return 0;
}