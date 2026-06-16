#ifndef TUBES_H
#define TUBES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**Spesifikasi fungsi:
Add data
Remove data
Search data based on ID
Update/modify data (jumlah stok, pemilik barang, PIC, lokasi, status)
Update status 
Print all data
Print summary about inventories. Di sini bisa buat subfungsi menu dalam voidnya
Classify based on category, location, and status
Mask ID based on interval of number of stock

struktur datanya? i guess hash table
 */

/**1. Setiap data inventaris setidaknya memuat: ID barang, nama komponen/peralatan, kategori 
barang, jumlah stok, lokasi penyimpanan, status barang (misalnya tersedia, dipinjam, rusak, atau habis), pemilik barang serta PIC untuk barang tersebut. 

 */


typedef struct Location
{
    char aisle[3]; //walkways in the warehouse, contohnya A1
    char bay[3]; //section of rack, contohnya B2
    char level[3]; //vertical level of rack
    char position[3];
} Location;

typedef struct Data
{
    char ID[10];
    char name[15];
    char category[15];
    int numStock;
    Location plot;
    char status[9];
    char PIC[8];
    char owner[8];
} Data;

typedef struct Node
{
    Data info;
    struct Node* next;
} Node;

typedef struct hashTable
{
    Node* table[10];
} hashTable;


int djbHashFunction(char* id);
void initializeStorage(hashTable* storage);

void menu(hashTable* storage);
void addData(hashTable* storage);
void storeLocation(Node* data, char* coordinate);
void removeData(hashTable* storage);

void withdrawProduct(hashTable* storage);
void searchProduct(hashTable* storage);
void updateProduct(hashTable* storage);
void printAllProducts(hashTable* storage);
void printUniques(hashTable* storage);
void mask(hashTable* storage);

void printUniquePIC(hashTable* storage, int* count);
void printUniqueName(hashTable* storage, int* count);
void printUniqueCategory(hashTable* storage, int* count);
void printUniqueOwnwer(hashTable* storage, int* count);

#endif