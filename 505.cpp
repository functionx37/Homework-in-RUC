#include<stdio.h>
#include<string.h>
#include<math.h>

typedef struct {
    char id[10];
    int score[5];
    
}student;

student students[100];

void bubblesort(student a[], int n, int q){
    int i, j;
    student temp;
    for(i = 0; i < n - 1; i++){
        for(j = 0; j < n - i - 1; j++){
            if(a[j].score[q] > a[j + 1].score[q]){
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}
void sort(student a[], int n, int q){
    int i, j;
    student temp;
    for(i = 0; i < n - 1; i++){
        for(j = 0; j < n - i - 1; j++){
            if(strcmp(a[j].id, a[j + 1].id) > 0 && a[j].score[q] == a[j + 1].score[q]){
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;    }
        }
    }
}


int main(){
    int n, m, k;
    float sum = 0;
    scanf("%d %d %d", &n, &m, &k);
    for(int i = 0; i < n; i++){
        scanf("%s", &students[i].id);
        for(int j = 0; j < m; j++){
            scanf(" %d", &students[i].score[j]);
        }
        scanf("\n");
    }

    bubblesort(students, n, k-1);
sort(students, n, k-1);
    for(int i = 0; i < n; i++) sum += students[i].score[k-1];
    float aver =sum / n;
    int u = floor((n) / 2);
    printf("%.1f %d\n",aver, students[u].score[k-1]);


    for(int i = 0; i < n; i++){
        if(students[i].score[k-1] == students[u].score[k-1]){
            printf("%s ", students[i].id);
            for(int j = 0; j < m; j++){
                printf("%d ", students[i].score[j]);
            }
            printf("\n");
            
        }
        else continue;
    }
    return 0;
}