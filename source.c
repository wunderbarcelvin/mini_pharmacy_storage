#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    Node* temp2; //pointer untuk menyimpan alamat terakhir setelah dikenai hash function
    
    while(temp!=NULL){
        if(strcmp(temp->info.ID, new->info.ID)==0){
            int add;
            printf("Masukkan jumlah stok tambahan: "); scanf("%d", &add);
            temp->info.numStock+=add;
            printf("Jumlah produk berhasil diupdate.\n");
            return;
        }
        if(temp->next==NULL){ //jika selanjutnya tidak ada data lagi dan belum ada produk serupa yang pernah disimpan
            temp2 = temp;
        }
        temp = temp->next;
    }

    printf("Produk belum pernah ditambahkan.\n");
    
    printf("Masukkan kategori: "); scanf("%s", new->info.category);
    printf("Masukkan jumlah stok: "); scanf("%d", new->info.numStock);
    char location[15];
    printf("Masukkan koordinat lokasi: ");
    scanf("%s", location);
    
    storeLocation(new, location);
    
    //karena baru ditambahkan, maka statusnya available.
    strcpy(new->info.status, "Tersedia");

    printf("Masukkan nama PIC: "); scanf("%s", new->info.PIC);
    printf("Masukkan nama pemilik: "); scanf("%s", new->info.owner);
    

    //store to the hash table
    if(storage->table[idx]==NULL){
        storage->table[idx] = new;
    } else{
        temp2->next=new;
        new->next = NULL;
    }
    
    printf("Produk berhasil ditambahkan.\n");
}


void storeLocation(Node* data, char* coordinate){
    //template koordinat: zona - aisle - bay - level - position
    char* token = strtok(coordinate, "-");
    
    if(!token){
        perror("Error parsing.");
        return;
    }
    strncpy(data->info.plot.aisle, token, 2); //aisle
    data->info.plot.aisle[2] = '\0';

    token = strtok(NULL, "-"); //bay
    strncpy(data->info.plot.bay, token, 2);
    data->info.plot.bay[2] = '\0';
    
    token = strtok(NULL, "-"); //level
    strncpy(data->info.plot.level, token, 2);
    data->info.plot.level[2] = '\0';
    
    token = strtok(NULL, "-"); //position
    strncpy(data->info.plot.position, token, 2);
    data->info.plot.position[2] = '\0';
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
            printf("Data produk berhasil dihapus.\n");
            return;
        }
        curr = curr->next;
    }
    
    printf("Data produk tidak ditemukan.\n");
}

void searchProduct(hashTable* storage){
    char ID[20];
    printf("Masukkan ID: "); scanf("%s", ID); //masukkan ID
    
    int idx = djbHashFunction(ID);
    Node* curr = storage->table[idx];
    
    while(curr){
        if(strcmp(curr->info.ID, ID)==0){
            char print;
            printf("Produk ditemukan.\n Apakah Anda ingin mencetak rincian info tentang produk? (y/n)");
            scanf("%c", &print);
            if(print=='y'){
                printf("ID Produk: %s", curr);
                printf("Nama Produk: %s", curr->info.name);
                printf("Kategori: %s", curr->info.category);
                printf("Jumlah stok: %s", curr->info.numStock);
                printf("Lokasi: %s-%s-%s-%s", curr->info.plot.aisle,
                    curr->info.plot.bay, curr->info.plot.level, curr->info.plot.position); //contohnya DRY-1-B-1-A
                //PIC sama status barang belum ditambahin
                printf("Status: %s", curr->info.status);
                printf("Nama PIC: %s", curr->info.PIC);
                printf("Nama Pemilik: %s", curr->info.owner);
            }
            return;
        }
        curr = curr->next;
    }
}

