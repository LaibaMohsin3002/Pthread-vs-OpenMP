#include<iostream>
#include<string.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>

using namespace std;

void merge(long arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    long L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

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

void mergeSort(long arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSort(arr, l, m);
            }
            #pragma omp section
            {
                mergeSort(arr, m + 1, r);
            }
        }

        merge(arr, l, m, r);
    }
}

void main_helper(const char *data_set)
{
    clock_t t;
    t = clock();

    int size , i = 0;

    FILE *fptr = fopen(data_set,"r");

    fscanf(fptr,"%d",&size);

    long *input_arr = new long[size];

    while( i < size )
    {
        fscanf(fptr , "%ld" , &input_arr[i]);
        i++;
    }

    fclose(fptr);

    mergeSort(input_arr, 0, size - 1);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    FILE *fptr1 = fopen("SortedMergeOMP.txt","a+");

    fprintf(fptr1 , "%s\n" , data_set);

    for(i = 0 ; i < size ; i++)
    {
        fprintf(fptr1 , "%ld\n" , input_arr[i]);
    }

    fprintf(fptr1 , "\nElapsed time = %f seconds\n\n" , time_taken);

    cout<<"Elapsed time = "<<time_taken<<" seconds\n\n";

    fclose(fptr1);
    delete[] input_arr;
}


int main()
{
    const char *Data_Set[3] = {"Count_100.txt", "Count_100000.txt","Count_1000000.txt"};
    string mess[3] = {"Random 100 elements data set", "Random 100000 elements data set", "Random 1000000 elements data set"};
    int i;
    printf("Merge Sort Using OpenMP \n");
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for(i = 0; i < 3 ; i++)
            {
                cout << endl << mess[i] << endl;
                main_helper(Data_Set[i]);
            }
        }
    }
    return 0;
}
