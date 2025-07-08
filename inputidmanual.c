	
#include <stdio.h>      // Untuk fungsi input-output
#include <stdlib.h>     // Untuk fungsi malloc, free
#include <string.h>     // Untuk manipulasi string
#include <ctype.h>      // Untuk fungsi toupper (huruf besar)
#include <time.h>       // Untuk fungsi waktu (srand, time)

// Struktur data untuk menyimpan data booking hotel
typedef struct Booking {
    char id[6];          // ID Booking, misalnya: RE123
    char name[31];       // Nama customer
    char phone[16];      // Nomor telepon
    int age;             // Umur
    char roomType[8];    // Tipe kamar: Regular, Deluxe, Suite
    int duration;        // Lama menginap (dalam malam)
} Booking;

// Struktur node untuk AVL Tree
typedef struct AVLNode {
    Booking data;                // Data booking
    int height;                  // Tinggi node
    struct AVLNode *left, *right; // Pointer ke anak kiri dan kanan
} AVLNode;

// Fungsi mencari maksimum dari dua angka
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Mengambil tinggi node (jika NULL, kembalikan 0)
int height(AVLNode* node) {
    return node ? node->height : 0;
}

// Mengambil balance factor node
int getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Membuat node AVL baru
AVLNode* createNode(Booking data) {
    AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
    newNode->data = data;
    newNode->height = 1; // Node baru punya tinggi 1
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Rotasi kanan pada node y
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

// Rotasi kiri pada node x
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

/// Membandingkan dua ID booking
int compareID(char* id1, char* id2) {
    return strcmp(id1, id2);  // Mengembalikan hasil perbandingan string ID
}

// Membuat ID Booking dari roomType dan angka
void generateBookingID(char* id, char* roomType, int number) {
    char prefix[3];                          // Menyimpan 2 huruf awal dari roomType
    prefix[0] = toupper(roomType[0]);        // Huruf pertama dikapitalisasi
    prefix[1] = toupper(roomType[1]);        // Huruf kedua dikapitalisasi
    prefix[2] = '\0';                        // Null-terminator string
    sprintf(id, "%s%03d", prefix, number % 1000);  // Format ID: 2 huruf + 3 digit angka (misal: SU042)
}

// Menyisipkan node ke dalam AVL Tree
AVLNode* insertNode(AVLNode* node, Booking data) {
    if (!node) return createNode(data);  // Jika tree kosong, buat node baru
	
    // Bandingkan ID booking, untuk memutuskan arah penyisipan
    if (compareID(data.id, node->data.id) < 0)
        node->left = insertNode(node->left, data);   // Masuk ke subtree kiri
    else if (compareID(data.id, node->data.id) > 0)
        node->right = insertNode(node->right, data); // Masuk ke subtree kanan
    else
        return node;  // ID sudah ada, tidak disisipkan ulang

    // Perbarui tinggi node
    node->height = 1 + max(height(node->left), height(node->right));

    // Hitung selisih tinggi (balance factor)
    int balance = getBalance(node);

    // Lakukan rotasi jika tidak seimbang
    if (balance > 1 && compareID(data.id, node->left->data.id) < 0)
        return rightRotate(node); // Left Left Case
    if (balance < -1 && compareID(data.id, node->right->data.id) > 0)
        return leftRotate(node);  // Right Right Case
    if (balance > 1 && compareID(data.id, node->left->data.id) > 0) {
        node->left = leftRotate(node->left);         // Left Right Case
        return rightRotate(node);
    }
    if (balance < -1 && compareID(data.id, node->right->data.id) < 0) {
        node->right = rightRotate(node->right);      // Right Left Case
        return leftRotate(node);
    }

    return node;  // Node tidak perlu rotasi, cukup dikembalikan
}

// Mencari node terkecil (paling kiri)
AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL)          // Bergerak terus ke kiri
        current = current->left;
    return current;                        // Kembali node terkecil
}