void updateProduct(hashTable* storage) {
    char ID[20];
    printf("\n=== MENU PEMUTAKHIRAN DATA PRODUK ===\n");
    printf("Masukkan ID barang yang ingin diubah: ");
    scanf("%s", ID);
//masih bingung aku, 
    int idx = djbHashFunction(ID);
    
    Node* curr = storage->table[idx];

    while (curr != NULL) {
        if (strcmp(curr->info.ID, ID) == 0) { 
            int option;
            printf("\nProduk Ditemukan: %s (Kategori: %s)\n", curr->info.name, curr->info.category);
            printf("Pilih data yang ingin Anda mutakhirkan:\n");
            printf("1. Jumlah Stok\n");
            printf("2. Lokasi Penyimpanan\n");
            printf("3. Status Barang (Tersedia/Dipinjam/Rusak/Habis)\n");
            printf("4. Pemilik Barang\n");
            printf("5. PIC Barang\n");
            printf("Masukkan pilihan (1-5): ");
            scanf("%d", &option);

            while (getchar() != '\n'); 

            switch(option) {
                case 1:
                    printf("Masukkan Jumlah Stok Baru: ");
                    scanf("%d", &curr->info.numStock);
                    printf(">> [SUKSES] Jumlah stok berhasil diperbarui menjadi %d!\n", curr->info.numStock);
                    break;
                
                case 2: {
                    char newLocation[15];
                    printf("Masukkan koordinat lokasi baru (Format: ZON-Walkway-Bay-Level-Posisi, misal DRY-1-B-1-A): ");
                    fgets(newLocation, sizeof(newLocation), stdin);
                    printf(">> [SUKSES] Lokasi penyimpanan berhasil diperbarui!\n");
                    break;
                }
                
                case 3:
                    printf("Masukkan Status Baru: ");
                    fgets(curr->info.status, sizeof(curr->info.status), stdin);
                    curr->info.status[strcspn(curr->info.status, "\n")] = 0; 
                    printf(">> [SUKSES] Status berhasil diperbarui!\n");
                    break;
                
                case 4:
                    printf("Masukkan Nama Pemilik Baru: ");
                    fgets(curr->info.owner, sizeof(curr->info.owner), stdin);
                    curr->info.owner[strcspn(curr->info.owner, "\n")] = 0;
                    printf(">> [SUKSES] Pemilik barang berhasil diperbarui!\n");
                    break;
                
                case 5:
                    printf("Masukkan Nama PIC Baru: ");
                    fgets(curr->info.PIC, sizeof(curr->info.PIC), stdin);
                    curr->info.PIC[strcspn(curr->info.PIC, "\n")] = 0;
                    printf(">> [SUKSES] PIC barang berhasil diperbarui!\n");
                    break;
                
                default:
                    printf(">> [GAGAL] Pilihan instruksi tidak valid.\n");
                    break;
            }
            return; 
        }
        curr = curr->next;
    }

    printf(">> [GAGAL] Produk dengan ID %s tidak ditemukan dalam database.\n", ID);
}

