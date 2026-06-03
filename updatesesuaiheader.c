#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void updateProduct(hashTable* storage) {
    char ID[20];
    printf("\n=== MENU PEMUTAKHIRAN DATA PRODUK ===\n");
    printf("Masukkan ID barang yang ingin diubah: ");
    scanf("%s", ID);

    int idx = djbHashFunction(ID);
    
    Node* curr = storage->table[idx];

    while (curr != NULL) {
        if (strcmp(curr->info.ID, ID) == 0) { 
            int option;
            printf("\nProduk Ditemukan: %s (Stok saat ini: %d)\n", curr->info.name, curr->info.numStock);
            printf("Pilih data yang ingin Anda mutakhirkan:\n");
            printf("1. Jumlah Stok\n");
            printf("2. Lokasi Penyimpanan\n");
            printf("Masukkan pilihan (1 atau 2): ");
            scanf("%d", &option);

            while (getchar() != '\n'); 

            if (option == 1) {
                printf("Masukkan Jumlah Stok Baru: ");
                scanf("%d", &curr->info.numStock);
                printf(">> [SUKSES] Jumlah stok berhasil diperbarui menjadi %d!\n", curr->info.numStock);
            
            } else if (option == 2) {
                char newLocation[15];
                printf("Masukkan koordinat lokasi baru (Format ZON-Lorong-Rak-Level-Posisi, misal DRY-1-B-1-A): ");
                fgets(newLocation, sizeof(newLocation), stdin);
                
                storeLocation(curr, newLocation);
                printf(">> [SUKSES] Lokasi penyimpanan berhasil diperbarui!\n");
            
            } else {
                printf(">> [GAGAL] Pilihan instruksi tidak valid.\n");
            }
            
            return; 
        
        curr = curr->next;
    }

    printf(">> [GAGAL] Produk dengan ID %s tidak ditemukan dalam database.\n", ID);
}