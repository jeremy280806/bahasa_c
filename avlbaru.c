#include <stdio.h>      // Untuk fungsi input-output standar
#include <stdlib.h>     // Untuk fungsi malloc
#include <string.h>     // Untuk fungsi string seperti strcpy

int idCounter = 1001;   // Inisialisasi ID unik, dimulai dari 1001

// Struktur data mahasiswa
typedef struct Mahasiswa {
    int id;             // ID unik
    char nama[50];      // Nama mahasiswa
    char prodi[50];     // Program studi
    float ipk;          // IPK
} Mahasiswa;

// Struktur node AVL Tree
typedef struct Node {
    Mahasiswa data;     // Data mahasiswa
    int height;         // Tinggi node (dibutuhkan untuk AVL balancing)
    struct Node *left;  // Pointer ke anak kiri
    struct Node *right; // Pointer ke anak kanan
} Node;

// Fungsi utilitas untuk mengambil nilai maksimum
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fungsi untuk mendapatkan tinggi suatu node
int height(Node *node) {
    return node ? node->height : 0;  // Jika node NULL, kembalikan 0
}

// Membuat node baru dan mengisi datanya
Node* createNode(char nama[], char prodi[], float ipk) {
    Node *newNode = (Node*)malloc(sizeof(Node));   // Alokasi memori untuk node baru
    newNode->data.id = idCounter++;                // Generate ID otomatis, lalu increment
    strcpy(newNode->data.nama, nama);              // Salin nama ke struct
    strcpy(newNode->data.prodi, prodi);            // Salin prodi ke struct
    newNode->data.ipk = ipk;                       // Simpan nilai IPK
    newNode->height = 1;                           // Node baru selalu punya tinggi awal 1
    newNode->left = newNode->right = NULL;         // Anak kiri dan kanan NULL
    return newNode;
}

// Fungsi rotasi kanan (Right Rotate) untuk menyeimbangkan AVL
Node* rightRotate(Node *y) {
    Node *x = y->left;       // Ambil anak kiri
    Node *T2 = x->right;     // Simpan subtree kanan dari anak kiri

    x->right = y;            // Jadikan y anak kanan dari x
    y->left = T2;            // Sambungkan kembali subtree T2 ke kiri y

    // Perbarui tinggi kedua node
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;                // Return root baru
}

// Fungsi rotasi kiri (Left Rotate)
Node* leftRotate(Node *x) {
    Node *y = x->right;      // Ambil anak kanan
    Node *T2 = y->left;      // Simpan subtree kiri dari anak kanan

    y->left = x;             // Jadikan x anak kiri dari y
    x->right = T2;           // Sambungkan kembali subtree T2 ke kanan x

    // Perbarui tinggi kedua node
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;                // Return root baru
}

// Menghitung balance factor
int getBalance(Node *node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Fungsi insert AVL Tree
Node* insert(Node *node, char nama[], char prodi[], float ipk) {
    if (!node)
        return createNode(nama, prodi, ipk); // Jika node kosong, buat node baru

    // Bandingkan ID yang akan dimasukkan dengan ID node sekarang
    if (idCounter - 1 < node->data.id)
        node->left = insert(node->left, nama, prodi, ipk);   // Rekursi ke kiri
    else
        node->right = insert(node->right, nama, prodi, ipk); // Rekursi ke kanan

    // Perbarui tinggi node saat ini
    node->height = 1 + max(height(node->left), height(node->right));

    // Hitung balance factor
    int balance = getBalance(node);

    // Kasus LL (Left Left)
    if (balance > 1 && idCounter - 1 < node->left->data.id)
        return rightRotate(node);

    // Kasus RR (Right Right)
    if (balance < -1 && idCounter - 1 > node->right->data.id)
        return leftRotate(node);

    // Kasus LR (Left Right)
    if (balance > 1 && idCounter - 1 > node->left->data.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Kasus RL (Right Left)
    if (balance < -1 && idCounter - 1 < node->right->data.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // Kembalikan node (tidak perlu rotasi)
}

// Inorder traversal (cetak dari kecil ke besar)
void inorder(Node *root) {
    if (root) {
        inorder(root->left);   // Kunjungi anak kiri
        printf("ID: %d | Nama: %s | Prodi: %s | IPK: %.2f\n",
               root->data.id, root->data.nama, root->data.prodi, root->data.ipk); // Cetak data
        inorder(root->right);  // Kunjungi anak kanan
    }
}

// Fungsi utama (main)
int main() {
    Node *root = NULL;  // Inisialisasi root AVL Tree

    // Menambahkan beberapa data mahasiswa
    root = insert(root, "Ali", "Teknik Komputer", 3.45);
    root = insert(root, "Budi", "Sistem Informasi", 3.60);
    root = insert(root, "Citra", "Teknik Elektro", 3.90);

    // Cetak isi tree secara inorder (urut berdasarkan ID)
    printf("\nData Mahasiswa (Inorder Traversal AVL Tree):\n");
    inorder(root);

    return 0;
}

