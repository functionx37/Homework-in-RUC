#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstring>
using namespace std;

int replaceStr(char A[ ], const char B[ ], const char C[ ])
{
//补充代码，完成此函数
int i=0,j=0;
    char anss[105];
    strcpy(anss,A);
    A[0] = '\n';
    A[1] = 0;
    strcat(A,anss);
    while(i<strlen(A)&&j<strlen(B)){
        if(A[i]==B[j]){
            i++;
            j++;
        }
        else{
            i = i-j+1;
            j=0;
        }
    }
    if(j==strlen(B)){
        char ans[100];
        for(int k=i;k<strlen(A);k++){
            ans[k-i] = A[k];
        }
        ans[strlen(A)-i] = 0;
        A[i-j] = 0;
        strcat(A,C);
        strcat(A,ans);
        return 1;
    }
    strcat(A,C);
    return 0;
}



int main()
{
    char str1[110], str2[110], str3[110];
    int nRes;

    cin.getline(str1, 100);
    cin.getline(str2, 100);
    cin.getline(str3, 100);

    nRes = replaceStr(str1, str2, str3);
    printf("%d", nRes);
    printf("%s", str1);
    return 0;

}