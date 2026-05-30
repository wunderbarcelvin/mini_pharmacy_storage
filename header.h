#ifndef TUBES_H
#define TUBES_H

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
    char zone[4]; //DRY, WET, FRZ, CHM
    int aisle; //walkways in the warehouse
    char bay[2]; //section of rack
    int level; //vertical level of rack
    char position;
} Location;

typedef struct Data
{
    char ID[20];
    char name[30];
    char category[15];
    int numStock;
    Location plot;
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

void menu(hashTable storage);
void addData(hashTable* storage);
void storeLocation(Node* data, char* coordinate);


void removeData(hashTable* storage);
void searchProduct(hashTable* storage);
void searchProduct(hashTable* storage);
void updateProduct(hashTable* storage);
void printAllProducts(hashTable* storage);
void printUniques(hashTable* storage);
void mask(hashTable* storage);


#endif