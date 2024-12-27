#include <stdio.h>
#include <stdlib.h>

int power( int x, int y)
{
if(y==0) return 1;
    int j = x;
    for(int i=1;i<y;i++){
        x *= j;
    }
    return x;
}
int main()
{
    int x, y;
    scanf("%d%d", &x, &y);
    printf("%d", power(x, y));
    return 0;

}