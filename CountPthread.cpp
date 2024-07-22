#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

struct Arguments
{
    long *array;
    int _start;
    int _size;
    int _max;
    int _total;
};

void *Count_sort(void *args)
{
    struct Arguments *args_ptr = (struct Arguments *)args;
    long *arr = args_ptr->array;
    int start = args_ptr->_start;
    int size = args_ptr->_size;
    int max = args_ptr->_max;
    int total = args_ptr->_total;

    long *count = new long[max + 1];

    long *temp = new long[total];
    memset(count, 0, sizeof(long) * (max + 1));
    int i, j;

    for (i = start; i < size; i++)
    {
        count[arr[i]]++;
    }

    for (j = 1; j <= max; j++)
    {
        count[j] = count[j - 1] + count[j];
    }

    for (i = size - 1; i >= start; i--)
    {
        temp[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }

    int k = 0;
    for (i = start; i < size; i++)
    {
        arr[i] = temp[k++];
    }

    delete[] count;
    delete[] temp;
    return NULL;
}

long largest(long *a, int s, int e)
{
    int i;
    long max = 0;
    for (i = s; i < e; i++)
    {
        if (a[i] > max)
            max = a[i];
    }

    return max;
}

void main_helper(const char *data_set)
{
    clock_t t;
    t = clock();
    int size, i = 0, j, k;

    FILE *fptr = fopen(data_set, "r");

    fscanf(fptr, "%d", &size);

    long *input_arr = new long[size];

    while (i < size)
    {
        fscanf(fptr, "%ld", &input_arr[i]);
        i++;
    }

    fclose(fptr);

    // Splitting data into two sections
    int start[2], end[2];
    int max[2];
    int ret[2];
    start[0] = 0;
    end[0] = (size / 2);
    start[1] = (size / 2);
    end[1] = size;
    max[0] = largest(input_arr, start[0], end[0]);

    max[1] = largest(input_arr, start[1], end[1]);

    struct Arguments temp[2];
    temp[0].array = input_arr;
    temp[0]._start = start[0];
    temp[0]._size = end[0];
    temp[0]._max = max[0];
    temp[0]._total = end[0];

    temp[1].array = input_arr;
    temp[1]._start = start[1];
    temp[1]._size = end[1];
    temp[1]._max = max[1];
    temp[1]._total = start[1];

    pthread_t pid[2];

    pthread_create(&pid[0], NULL, Count_sort, &temp[0]);

    pthread_create(&pid[1], NULL, Count_sort, &temp[1]);

    pthread_join(pid[0], NULL);
    pthread_join(pid[1], NULL);

    long *result_arr = new long[size];
    i = start[0], j = start[1], k = 0;
    while (i < end[0] && j < end[1])
    {
        if (input_arr[i] <= input_arr[j])
        {
            result_arr[k] = input_arr[i];
            k++;
            i++;
        }
        else
        {
            result_arr[k] = input_arr[j];
            k++;
            j++;
        }
    }
    while (i < end[0])
    {
        result_arr[k] = input_arr[i];
        k++;
        i++;
    }
    while (j < end[1])
    {
        result_arr[k] = input_arr[j];
        k++;
        j++;
    }

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

    FILE *fptr1 = fopen("SortedCountPTH.txt", "a+");

    fprintf(fptr1, "%s\n", data_set);

    for (i = 0; i < size; i++)
    {
        fprintf(fptr1, "%ld\n", result_arr[i]);
    }

    fprintf(fptr1, "\nElapsed time = %f seconds\n\n", time_taken);

    cout << "Elapsed time = " << time_taken << " seconds\n\n";

    fclose(fptr1);
    delete[] input_arr;
    delete[] result_arr;
}

int main()
{
    FILE *fptr2 = fopen("SortedCountPTH.txt", "w");
    const char *Data_Set[3] = {"Count_100.txt", "Count_100000.txt", "Count_1000000.txt"};
    const char *mess[3] = {"Random 100 elements data set", "Random 100000 elements data set", "Random 1000000 elements data set"};
    int i;

    cout << "Count Sort using PThread" << endl;
    for (i = 0; i < 3; i++)
    {
        cout << endl << mess[i] << endl;
        main_helper(Data_Set[i]);
    }
    fclose(fptr2);
    return 0;
}
