#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

// Struktur node untuk linked list
struct Node {
    int data;
    struct Node* next;
};

// Array of pointers (setiap index menunjuk ke linked list)
struct Node* hashTable[SIZE];

// Fungsi hash
int hash(int key) {
    return key % SIZE;
}

// Fungsi untuk membuat node baru
struct Node* createNode(int key) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = key;
    newNode->next = NULL;
    return newNode;
}

// Fungsi insert (pakai chaining)
void insert(int key) {
    int index = hash(key);
    struct Node* newNode = createNode(key);

    if (hashTable[index] == NULL) {
        hashTable[index] = newNode;
    } else {
        struct Node* temp = hashTable[index];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    printf("Data %d dimasukkan ke indeks %d\n", key, index);
}

// Fungsi search
void search(int key) {
    int index = hash(key);
    struct Node* temp = hashTable[index];

    while (temp != NULL) {
        if (temp->data == key) {
            printf("Data %d ditemukan di indeks %d\n", key, index);
            return;
        }
        temp = temp->next;
    }

    printf("Data %d tidak ditemukan\n", key);
}

// Fungsi untuk menampilkan isi hash table
void display() {
    printf("\nIsi Hash Table:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("[%d] -> ", i);
        struct Node* temp = hashTable[i];
        while (temp != NULL) {
            printf("%d -> ", temp->data);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main() {
    insert(15);
    insert(25);
    insert(35);
    insert(5);
    insert(10);

    search(25);
    search(99);

    display();

    return 0;
}

