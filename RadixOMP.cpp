#include<iostream>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<omp.h>
#define NUM_THREADS 2
using namespace std;

int largest(long *a , int s,int e)
{
    int i;
    int max = 0;
    for(i=s ; i<e ; i++)
    {
        if(a[i] > max)
        max = a[i];
    }

    return max;
}

void counting_sort(long *arr, int start, int size, int pow, int total)
{
    long *result = new long[total];
    int new_arr[10] = {0};

    for(int i=start; i<size; i++)
    {
        new_arr[(arr[i]/pow)%10]++;
    }
    //cumulative frenquency
    for(int i=1; i<10; i++)
    {
        new_arr[i] += new_arr[i-1];
    }
    //sorting
    for(int i=size-1; i>=start; i--)
    {
        result[new_arr[(arr[i]/pow)%10]-1] = arr[i];
        new_arr[(arr[i]/pow)%10]--;
    }
    int k=0;
    for(int i=start; i<size; i++)
        arr[i] = result[k++];
    delete[] result;
}

void radix_sort(long *arr,int start,int size,int max,int total)
{
    int exp = 1;

    for (exp = 1; max/exp > 0; exp *= 10) 
    {   
        counting_sort(arr, start, size, exp, total);    
    }
}

void main_helper(const char *data_set)
{
    clock_t t; 
    t = clock();   
    int size , i = 0 ,j ,k;

    FILE *fptr = fopen(data_set,"r");
    fscanf(fptr,"%d",&size);

    long *input_arr = new long[size];

    while( i < size )
    {
        fscanf(fptr , "%ld" , &input_arr[i]);
        i = i + 1;
    }

    fclose(fptr);

    //Spliting data into two sections
        int start[2], end[2];
        int max[2];
        //int ret[2];
        start[0] = 0;
        end[0]=(size/2);
        start[1] = (size/2);
        end[1] = size;
        omp_set_num_threads(NUM_THREADS);
        #pragma omp parallel
        {
            #pragma omp sections
            {
                #pragma omp section
                {
                    int id = omp_get_thread_num();
                    int max =largest(input_arr,start[0],end[0]);
                    radix_sort(input_arr, start[0], end[0], max, end[0]);
                    
                }
                #pragma omp section
                {
                    int id = omp_get_thread_num();
                    int max = largest(input_arr,start[1], end[1]);
                    radix_sort(input_arr, start[1], end[1], max, start[1]);
                }
            }
        }

        
        long result_arr[size];// = new long[size];
        i=start[0], j=start[1], k=0;
        while(i<end[0] && j<end[1])
        {
            int x=input_arr[i];
            int y=input_arr[j];
            if(x<=y)
            {
                
                result_arr[k] = x;
                k++;
                i++;
                
                
            }
            else
            {
                result_arr[k] = y;
                k++;
                j++;
                
                
            }
        }
        while(i<end[0])
        {
            result_arr[k] = input_arr[i];    
            k++;
            i++;
            
        }
        while(j<end[1])
        {

            result_arr[k] = input_arr[j];
            k++;
            j++;
            
        }
        
    
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 

    FILE *fptr1 = fopen("SortedRadixOMP.txt","a+");  
    
    fprintf(fptr , "%s\n" , data_set);   

    for(i=0 ; i<size ; i++)
    {
        fprintf(fptr , "%ld\n" , result_arr[i]);
    }
    
    fprintf(fptr , "\nElapsed time = %f seconds\n\n" , time_taken);    
    
    cout<<"Elapsed Time = "<<time_taken<<" seconds\n\n";

    fclose(fptr);
    delete[] input_arr;
}


int main()
{
    FILE *fptr2 = fopen("SortedRadixOMP.txt","w");
    const char *Data_Set[3] = {"Count_100.txt", "Count_100000.txt","Count_1000000.txt"};
    string mess[3] = {"Random 100 elements data set", "Random 100000 elements data set", "Random 1000000 elements data set"};
    int i;
      
    cout<<"Radix Sort using OpenMP \n";
    for(i=0; i<3 ; i++)
    {
        cout<<endl<<mess[i]<<endl;
        main_helper(Data_Set[i]);
    }
    fclose(fptr2);
    return 0;
}
