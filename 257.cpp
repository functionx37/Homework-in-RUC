#include<bits/stdc++.h>
using namespace std;

int main()
{
	int k; cin>>k;
	int n; n=0;
	double s; s=0;
	while(1){
		n=n+1;
		s=s+1.0/n;
		if(s>k) break;
	}
	cout<<n;
	return 0 ;
}