// Menghapus node dari AVL Tree berdasarkan ID
AVLNode* deleteNode(AVLNode* root, char id[]) {
    if (!root) return root; // Jika tree kosong, tidak ada yang dihapus

    // Bandingkan ID untuk menentukan arah penghapusan
    if (compareID(id, root->data.id) < 0)
        root->left = deleteNode(root->left, id);
    else if (compareID(id, root->data.id) > 0)
        root->right = deleteNode(root->right, id);
    else {
        // Node dengan 1 anak atau tanpa anak
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            free(root);       // Hapus node
            return temp;      // Kembalikan anaknya (bisa NULL)
        }

        // Node dengan 2 anak: cari pengganti terkecil dari kanan
        AVLNode* temp = minValueNode(root->right);
        root->data = temp->data;  // Salin data pengganti
        root->right = deleteNode(root->right, temp->data.id); // Hapus pengganti
    }

    // Update tinggi
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);  // Hitung balance factor

    // Periksa 4 kemungkinan rotasi
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root); // Left Left
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left); // Left Right
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root); // Right Right
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right); // Right Left
        return leftRotate(root);
    }

    return root; // Kembalikan root baru setelah penghapusan
}

// Menampilkan data booking secara urut (dari terkecil ke terbesar berdasarkan ID)
void inOrder(AVLNode* root) {
    if (root) {
        inOrder(root->left);  // Kunjungi subtree kiri
        printf("ID: %s | Name: %s | Phone: %s | Type: %s | Duration: %d\n",
               root->data.id, root->data.name, root->data.phone,
               root->data.roomType, root->data.duration);
        inOrder(root->right); // Kunjungi subtree kanan
    }
}

// Fungsi menu utama program
void menu() {
    AVLNode* root = NULL;   // Inisialisasi root tree kosong
    int choice;             // Variabel pilihan menu
    do {
        // Tampilkan menu pilihan
        printf("\n==== HOTEL AVL MENU ====\n");
        printf("1. Insert Booking (Manual ID)\n");
        printf("2. Insert Booking (Auto ID from number)\n");
        printf("3. View All Bookings\n");
        printf("4. Delete Booking by ID\n");
        printf("5. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice); getchar(); // Ambil input user

        // Proses menu insert
        if (choice == 1 || choice == 2) {
            Booking b;

            // Input data pelanggan
            printf("Input Name: "); fgets(b.name, sizeof(b.name), stdin); b.name[strcspn(b.name, "\n")] = 0;
            printf("Input Phone: "); fgets(b.phone, sizeof(b.phone), stdin); b.phone[strcspn(b.phone, "\n")] = 0;
            printf("Input Age: "); scanf("%d", &b.age); getchar();
            printf("Input Room Type (Regular/Deluxe/Suite): ");
            fgets(b.roomType, sizeof(b.roomType), stdin); b.roomType[strcspn(b.roomType, "\n")] = 0;
            printf("Input Duration (nights): "); scanf("%d", &b.duration); getchar();

            if (choice == 1) {
                // Manual input ID
                printf("Input Booking ID (format: XX999): ");
                fgets(b.id, sizeof(b.id), stdin); b.id[strcspn(b.id, "\n")] = 0;
            } else {
                // Auto generate ID dari angka + tipe kamar
                int num;
                printf("Input unique number (will be combined into ID): ");
                scanf("%d", &num); getchar();
                generateBookingID(b.id, b.roomType, num);
            }

            root = insertNode(root, b);  // Simpan ke AVL Tree
            printf("Booking saved with ID: %s\n", b.id);
        }	
        else if (choice == 3) {
            printf("=== All Bookings ===\n");
            inOrder(root);  // Cetak semua data booking
        }
        else if (choice == 4) {
            // Hapus data berdasarkan ID
            char id[6];
            printf("Input ID to Delete: ");
            fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = 0;
            root = deleteNode(root, id);
            printf("Deleted if exits.\n");
        }
    } while (choice != 5);  // Ulangi sampai user pilih Exit
}


// Fungsi utama program
int main() {
    srand(time(NULL)); // Untuk mengacak angka jika diperlukan
    menu(); // Panggil menu interaktif
    return 0; // Program selesai
}
