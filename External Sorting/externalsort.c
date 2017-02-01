#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

void merge(int arr[], int l, int m, int r){
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
    while (i < n1 && j < n2){
        if (L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r){
    if (l < r){
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
    FILE *intermediate_infile[500];
    FILE *intermediate_outfile;
    char temp[1000000];
    int level = 0;
    int intermediate_outfile_count = size;
    int completed_file_count = 0;
    int previdx = 0;
    while(intermediate_outfile_count > 1){
        intermediate_outfile_count = 0;
        completed_file_count = 0;
        while(completed_file_count < size){
            eof_count = 0;
            sprintf(temp, "out%d%d.txt", level+1, ++intermediate_outfile_count);
            intermediate_outfile = fopen(temp, "w");
            for(i=0; i<500 && completed_file_count<size; i++){
                sprintf(temp,"out%d%d.txt",level,completed_file_count+1);
                intermediate_infile[i] = fopen(temp,"r");
                completed_file_count += 1;
                if(!feof(intermediate_infile[i])){
                    fscanf(intermediate_infile[i],"%d",&num[i]);
                }
                else{
                    num[i] = INT_MAX;
                    eof_count += 1;
                }
            }
            while(eof_count < i){
                min_index = pick_minimum(num, i);
                fprintf(intermediate_outfile, "%d\n", num[min_index]);
                if(!feof(intermediate_infile[min_index])){
                    fscanf(intermediate_infile[min_index],"%d",&num[min_index]);
                }
                else{
                    num[min_index] = INT_MAX;
                    eof_count += 1;
                    fclose(intermediate_infile[min_index]);
                    sprintf(temp,"out%d%d.txt", level, previdx+min_index+1);
                    printf("%s\n",temp);
                    unlink(temp);
                }
            }
            previdx = completed_file_count;
            fclose(intermediate_outfile);
        }
        level += 1;
        size = intermediate_outfile_count;
    }
    sprintf(temp, "out%d%d.txt", level, 1);
    intermediate_outfile = fopen(temp, "r");
    while((size=fread(temp,1,100,intermediate_outfile)) != 0){
        fwrite(temp, 1, size, outputfile);
    }
    fclose(intermediate_outfile);
    sprintf(temp, "out%d%d.txt", level, 1);
    printf("%s",temp);
    unlink(temp);
    fclose(outputfile);
    return;
}

void externalsort(char *input_filename, char *output_filename, unsigned long long max_run_size){
    int i,num, *arr,size, outfile_index;
    char temp[100];
    FILE *ip = fopen(input_filename, "r");
    FILE *intermediate_outfile;
    arr = malloc(max_run_size*sizeof(int));
    outfile_index = 0;
    while(!feof(ip)){
        sprintf(temp, "out0%d.txt", outfile_index+1);
        intermediate_outfile = fopen(temp,"w");
        for(i=0;i<max_run_size;i++){
            fscanf(ip,"%d",&num);
            arr[i] = num;
            if(feof(ip))
                break;
        }
        size = i;
        mergeSort(arr,0,size-1);
        for(i=0; i<size; i++)
            fprintf(intermediate_outfile,"%d\n",arr[i]);
        fclose(intermediate_outfile);
        outfile_index += 1;
    }
    fclose(ip);
    merge_files(outfile_index, output_filename);
    return;
}

int main(){
    unsigned long long int max_run_size = 500000;
    char *input_filename = "file1.txt";
    char *output_filename = "out.txt";
    externalsort(input_filename, output_filename, max_run_size);
    return 0;
}
