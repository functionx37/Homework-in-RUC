#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

struct zimu{
    char p;
    int f;
}a[26];

bool cmp(zimu a,zimu b){
    if(a.f>b.f) return 1;
    if(a.f<b.f) return 0;
    if(a.p<b.p) return 1;
    return 0;
}

int main()
{
	string str;
	cin>>str;
	for (int i=0;i<str.length();i++)
	{
		if (str[i]>='a'&&str[i]<='z')
		str[i]-=32;
	}
    for (int j=0;j<str.length();j++)
    {
    	a[str[j]-'A'].f++;
    }
    for(int i=0;i<26;i++){
        a[i].p=i+'a';
    }
    sort(a,a+26,cmp);
    for(int i=0;i<26;i++){
        if(a[i].f==0){
            break;
        }
        cout<<a[i].p<<" "<<a[i].f<<endl;
    }
    return 0;
}