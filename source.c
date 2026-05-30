#include "header.h"
#include <stdio.h>
#include <stdlib.h>

int djbHashFunction(char* id){ //inspired by Dan Berstein
    unsigned long hash = 5381;
    int c;
    
    while ((c = *id++))
    {
        hash = ((hash<<5)+hash) + c;
    }
    
    return hash%10; //hash modulo 10 karena ukuran hash table
}

/**Add data
Remove data
Search data based on ID
Update/modify data (jumlah stok, pemilik barang, PIC, lokasi, status)
Update status 
Print all data
Print summary about inventories. Di sini bisa buat subfungsi menu dalam voidnya
Classify based on category, location, and status
Mask ID based on interval of number of stock */

void menu(hashTable* storage){
    
    int choice;
    do
    {
        print("Selamat datang di laman manajemen stok gudang XYZ\n");
        print("Silakan pilih salah satu instruksi di bawah.");

        printf("1. Menambahkan produk.\n");
        printf("2. Menghapus produk.\n");
        printf("3. Mencari produk.\n");
        printf("4. Memutakhirkan info produk.\n");
        printf("5. Cetak semua produk.\n");
        printf("6. Mencari klasifikasi produk tertentu.\n");
        printf("7. Mencari produk tertentu dengan kunci filter tertentu.\n");
        printf("8. Keluar dari program.\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addData(storage);
            break;
        case 2:
            removeData(storage);
            break;
        case 3:
            searchProduct(storage);
            break;
        case 4:
            updateProduct(storage);
            break;
        case 5:
            printAllProducts(storage);
            break;
        case 6:
            printUniques(storage);
            break;
        case 7:
            mask(storage);
            break;
        default:
            printf("Perintah tidak dikenali. Coba lagi.");
            break;
        }
    } while (choice!=8);
}



/**Setiap data inventaris setidaknya memuat: ID barang, nama komponen/peralatan, kategori 
barang, jumlah stok, lokasi penyimpanan, status barang (misalnya tersedia, dipinjam, rusak, atau habis), pemilik barang serta PIC untuk barang tersebut.
*/

void addData(hashTable* storage){
    Node* new = malloc(sizeof(Node));
    
    if(!new) return;

    printf("Masukkan ID: "); scanf("%s", new->info.ID); //masukkan ID
    int idx = djbHashFunction(new->info.ID);

    printf("Masukkan nama alat/barang: "); scanf("%s", new->info.name); //masukkan nama
    
    
    Node* temp = storage->table[idx];
    
    while(temp!=NULL){
        if(strcmp(temp->info.name, new->info.name)==0){
            int add;
            printf("Masukkan jumlah stok tambahan: "); scanf("%d", &add);
            temp->info.numStock+=add;
            printf("Jumlah produk berhasil diupdate.\n");
            return;
        }
        if(temp->next==NULL){
            temp->next = new; //simpan alamat memori satu node sebelum endpoint
        }
        temp = temp->next;
    }

    printf("Produk belum pernah ditambahkan.\n");
    
    printf("Masukkan kategori: "); scanf("%s", new->info.category);
    printf("Masukkan jumlah stok: "); scanf("%s", new->info.numStock);
    char location[15];
    printf("Masukkan koordinat lokasi: ");
    fgets(location, sizeof(location), stdin);
    
    storeLocation(new, location);
    
    //PIC dan status barang belum ditambahkan
    
    if(storage->table[idx]==NULL){
        storage->table[idx] = new;
    }
    
    printf("Produk berhasil ditambahkan.\n");
}

void storeLocation(Node* data, char* coordinate){
    //template koordinat: zona - aisle - bay - level - position
}

void removeData(hashTable* storage);
void searchProduct(hashTable* storage);
void searchProduct(hashTable* storage);
void updateProduct(hashTable* storage);
void printAllProducts(hashTable* storage);
void printUniques(hashTable* storage);
void mask(hashTable* storage);