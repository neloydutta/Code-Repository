#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

int pick_minimum(int *arr, int size){
    int i, small = INT_MAX, sindex = -1;
    for(i=0; i<size; i++){
        if(arr[i] <= small){
            small = arr[i];
            sindex = i;
        }
    }
    return sindex;
}

void merge_files(int size, char *outfile){
    int i, num[size], min_index, eof_count = 0;
    FILE *outputfile = fopen(outfile,"w");
    FILE *intermediate_outfile[size];
    char temp[100];

    for(i=0; i<size; i++){
        sprintf(temp,"out%d.txt",i+1);
        intermediate_outfile[i] = fopen(temp,"r");
        if(!feof(intermediate_outfile[i])){
            fscanf(intermediate_outfile[i],"%d",&num[i]);
        }
        else{
            num[i] = INT_MAX;
            eof_count += 1;
        }
    }
    while(eof_count < size){
        min_index = pick_minimum(num, size);
        fprintf(outputfile, "%d\n", num[min_index]);
        if(!feof(intermediate_outfile[min_index])){
            fscanf(intermediate_outfile[min_index],"%d",&num[min_index]);
        }
        else{
            num[min_index] = INT_MAX;
            eof_count += 1;
        }
    }
    for(i=0; i<size; i++){
        fclose(intermediate_outfile[i]);
        sprintf(temp,"out%d.txt",i+1);
        unlink(temp);
    }
    fclose(outputfile);
    return;
}

void externalsort(char *input_filename, char *output_filename, unsigned long long max_run_size, unsigned long long num_of_blocks){
    int i,num, *arr,size, outfile_index;
    char temp[100];
    FILE *ip = fopen(input_filename, "r");
    FILE *intermediate_outfiles[num_of_blocks];
    arr = malloc(max_run_size*sizeof(int));
    outfile_index = 0;
    while(!feof(ip) && outfile_index < num_of_blocks){
        sprintf(temp, "out%d.txt", outfile_index+1);
        intermediate_outfiles[outfile_index] = fopen(temp,"w");
        for(i=0;i<max_run_size;i++){
            fscanf(ip,"%d",&num);
            arr[i] = num;
            if(feof(ip))
                break;
        }
        size = i;
        mergeSort(arr,0,size-1);
        for(i=0; i<size; i++)
            fprintf(intermediate_outfiles[outfile_index],"%d\n",arr[i]);
        fclose(intermediate_outfiles[outfile_index]);
        outfile_index += 1;
    }
    fclose(ip);
    free(intermediate_outfiles);
    merge_files(outfile_index, output_filename);
    return;
}

int main(){
    unsigned long long int max_run_size = 500000;
    unsigned long long int num_of_blocks = 500;
    char *input_filename = "file.txt";
    char *output_filename = "out.txt";
    externalsort(input_filename, output_filename, max_run_size, num_of_blocks);
    return 0;
}
