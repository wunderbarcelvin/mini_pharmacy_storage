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
        printf("\n>> Selamat datang di laman manajemen stok gudang XYZ\n");
        printf(">> Silakan pilih salah satu instruksi di bawah.\n");

        printf("1. Menambahkan produk.\n");
        printf("2. Menghapus data produk.\n");
        printf("3. Menarik produk (withdraw).\n");
        printf("4. Mencari produk.\n");
        printf("5. Memutakhirkan info produk.\n");
        printf("6. Cetak semua produk.\n");
        printf("7. Mencari klasifikasi produk tertentu.\n");
        printf("8. Mencari produk tertentu dengan kunci filter tertentu.\n");
        printf("9. Keluar dari program.\n");
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
            withdrawProduct(storage);
            break;
        case 4:
            searchProduct(storage);
            break;
        case 5:
            updateProduct(storage);
            break;
        case 6:
            printAllProducts(storage);
            break;
        case 7:
            printUniques(storage);
            break;
        case 8:
            mask(storage);
            break;
        case 9:
            printf(">> Keluar dari program..\n");
            return;
        default:
            printf(">> Perintah tidak dikenali. Coba lagi.\n");
            break;
        }
    } while (choice!=9);
}



/**Setiap data inventaris setidaknya memuat: ID barang, nama komponen/peralatan, kategori 
barang, jumlah stok, lokasi penyimpanan, status barang (misalnya tersedia, dipinjam, rusak, atau habis), pemilik barang serta PIC untuk barang tersebut.
*/

