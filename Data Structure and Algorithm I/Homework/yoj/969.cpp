#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int tree[1000001];
int n,m;
int main()
{
    int i,num,ans;
    long long sum;
    scanf("%d%d",&n,&m);
    for(i=1;i<=n;i++)
        scanf("%d",&tree[i]);
    sort(tree+1,tree+n+1);
    sum=0;
    num=n;
    while(sum<m)
    {
        sum+=(tree[num]-tree[num-1])*(n-num+1);
        num--;
    }
    num++;
    ans=tree[num-1]+(sum-m)/(n-num+1);
    printf("%d\n",ans);
    return 0;
}