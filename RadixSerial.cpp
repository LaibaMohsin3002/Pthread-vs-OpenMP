#include<iostream>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
using namespace std;

int largest(long *a, int size)
{
    int i, max = 0;
    for(i=0; i<size; i++)
    {
        if(a[i] > max)
        max = a[i];
    }

    return max;
}

void counting_sort(long *arr, int size, int pow)
{
    long *result = new long[size];
    int new_arr[10] = {0};

    for(int i=0; i<size; i++)
    {
        new_arr[(arr[i]/pow)%10]++;
    }
    //cumulative frequency
    for(int i=1; i<10; i++)
    {
        new_arr[i] += new_arr[i-1];
    }
    //sorting
    for(int i=size-1; i>=0; i--)
    {
        result[new_arr[(arr[i]/pow)%10]-1] = arr[i];
        new_arr[(arr[i]/pow)%10]--;
    }
    int k=0;
    for(int i=0; i<size; i++)
        arr[i] = result[k++];
    delete[] result;
}

void radix_sort(long *arr, int size, int max)
{
    int exp=0;

    for (exp = 1; max/exp > 0; exp *= 10)
    {   
        counting_sort(arr, size, exp);    
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
    fscanf(fptr , "%ld" , &input_arr[i]); // Changed %d to %ld
    i = i + 1;
   }

   fclose(fptr);
    int max=largest(input_arr,size);
    radix_sort(input_arr,size,max);    
    
   t = clock() - t; 
   double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 

   FILE *fptr1 = fopen("SortedRadixSerial.txt","a+");  
  
   fprintf(fptr1 , "%s\n" , data_set); // Changed fptr to fptr1  

   for(i=0 ; i<size ; i++)
   {
        fprintf(fptr1 , "%ld\n" , input_arr[i]); // Changed %d to %ld
   }
   
   fprintf(fptr1 , "\nElapsed time = %f seconds\n\n" , time_taken);    // Changed fptr to fptr1
   
   cout<<"Elapsed Time = "<<time_taken<<" seconds\n\n";

   fclose(fptr1); // Changed fptr to fptr1
   delete[] input_arr;
}


int main()
{
    FILE *fptr2 = fopen("SortedRadixSerial.txt","w");
    fprintf(fptr2 , "%s", "");  // Fixed zero-length format string
    const char *Data_Set[3] = {"Count_100.txt", "Count_100000.txt","Count_1000000.txt"};
    string mess[3] = {"Random 100 elements data set", "Random 100000 elements data set", "Random 1000000 elements data set"};
    int i;
      
    cout<<"Radix Sort Serial \n";
    for(i=0; i<3 ; i++) // Changed the loop to iterate only over the first three data sets
    {
        cout<<endl<<mess[i]<<endl;
        main_helper(Data_Set[i]);
    }
    fclose(fptr2); // Added closing file operation
    return 0;
}