void addData(hashTable* storage){
    char inputID[10];
    
    printf("Masukkan ID: "); scanf("%s", inputID); //masukkan ID
    if(strcmp(inputID, "cancel")==0){
        printf(">> Perintah dibatalkan. Kembali ke menu utama..\n");
        return;
    }

    int idx = djbHashFunction(inputID);

    //printf("Masukkan nama alat/barang: "); scanf("%s", new->info.name); //masukkan nama
    
    Node* temp = storage->table[idx];
    Node* prev = NULL; //pointer untuk menyimpan alamat terakhir setelah dikenai hash function
    //hati-hati pointer tanpa inisiasi nilai awal
    
    while(temp!=NULL){
        if(strcmp(temp->info.ID, inputID)==0){
            int add;
            printf(">> Produk sudah pernah ditambahkan.\n");
            printf("Masukkan jumlah stok tambahan: "); scanf("%d", &add);
            temp->info.numStock+=add;
            printf("[SUKSES] Jumlah produk berhasil diupdate.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf(">> Produk belum pernah ditambahkan.\n");

    Node* new = malloc(sizeof(Node));
    if(!new){
        perror("[GAGAL] Gagal mengalokasikan memori.\n");
        return;
    }
    
    new->next = NULL;
    strcpy(new->info.ID, inputID);

    int c;
    while ((c = getchar()) != '\n' && c != EOF); //ini diperlukan untuk clear buffer

    char line[15];
    printf("Masukkan nama barang: ");
    fgets(line, sizeof(line), stdin);

    line[strcspn(line, "\n")] = 0;
    strcpy(new->info.name, line);

    printf("Masukkan kategori: ");
    //scanf("%s", new->info.category); //masukkan kategori
    
    fgets(line, sizeof(line), stdin);

    line[strcspn(line, "\n")] = 0;
    strcpy(new->info.category, line);

    printf("Masukkan jumlah stok awal: ");
    scanf("%d", &new->info.numStock);

    char location[15];
    printf("Masukkan koordinat lokasi: ");
    scanf("%s", location);
    storeLocation(new, location);
    
    //karena baru ditambahkan, maka statusnya langsung available.
    strcpy(new->info.status, "Tersedia");

    printf("Masukkan nama PIC: ");
    scanf("%s", new->info.PIC);

    printf("Masukkan nama pemilik: ");
    scanf("%s", new->info.owner);
    

    //store to the hash table
    if(storage->table[idx]==NULL){
        storage->table[idx] = new;
    } else{
        prev->next = new;
    }
    
    printf("[SUKSES] Produk berhasil ditambahkan.\n");
}


void storeLocation(Node* data, char* coordinate){
    //template koordinat: zona - aisle - bay - level - position
    char* token = strtok(coordinate, "-");
    
    if(!token){
        perror(">> Gagal parsing.");
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

void removeData(hashTable* storage){ //menggunakan
    char ID[20];
    printf("Masukkan ID: "); scanf("%s", ID); //masukkan ID
    
    if(strcmp(ID, "cancel")==0){
        printf(">> Perintah dibatalkan. Kembali ke menu utama..\n");
        return;
    }
    
    int idx = djbHashFunction(ID);
    Node* curr = storage->table[idx];
    
    Node* prev=NULL;
    while(curr){
        if(strcmp(curr->info.ID, ID)==0){
            if(prev==NULL){ //jika di urutan terdepan dari indexing hash table
                storage->table[idx]=curr->next;
            } else{
                prev->next = curr->next;
            }
            free(curr);
            printf("[SUKSES] Data produk berhasil dihapus.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    
    printf("[GAGAL] Data produk tidak ditemukan.\n");
}

void withdrawProduct(hashTable* storage){ //fungsi baru
    char ID[20];
    printf("Masukkan ID: "); scanf("%s", ID); //masukkan ID
    
    //untuk membatalkan perintah
    if(strcmp(ID, "cancel")==0){
        printf(">> Perintah dibatalkan. Kembali ke menu utama..\n");
        return;
    }
    
    int idx = djbHashFunction(ID);
    Node* curr = storage->table[idx];
    
    while(curr){
        if(strcmp(curr->info.ID, ID)==0){
            while(1){
                int numReq;
                printf("Masukkan jumlah barang yang akan digunakan: ");
                
                if(scanf("%d", &numReq) != 1){
                    printf("[GAGAL] Input tidak valid! Harap masukkan angka.\n");
                    while (getchar() != '\n')
                    continue;;
                }
                
                if(numReq==0){
                    printf("[GAGAL] Coba masukkan angka lebih dari nol.\n");
                    continue;
                }
                
                if(curr->info.numStock - numReq>=0){
                    curr->info.numStock-=numReq;
                    printf("[SUKSES] Produk %s sejumlah %d unit berhasil ditarik.\n", curr->info.name, numReq);
                    
                    if(curr->info.numStock==0){
                        strcpy(curr->info.status, "Habis");
                    }
                    return;
                }

                printf("[GAGAL] Jumlah unit yang diminta melebihi stok. Coba lagi.\n");
            }
           
        }
        curr = curr->next;
    }
    printf("[GAGAL] Data produk tidak ditemukan.\n");
}

void searchProduct(hashTable* storage){
    char ID[20];
    printf("Masukkan ID: "); scanf("%s", ID); //masukkan ID
    
    //untuk membatalkan perintah
    if(strcmp(ID, "cancel")==0){
        printf(">> Perintah dibatalkan. Kembali ke menu utama..\n");
        return;
    }

    int idx = djbHashFunction(ID);
    Node* curr = storage->table[idx];
    
    while(curr){
        if(strcmp(curr->info.ID, ID)==0){
            char print;
            printf(">> Produk ditemukan.\n Apakah Anda ingin mencetak rincian info tentang produk? (y/n): ");
            scanf(" %c", &print);
            if(print=='y'|| print=='Y'){
                printf("=== DETAIL PRODUK ===\n");
                printf("ID Produk.  : %s\n", curr->info.ID);
                printf("Nama Produk : %s\n", curr->info.name);
                printf("Kategori.   : %s\n", curr->info.category);
                printf("Jumlah stok : %d\n", curr->info.numStock);
                printf("Lokasi.     : %s-%s-%s-%s\n", curr->info.plot.aisle,
                    curr->info.plot.bay, curr->info.plot.level, curr->info.plot.position); //contohnya DRY-1-B-1-A
                //PIC sama status barang belum ditambahin
                printf("Status.     : %s\n", curr->info.status);
                printf("Nama PIC.   : %s\n", curr->info.PIC);
                printf("Nama Pemilik: %s\n", curr->info.owner);
            }
            return;
        }
        curr = curr->next;
    }
    printf("[GAGAL} Data produk tidak ditemukan.\n");
}

void updateProduct(hashTable* storage) { //UPDATE ADA YANG DIUBAH
    char ID[20];
    printf("\n=== MENU PEMUTAKHIRAN DATA PRODUK ===\n");
    printf("Masukkan ID barang yang ingin diubah: ");
    scanf("%s", ID);

    //untuk membatalkan perintah
    if(strcmp(ID, "cancel")==0){
        printf(">> Perintah dibatalkan. Kembali ke menu utama..\n");
        return;
    }

    int idx = djbHashFunction(ID);
    Node* curr = storage->table[idx];

    while (curr != NULL) {
        if (strcmp(curr->info.ID, ID) == 0) { 
            int option;
            printf("\nProduk Ditemukan: %s (Kategori: %s)\n", curr->info.name, curr->info.category);
            printf("Pilih data yang ingin Anda mutakhirkan:\n");
            printf("1. Lokasi Penyimpanan\n");
            printf("2. Status Barang (Tersedia/Dipinjam/Rusak/Habis)\n");
            printf("3. Pemilik Barang\n");
            printf("4. PIC Barang\n");
            printf("Masukkan pilihan (1-4): ");
            scanf("%d", &option);

            while (getchar() != '\n'); //besihkan angka setelah menu

            switch(option){
                case 1:{
                    char newLocation[15];
                    printf("Masukkan koordinat lokasi baru (Format: Aisle-Bay-Level-Posisi, misal A1-B1-L1-P1): ");
                    
                    scanf("%s", newLocation);
                    storeLocation(curr, newLocation);
                    printf(">> [SUKSES] Lokasi penyimpanan berhasil diperbarui!\n");
                    break;
                }
                                   
                case 2:{
                    char newStatus[8];
                    printf("Masukkan Status Baru: ");
                    scanf("%s", newStatus);

                    if(strcmp(newStatus, "Habis")==0){
                        if(curr->info.numStock!=0){
                            printf("[GAGAL] Request produk terlebih dulu supaya jumlahnya berkurang.\n"); //harus withdrawProduct terlebih dulu
                            return;
                        } else{
                            strcpy(curr->info.status, newStatus);
                        }
                    } else{
                        strcpy(curr->info.status, newStatus);
                    }
                    
                    printf(">> [SUKSES] Status berhasil diperbarui!\n");
                    break;
                }
                    
                
                case 3:
                    printf("Masukkan Nama Pemilik Baru: ");
                    fgets(curr->info.owner, sizeof(curr->info.owner), stdin);
                    curr->info.owner[strcspn(curr->info.owner, "\n")] = 0;
                    printf(">> [SUKSES] Pemilik barang berhasil diperbarui!\n");
                    break;
                
                case 4:
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

            //print spesifikasi barang
            printf("----------------------------------\n");
            printf("ID Barang   : %s\n", maskedID);
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
        return;
    }

    // Penanganan jika tidak ada data yang cocok sesuai syarat
    printf("\nTotal: %d macam produk unik ditemukan.\n", countUnique);
    printf("==================================\n\n");
}

void mask(hashTable* storage){ //MASK ADA YANG DIUBAH
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
    switch (pilihan){
        case 1:
            printf("Masukkan nama kategori yang dicari: ");
            scanf("%s", keyword);
            break;

        case 2:
            printf("Masukkan nama aisle yang dicari: ");
            scanf("%s", keyword);
            break;

        case 3:
            printf("Masukkan nama status yang ingin dicari: ");
            scanf("%s", keyword);
            break;

        case 4:
            printf("Masukkan batas bawah jumlah stok: ");
            scanf("%d", &minStock);
            printf("Masukkan batas atas jumlah stok: ");
            scanf("%d", &maxStock);
            break;

        default:
            break;
    }

    printf("\n=== Hasil Pencarian Filter Stok ===\n");
    for (int i = 0; i < 10; i++) {
        Node* curr = storage->table[i];

        while (curr != NULL) {
            int doPrint = 0;
            switch (pilihan)
            {
                case 1:
                    if (strcmp(curr->info.category, keyword) == 0){
                        doPrint = 1;
                        found = 1;
                    }
                    break;
                case 2:
                    if (strcmp(curr->info.plot.aisle, keyword) == 0){
                        doPrint = 1;
                        found = 1;
                    }
                    break;

                case 3:
                    if (strcmp(curr->info.status, keyword) == 0){
                        doPrint = 1;
                        found = 1;
                    }
                    break;

                case 4:
                    if ((curr->info.numStock > minStock) && (curr->info.numStock < maxStock)){
                        doPrint = 1;
                        found = 1;
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
        
        while(curr){
            bool isDuplicate = false;
            for (int j = 0; j < uniqueCount; j++)
            {
                if(strcmp(curr->info.PIC, seenPICs[j])==0){
                    isDuplicate=true;
                    break;
                }
            }
            
            if(!isDuplicate){
                if(uniqueCount<20){
                    seenPICs[uniqueCount] = curr->info.PIC;
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
        
        while(curr){
            bool isDuplicate = false;
            for (int j = 0; j < uniqueCount; j++)
            {
                if(strcmp(curr->info.name, seenProducts[j])==0){
                    isDuplicate=true;
                    break;
                }
            }
            
            if(!isDuplicate){
                if(uniqueCount<20){
                    seenProducts[uniqueCount] = curr->info.name;
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
        
        while(curr){
            bool isDuplicate = false;
            for (int j = 0; j < uniqueCount; j++)
            {
                if(strcmp(curr->info.category, seenCategories[j])==0){
                    isDuplicate=true;
                    break;
                }
            }
            
            if(!isDuplicate){
                if(uniqueCount<20){
                    seenCategories[uniqueCount] = curr->info.category; //harusnya indeksnya adalah uniqueCount
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
        
        while(curr){
            bool isDuplicate = false;
            for (int j = 0; j < uniqueCount; j++)
            {
                if(strcmp(curr->info.owner, seenOwners[j])==0){
                    isDuplicate=true;
                    break;
                }
            }
            
            if(!isDuplicate){
                if(uniqueCount<20){
                    seenOwners[uniqueCount] = curr->info.owner;
                    uniqueCount++;
                    printf("- %s\n", curr->info.owner);
                }
            }
            curr = curr->next;
        }
        
    }
    *count = uniqueCount;
}