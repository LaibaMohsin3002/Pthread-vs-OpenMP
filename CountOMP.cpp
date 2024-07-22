#include<iostream>
#include<string.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>

using namespace std;

long largest(long *arr, int size) 
{
    long max = arr[0]; // Start from index 0
       
    for(int i = 1; i < size; i++) // Start loop from index 1
    {
        if(arr[i] > max)
            max = arr[i];
    }
  
    return max; //the max element from the array
}


void Count_sort(long *arr,int size)
{
        
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            long *temp = new long[size];
            long max = largest(arr, size);
            long count[max+10]; // Allocate count array dynamically

            memset(count, 0, sizeof(count)); // Initialize count array to 0s
                            
            for(int i = 0; i < size; i++)
            {
                count[arr[i]]++; // Increase number count in count array.
            }
                
            for(int i = 1; i <= max; i++)
            {
                count[i] += count[i-1]; // Find cumulative frequency
            }
                
            for(int i = size-1; i >= 0; i--) 
            {
                temp[count[arr[i]]-1] = arr[i];
                count[arr[i]]--; // Decrease count for same numbers
            }
                    
            for(int i = 0; i < size; i++) 
            {
                arr[i] = temp[i]; // Store output array to main array
            }
                    
            delete[] temp;
        }
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
        fscanf(fptr , "%ld" , &input_arr[i]); // Correct specifier for long
        i++;
    }

    fclose(fptr);

    Count_sort(input_arr,size);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    FILE *fptr1 = fopen("SortedCountOMP.txt","a+");  
         
    fprintf(fptr1 , "%s\n" , data_set);  
           
    for(i = 0 ; i < size ; i++)
    {
        fprintf(fptr1 , "%ld\n" , input_arr[i]); // Correct specifier for long
    }
           
    fprintf(fptr1 , "\nElapsed time = %f seconds\n\n" , time_taken);
           
    cout<<"Elapsed time = "<<time_taken<<" seconds\n\n";

    fclose(fptr1);
    delete[] input_arr; // Deallocate memory
}


int main()
{
    const char *Data_Set[3] = {"Count_100.txt", "Count_100000.txt","Count_1000000.txt"};
    string mess[3] = {"Random 100 elements data set", "Random 100000 elements data set", "Random 1000000 elements data set"};
    int i;
    printf("Count Sort Using OpenMP \n");
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
