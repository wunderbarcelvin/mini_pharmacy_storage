#include "header.h"

int main(void){
    //initialize hash table

    clock_t start_time = clock();

    hashTable storage;
    for (int i = 0; i < 10; i++)
    {
        storage.table[i]= NULL;
    }
    
    menu(&storage);

    clock_t end_time = clock();
    
    double time_taken = ((double)(end_time-start_time))/ CLOCKS_PER_SEC;
    
    printf("Fungsi memakan %f sekon untuk dieksekusi.\n", time_taken);
    return 0;
}