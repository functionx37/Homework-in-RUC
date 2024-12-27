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
	for(int i=x;i>=0;i--){
		for(int j=y;j>=0;j--){
			if(i==x&&j==y) p[i][j]=1; 
			else p[i][j]=p[i][j+1]+p[i+1][j];
			if(in[i][j]==1) p[i][j]=0;
		}
	}
	long long int sum1 = p[0][0];
    memset(p,0,sizeof(p));
    for(int i=x;i>=a;i--){
		for(int j=y;j>=b;j--){
			if(i==x&&j==y) p[i][j]=1; 
			else p[i][j]=p[i][j+1]+p[i+1][j];
		}
	}
    long long int sum2 = p[a][b];
    memset(p,0,sizeof(p));
    in[a][b]=0;
    for(int i=a;i>=0;i--){
		for(int j=b;j>=0;j--){
			if(i==a&&j==b) p[i][j]=1; 
			else p[i][j]=p[i][j+1]+p[i+1][j];
            if(in[i][j]==1) p[i][j]=0;
		}
	}
    long long int sum3 = p[0][0];
    std::cout<<(sum1+(sum2*sum3));
    return 0;  
}  