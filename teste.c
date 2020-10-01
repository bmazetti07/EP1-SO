#include <stdio.h>    
     
int main()    
{    
    int ini = 2;
    int fim = 5;

    //Initialize array    
    //           0  1  2  3  4  5  6 
    int arr[] = {7, 6, 5, 4, 3, 2, 1};     
    int temp = 0;    
        
    //Calculate length of array arr    
    int length = sizeof(arr)/sizeof(arr[0]);    
        
    //Displaying elements of original array    
    printf("Elements of original array: \n");    
    for (int i = 0; i < length; i++) {     
        printf("%d ", arr[i]);     
    }      
        
    //Sort the array in ascending order    
    for (int i = ini; i < fim; i++) {     
        for (int j = i+1; j < fim; j++) {     
           if(arr[i] > arr[j]) {    
               temp = arr[i];    
               arr[i] = arr[j];    
               arr[j] = temp;    
           }     
        }     
    }    
        
    printf("\n");    
        
    //Displaying elements of array after sorting    
    printf("Elements of array sorted in ascending order: \n");    
    for (int i = 0; i < length; i++) {     
        printf("%d ", arr[i]);    
    }
    printf ("\n");

    return 0;    
}     