//printAllProducts dan printUniques masih di local repo
void printAllProducts(hashTable* storage){
    int isEmpty = 1; //penanda barang ini kosong atau tidak

    printf("\n=== DAFTAR SELURUH INVENTARIS ===\n");

    for (int i = 0; i < 10; i++) {
        Node* curr = storage->table[i];
        
        while (curr != NULL) {
            isEmpty = 0; // Begitu ketemu minimal 1 data, flag diubah jadi 0 (tidak kosong)

            //print spesifikasi barang
            printf("----------------------------------\n");
            printf("ID Barang   : %s\n", curr->info.ID);
            printf("Nama Alat   : %s\n", curr->info.name);
            printf("Kategori    : %s\n", curr->info.category);
            printf("Jumlah Stok : %d\n", curr->info.numStock); 
            
            // Mencetak lokasi
            printf("Lokasi      : %s-%s-%s-%s\n", 
                   curr->info.plot.aisle, 
                   curr->info.plot.bay, 
                   curr->info.plot.level, 
                   curr->info.plot.position);           
                   
            printf("Status: %s\n", curr->info.status);
            printf("Nama PIC: %s\n", curr->info.PIC);
            printf("Nama Pemilik: %s\n", curr->info.owner);

            curr = curr->next; // Geser ke node/barang selanjutnya di storage yang sama
        }
    }

    // kondisi ketika data kosong
    if (isEmpty == 1) {
        printf("Data kosong! Belum ada inventaris di dalam sistem.\n");
    }
    
    printf("==================================\n\n");

}
void printUniques(hashTable* storage){
    int pilihan;
    char keyword[30];
    int countUnique;


    printf("\n=== MENCARI NILAI UNIK ===\n");
    printf("Pilih dasar klasifikasi pencarian:\n");
    printf("1. Berdasarkan Nama Barang\n");
    printf("2. Berdasarkan Kategori\n");
    printf("3. Berdasarkan Nama Pemilik (contoh: Tersedia)\n");
    printf("4. Berdasarkan Nama PIC (contoh: Tersedia)\n");
    
    printf("Masukkan pilihan Anda: ");
    scanf("%d", &pilihan);
    
    switch (pilihan)
    {
    case 1:
        printUniqueName(storage, &countUnique);
        break;
    case 2:
        printUniqueCategory(storage, &countUnique);
        break;
    case 3:
        printUniqueOwnwer(storage, &countUnique);
        break;
    case 4:
        printUniquePIC(storage, &countUnique);
        break; 
    default:
        printf("Pilihan tidak valid. Kembali ke menu utama.\n");
        break;
    }

    /**printf("\n--- Hasil Pencarian untuk '%s' ---\n", keyword);

    // Looping seluruh isi Hash Table
    for (int i = 0; i < 10; i++) {
        Node* curr = storage->table[i];
        
        while (curr != NULL) {
            int isMatch = 0; // Flag untuk menandai apakah barang cocok dengan filter

            // Cek kecocokan menggunakan strcmp 
            if (pilihan == 1 && strcmp(curr->info.category, keyword) == 0) {
                isMatch = 1;
            } else if (pilihan == 2 && strcmp(curr->info.plot.zone, keyword) == 0) {
                isMatch = 1;
            } else if (pilihan == 3 && strcmp(curr->info.status, keyword) == 0){
                isMatch = 1;
            }

            // Jika barang cocok dengan filter, cetak ke layar
            if (isMatch == 1) {
                printf("- [%s] %s (Stok: %d)\n", curr->info.ID, curr->info.name, curr->info.numStock);
                count++; 
            }

            curr = curr->next;
        }
    }**/

    // Penanganan jika tidak ada data yang cocok sesuai syarat
    printf("\nTotal: %d macam produk unik ditemukan.\n", countUnique);
    printf("==================================\n\n");
}

