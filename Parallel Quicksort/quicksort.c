#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct args{
    int *arr;
    int low;
    int high;
};

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int arr[], int low, int high){
    int pivot = arr[high];
    int i = (low - 1);
    int j;
    for (j = low; j <= high- 1; j++){
        if (arr[j] <= pivot){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high);

void *quickSortUtil(void *arg){
    int *arr, low, high;
    struct args *temp = (struct args*)arg;
    arr = temp->arr;
    low = temp->low;
    high = temp->high;
    if (low < high){
        int pidx = partition(arr, low, high);
        quickSort(arr, low, pidx - 1);
        quickSort(arr, pidx + 1, high);
    }
}

void quickSort(int arr[], int low, int high){
    struct args temp;
    pthread_t thread;
    int retval;
    temp.arr = arr;
    temp.low = low;
    temp.high = high;
    retval = pthread_create(&thread, NULL, quickSortUtil, (void*)&temp);
    if(retval){
        printf("Thread Error!");
        exit(-1);
    }
    pthread_join(thread, NULL);
}

int main(){
    int arr[] = {10, 80, 30, 90, 40, 50, 70};
    int i;
    quickSort(arr, 0, 6);
    for(i=0; i<=6; i++)
        printf("%d ",arr[i]);
    return 0;
}
