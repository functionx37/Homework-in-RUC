#include <bits/stdc++.h>

char board[2005][2005]={0};
char word[2005];

bool ifsolve = 0;

int redir=0,rebla=2000;

int bi,bj,ei,ej;

int main(){
    int m,n;
    std::cin>>m>>n;
    for(int i=1;i<=m;i++){
        for(int j=1;j<=n;j++){
            std::cin>>board[i][j];
        }
    }
    for(int i=0;i<2005;i++){
        board[0][i]='1';
        board[i][0]='1';
        board[m+1][i]='1';
        board[i][n+1]='1';
    }
    std::cin>>word;
    int num = strlen(word);
    int dir,bla;
    for(int i=1;i<m;i++){
        for(int j=1;j<n;j++){
            bool f;

            f=1;
            bla=0;
            dir=1;
            for(int k=0;k<num;k++){
                if(board[i][j+k]=='0'||board[i][j+k]=='*'||board[i][j+k]==word[k]){
                    if(board[i][j+k]=='0') bla++;
                }
                else{
                    f=0;
                    break;
                }
            }
            if(f==1){
                if(board[i][j-1]=='1'&&board[i][j+num]=='1'){
                    ifsolve=1;
                    if((bla<rebla)||(bla==rebla&&dir>redir)){
                        bi = i;
                        bj = j;
                        ei = i;
                        ej = j+num-1;
                        redir = dir;
                        rebla = bla;
                    }
                }
            }


            f=1;
            bla=0;
            dir=0;
            for(int k=0;k<num;k++){
                if(board[i][j-k]=='0'||board[i][j-k]=='*'||board[i][j-k]==word[k]){
                    if(board[i][j-k]=='0') bla++;
                }
                else{
                    f=0;
                    break;
                }
            }
            if(f==1){
                if(board[i][j+1]=='1'&&board[i][j-num]=='1'){
                    ifsolve=1;
                    if((bla<rebla)||(bla==rebla&&dir>redir)){
                        bi = i;
                        bj = j;
                        ei = i;
                        ej = j-num+1;
                        redir = dir;
                        rebla = bla;
                    }
                }
            }

            f=1;
            bla=0;
            dir=1;
            for(int k=0;k<num;k++){
                if(board[i+k][j]=='0'||board[i+k][j]=='*'||board[i+k][j]==word[k]){
                    if(board[i+k][j]=='0') bla++;
                }
                else{
                    f=0;
                    break;
                }
            }
            if(f==1){
                if(board[i-1][j]=='1'&&board[i+num][j]=='1'){
                    ifsolve=1;
                    if((bla<rebla)||(bla==rebla&&dir>redir)){
                        bi = i;
                        bj = j;
                        ei = i+num-1;
                        ej = j;
                        redir = dir;
                        rebla = bla;
                    }
                }
            }

            f=1;
            bla=0;
            dir=0;
            for(int k=0;k<num;k++){
                if(board[i-k][j]=='0'||board[i-k][j]=='*'||board[i-k][j]==word[k]){
                    if(board[i-k][j]=='0') bla++;
                }
                else{
                    f=0;
                    break;
                }
            }
            if(f==1){
                if(board[i+1][j]=='1'&&board[i-num][j]=='1'){
                    ifsolve=1;
                    if((bla<rebla)||(bla==rebla&&dir>redir)){
                        bi = i;
                        bj = j;
                        ei = i-num+1;
                        ej = j;
                        redir = dir;
                        rebla = bla;
                    }
                }
            }

        }
    }
    if(ifsolve==0){
        printf("No");
    }
    else{
        printf("%d %d\n%d %d",bi-1,bj-1,ei-1,ej-1);
    }
    return 0;
}