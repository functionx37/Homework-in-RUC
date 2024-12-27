#include<bits/stdc++.h>
using namespace std;

int x,y,a,b; long long p[25][25]; bool in[25][25];

int main()
{   
	cin>>x>>y>>a>>b;
	in[a][b]=1;
	in[a-1][b-2]=1;
	in[a+1][b-2]=1;
	in[a-1][b+2]=1;
	in[a+1][b+2]=1;
	in[a-2][b-1]=1;
	in[a+2][b-1]=1;
	in[a-2][b+1]=1;
	in[a+2][b+1]=1;
	p[x][y]=1;
	for(int i=x;i>=0;i--){
		for(int j=y;j>=0;j--){
			if(i==x&&j==y) ; 
			else p[i][j]=p[i][j+1]+p[i+1][j];
			if(in[i][j]==1) p[i][j]=0;
		}
	}
	cout<<p[0][0];
    return 0;  
}  