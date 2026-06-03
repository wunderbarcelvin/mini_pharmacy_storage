#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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