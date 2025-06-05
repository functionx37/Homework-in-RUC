#include <bits/stdc++.h>
using std::cin;
using std::cout;

int n,m;
int app[105][2];
bool add[105];

int main()
{
    cin>>n>>m;
    for(int i=0;i<n;i++){
        int li;
        cin>>li;
        for(int j=0;j<li;j++){
            int k;
            cin>>k;
            app[k][1]++;
            add[k] = 1;
        }
        for(int j=1;j<=m;j++){
            if(add[j]){
                add[j] = 0;
                app[j][0]++;
            }
        }
    }
    for(int i=1;i<=m;i++){
        printf("%d %d\n",app[i][0],app[i][1]);
    }
    return 0;
}