#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **a;
int **b;
int **c;

struct args{
    int i;
    int j;
    int c1;
};

void* parallel_util(void *arg){
    struct args *temp = (struct args *)arg;
    int k, sum=0;
    for(k=0; k<temp->c1; k++)
        sum += (a[temp->i][k] * a[k][temp->j]);
    c[temp->i][temp->j] = sum;
}

void matrix_multiplication(int r1, int c1, int r2, int c2){
    int i, j, tcount = 0;
    pthread_t t[r1*c2];
    c = malloc(sizeof(int*)*r1);
    for(i=0; i<r1; i++)
        c[i] = malloc(sizeof(int)*c2);
    for(i=0; i<r1; i++){
        for(j=0; j<c2; j++){
            struct args *temp = malloc(sizeof(struct args));
            temp->i = i;
            temp->j = j;
            temp->c1 = c1;
            pthread_create(&t[tcount++], NULL, parallel_util, (void*)temp);
            //parallel_util(temp);
        }
    }
    for(i=0; i<tcount; i++)
        pthread_join(t[i], NULL);
}

int main()
{
    int i,j;
    int r1, c1, r2, c2;
    FILE *fp = fopen("input.txt","r");

    fscanf(fp,"%d %d %d %d",&r1,&c1,&r2,&c2);

    a = malloc(sizeof(int*)*r1);
    for(i=0; i<r1; i++)
        a[i] = malloc(sizeof(int)*c1);
    b = malloc(sizeof(int*)*r2);
    for(i=0; i<r2; i++)
        b[i] = malloc(sizeof(int)*c2);

    for(i=0; i<r1; i++){
        for(j=0; j<c1; j++)
            fscanf(fp, "%d", &a[i][j]);
    }
    for(i=0; i<r2; i++){
        for(j=0; j<c2; j++)
            fscanf(fp, "%d", &b[i][j]);
    }
    matrix_multiplication(r1, c1, r2, c2);
    for(i=0; i<r1; i++){
        for(j=0; j<c2; j++){
            printf("%d ",c[i][j]);
        } printf("\n");
    }
    return 0;
}
