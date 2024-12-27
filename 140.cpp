#include <bits/stdc++.h>
using namespace std;

int main()
{
    string a,bir,major;
    bir[0]=0;
    major[0] = 0;
    getline(cin,a);
    for(int i=0;i<a.length();i++){
        if(a[i]=='r'&&a[i+1]=='n'&&a[i+2]==' '&&a[i+3]=='i'&&a[i+4]=='n'&&a[i+5]==' '){
            for(int j=i+6;a[j]!=',';j++){
                bir.push_back(a[j]);
            }
        }
        if(a[i]=='m'&&a[i+1]=='a'&&a[i+2]=='j'&&a[i+3]=='o'&&a[i+4]=='r'){
            for(int j=i+9;a[j]!='.';j++){
                major.push_back(a[j]);
            }
        }
    }
    cout<<bir<<", "<<major<<endl;
    return 0;
}