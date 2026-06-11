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

/**typedef struct Location
{
    char zone[4]; //DRY, WET, FRZ, CHM
    int aisle; //walkways in the warehouse
    char bay[2]; //section of rack
    int level; //vertical level of rack
    char position;
} Location;
 */

void storeLocation(Node* data, char* coordinate){
    //template koordinat: zona - aisle - bay - level - position
    char* token = strtok(coordinate, '-');
    
    if(!token){
        perror("Error parsing.");
        return;
    }
    strncpy(data->info.plot.zone, token, strlen(token)-1); //zona
    data->info.plot.zone[strlen(token)-1] = '\0';

    token = strtok(NULL, '-'); //walkway/lorong
    data->info.plot.aisle = atoi(token);
    
    token = strtok(NULL, '-'); //rak
    strncpy(data->info.plot.bay, token, 2);
    
    token = strtok(NULL, '-'); //level horizontal di rak tertentu
    data->info.plot.level = atoi(token);

    token = strtok(NULL, '-'); //position
    data->info.plot.position = token;
}

void removeData(hashTable* storage){
    char ID[20];
    printf("Masukkan ID: "); scanf("%s", ID); //masukkan ID
    
    int idx = djbHashFunction(ID);
    Node* curr = storage->table[idx];
    
    Node* temp;
    while(curr){
        if(curr->next != NULL && curr->next->info.ID == ID){
            temp = curr->next->next;
            free(curr->next);
            if(temp){
                curr->next = temp;
            } else{
                curr->next = NULL;
            }
        }
        curr = curr->next;
    }
}

void searchProduct(hashTable* storage){
    char ID[20];
    printf("Masukkan ID: "); scanf("%s", ID); //masukkan ID
    
    int idx = djbHashFunction(ID);
    Node* curr = storage->table[idx];
    
    while(curr){
        if(curr->info.ID == ID){
            char print;
            printf("Produk ditemukan.\n Apakah Anda ingin mencetak rincian info tentang produk? (y/n)");
            scanf("%c", &print);
            if(print=='y'){
                printf("ID Produk: %s", curr->info.ID);
                printf("Nama Produk: %s", curr->info.name);
                printf("Kategori: %s", curr->info.category);
                printf("Jumlah stok: %s", curr->info.numStock);
                printf("Lokasi: %s-%d-%c-%d-%c", curr->info.plot.zone, curr->info.plot.aisle,
                    curr->info.plot.bay, curr->info.plot.level, curr->info.plot.position); //contohnya DRY-1-B-1-A
                //PIC sama status barang belum ditambahin
            }
            return;
        }
        curr = curr->next;
    }
}

void searchProduct(hashTable* storage);
void updateProduct(hashTable* storage);
void printAllProducts(hashTable* storage);
void printUniques(hashTable* storage);
void mask(hashTable* storage){
    int minStock, maxStock;
    int found = 0;

    printf("Masukkan batas bawah jumlah stok: ");
    scanf("%d", &minStock);
    printf("Masukkan batas atas jumlah stok: ");
    scanf("%d", &maxStock);

    printf("\n=== Hasil Pencarian Filter Stok (%d - %d) ===\n", minStock, maxStock);

    for (int i = 0; i < 10; i++) {
        Node* curr = storage->table[i];

        while (curr != NULL) {
            if (curr->info.numStock >= minStock && curr->info.numStock <= maxStock) {
                found = 1;
                char maskedID[20];
                int len = strlen(curr->info.ID);

                strcpy(maskedID, curr->info.ID);
                if (len > 4) {
                    for (int j = 2; j < len - 2; j++) {
                        maskedID[j] = '*';
                    }

                    for (int j = 1; j < len; j++) {
                        maskedID[j] = '*';
                    }
                }

                printf("ID Produk   : %s\n", maskedID);
                printf("Nama Produk : %s\n", curr->info.name);
                printf("Kategori    : %s\n", curr->info.category);
                printf("Jumlah stok : %d\n", curr->info.numStock);
            
                printf("Lokasi      : %s-%d-%s-%d-%c\n", 
                       curr->info.plot.zone, 
                       curr->info.plot.aisle, 
                       curr->info.plot.bay, 
                       curr->info.plot.level, 
                       curr->info.plot.position);
                       
                printf("---------------------------------------\n");
            }
            curr = curr->next;
        }
    }

    if (!found) {
        printf("Tidak ada produk yang ditemukan dalam interval stok tersebut.\n");
    }
}