void mask(hashTable* storage){
    int minStock, maxStock;
    int found = 0;
    
    int pilihan;
    printf("Pilih kriteria pencarian:\n");
    printf("1. Berdasarkan Kategori\n");
    printf("2. Berdasarkan Zona Lokasi (contoh: DRY)\n");
    printf("3. Berdasarkan Status (contoh: Tersedia)\n");
    printf("4. Berdasarkan Jumlah Stok \n");

    printf("Masukkan pilihan Anda: ");
    scanf("%d", &pilihan);

    char keyword[30];

    printf("\n=== Hasil Pencarian Filter Stok (%d - %d) ===\n", minStock, maxStock);
    for (int i = 0; i < 10; i++) {
        Node* curr = storage->table[i];

        int doPrint = 0;
        while (curr != NULL) {
            switch (pilihan)
            {
                case 1:
                    printf("Masukkan nama kategori yang dicari: ");
                    scanf("%s", keyword);

                    if (strcmp(curr->info.category, keyword) == 0){
                        doPrint = 1;
                    }
                    break;

                case 2:
                    printf("Masukkan nama aisle yang dicari: ");
                    scanf("%s", keyword);

                    if (strcmp(curr->info.plot.aisle, keyword) == 0){
                        doPrint = 1;
                    }
                    break;

                case 3:
                    printf("Masukkan nama status yang ingin dicari: ");
                    scanf("%s", keyword);

                    if (strcmp(curr->info.status, keyword) == 0){
                        doPrint = 1;
                    }
                    break;

                case 4:
                    printf("Masukkan batas bawah jumlah stok: ");
                    scanf("%d", &minStock);
                    printf("Masukkan batas atas jumlah stok: ");
                    scanf("%d", &maxStock);
                    
                    if ((curr->info.numStock > minStock) && (curr->info.numStock < maxStock)){
                        doPrint = 1;
                    }
                    break;

                default:
                    break;
            }
    
            if (doPrint){
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

                printf("ID Produk   : %s\n", maskedID); //classified ID
                printf("Nama Produk : %s\n", curr->info.name);
                printf("Kategori    : %s\n", curr->info.category);
                printf("Jumlah stok : %d\n", curr->info.numStock);
            
                printf("Lokasi      : %s-%s-%s-%s\n", 
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

void printUniquePIC(hashTable* storage, int* count){
    char* seenPICs[20];
    int uniqueCount = 0;
    
    for (int i = 0; i < 10; i++)
    {
        Node* curr = storage->table[i];
        
        while(!curr){
            bool isDuplicate = false;
            for (int j = 0; j < uniqueCount; i++)
            {
                if(strcmp(curr->info.PIC, seenPICs[j])==0){
                    isDuplicate=true;
                    break;
                }
            }
            
            if(!isDuplicate){
                if(uniqueCount<20){
                    seenPICs[uniqueCount+1] = curr->info.PIC;
                    uniqueCount++;
                    printf("- %s\n", curr->info.PIC);
                }
            }
            curr = curr->next;
        }
    }
    *count = uniqueCount;
}

void printUniqueName(hashTable* storage, int* count){
    char* seenProducts[20];
    int uniqueCount = 0;
    
    for (int i = 0; i < 10; i++)
    {
        Node* curr = storage->table[i];
        
        while(!curr){
            bool isDuplicate = false;
            for (int j = 0; j < uniqueCount; i++)
            {
                if(strcmp(curr->info.name, seenProducts[j])==0){
                    isDuplicate=true;
                    break;
                }
            }
            
            if(!isDuplicate){
                if(uniqueCount<20){
                    seenProducts[uniqueCount+1] = curr->info.name;
                    uniqueCount++;
                    printf("- %s\n", curr->info.name);
                }
            }
            curr = curr->next;
        }
        
    }
    *count = uniqueCount;
}

void printUniqueCategory(hashTable* storage, int* count){
    char* seenCategories[20];
    int uniqueCount = 0;
    
    for (int i = 0; i < 10; i++)
    {
        Node* curr = storage->table[i];
        
        while(!curr){
            bool isDuplicate = false;
            for (int j = 0; j < uniqueCount; i++)
            {
                if(strcmp(curr->info.category, seenCategories[j])==0){
                    isDuplicate=true;
                    break;
                }
            }
            
            if(!isDuplicate){
                if(uniqueCount<20){
                    seenCategories[uniqueCount+1] = curr->info.category;
                    uniqueCount++;
                    printf("- %s\n", curr->info.category);
                }
            }
            curr = curr->next;
        }
        
    }
    *count = uniqueCount;
}

void printUniqueOwnwer(hashTable* storage, int* count){
    char* seenOwners[20];
    int uniqueCount = 0;
    
    for (int i = 0; i < 10; i++)
    {
        Node* curr = storage->table[i];
        
        while(!curr){
            bool isDuplicate = false;
            for (int j = 0; j < uniqueCount; i++)
            {
                if(strcmp(curr->info.owner, seenOwners[j])==0){
                    isDuplicate=true;
                    break;
                }
            }
            
            if(!isDuplicate){
                if(uniqueCount<20){
                    seenOwners[uniqueCount+1] = curr->info.owner;
                    uniqueCount++;
                    printf("- %s\n", curr->info.owner);
                }
            }
            curr = curr->next;
        }
        
    }
    *count = uniqueCount;
}