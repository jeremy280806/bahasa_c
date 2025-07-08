#include <stdio.h>
#define SIZE 10

int hashTable[SIZE];     // Array untuk menyimpan data
int status[SIZE];        // Status 0 = kosong, 1 = terisi

int hash(int key) {
    return key % SIZE;   // Fungsi hash sederhana
}

void insert(int key) {
    int index = hash(key);
    int i = 0;

    while (status[index] == 1 && i < SIZE) {
        index = (index + 1) % SIZE;  // Linear probing : Mencari slot kosong
        i++;
    }

    if (i == SIZE) {
        printf("Tabel penuh. Tidak bisa menyisipkan %d\n", key);
    } else {
        hashTable[index] = key;
        status[index] = 1;
        printf("Data %d dimasukkan di indeks %d\n", key, index);
    }
}

void search(int key) {
    int index = hash(key);
    int i = 0;

    while (status[index] != 0 && i < SIZE) {
        if (hashTable[index] == key) {
            printf("Data %d ditemukan di indeks %d\n", key, index);
            return;
        }
        index = (index + 1) % SIZE;
        i++;
    }

    printf("Data %d tidak ditemukan\n", key);
}

int main() {
    // Contoh penyisipan data
    insert(15);
    insert(25);
    insert(35);
    insert(5);
    insert(10);
    insert(99);

    // Contoh pencarian data
    search(25);
    search(99);

    return 0;
}

