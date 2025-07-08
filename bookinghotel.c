#include <stdio.h>      // Untuk fungsi input/output: printf, scanf, etc
#include <stdlib.h>     // Untuk fungsi malloc, free, system, rand
#include <string.h>     // Untuk manipulasi string: strcpy, strcmp, strlen, strncmp
#include <ctype.h>      // Untuk fungsi karakter: toupper
#include <time.h>       // Untuk fungsi waktu: srand, time

#define SIZE_TABLE 100  // Ukuran tabel hash

int countBooking = 0;   // Menyimpan jumlah booking yang sudah dibuat

// Struktur data untuk 1 booking
typedef struct Booking {
    char id[6];                // ID unik booking (misal: "RE001")
    char name[31];            // Nama pemesan (maksimal 30 karakter)
    char phone[16];           // Nomor telepon (misal: "+62 12345678910")
    int age;                  // Umur pemesan
    char roomType[8];         // Jenis kamar: "Regular", "Deluxe", "Suite"
    int duration;             // Lama menginap (dalam malam)
    struct Booking* next;     // Pointer ke booking berikutnya (linked list)
} Booking;

Booking* hashTable[SIZE_TABLE];
//struct Booking* hashTable[SIZE_TABLE];
//Booking* hashTable[SIZE_TABLE];  // Tabel hash berisi pointer ke Booking


// Membuat node booking baru dan mengisi datanya
Booking* createBooking(char name[], char phone[], int age, char roomType[], int duration) {
    Booking* newBooking = (Booking*) malloc(sizeof(Booking)); // Alokasi memori
    strcpy(newBooking->name, name);                           // Salin nama
    strcpy(newBooking->phone, phone);                         // Salin nomor telepon
    newBooking->age = age;                                    // Isi umur
    strcpy(newBooking->roomType, roomType);                   // Salin tipe kamar
    newBooking->duration = duration;                          // Isi durasi inap
    return newBooking;                                        // Kembalikan pointer node
}

// Fungsi hash untuk menentukan index hashTable berdasarkan ID booking
int hash(char* id) {
    int threeDigits = atoi(id + 2); // Ambil 3 digit terakhir dari ID (misal RE123 -> 123)
    int sum = (threeDigits / 100) + ((threeDigits / 10) % 10) + (threeDigits % 10) - 1;
    return sum % SIZE_TABLE;       // Kembalikan index di dalam ukuran tabel hash
}

// Membuat ID booking dari prefix tipe kamar + angka random
void generateBookingID(char* id, char* roomType) {
    char prefix[3];
    prefix[0] = toupper(roomType[0]);     // Huruf pertama tipe kamar (huruf besar)
    prefix[1] = toupper(roomType[1]);     // Huruf kedua tipe kamar
    prefix[2] = '\0';                     // Null-terminator string

    int randomNum = rand() % 1000;        // Angka acak 0–999
    sprintf(id, "%s%03d", prefix, randomNum); // Format ID: misal "RE042"
}

// Validasi nomor telepon harus mulai dengan +62 dan panjang tepat 15 karakter
int isValidPhone(char* phone) {
    return strncmp(phone, "+62", 3) == 0 && strlen(phone) == 15 && phone[3] == ' ';
}

// Validasi tipe kamar hanya bisa Regular, Deluxe, atau Suite
int isValidType(char* roomType) {
    return strcmp(roomType, "Regular") == 0 ||
           strcmp(roomType, "Deluxe") == 0 ||
           strcmp(roomType, "Suite") == 0;
}

// Masukkan node Booking ke dalam hashTable menggunakan chaining (linked list)
void insertNode(Booking* newBooking) {
    int hashKey = hash(newBooking->id); // Hitung indeks hash

    if (hashTable[hashKey] == NULL) {
        hashTable[hashKey] = newBooking;   // Jika kosong, langsung isi
    } else {
        Booking* temp = hashTable[hashKey]; // Jika sudah ada, cari ujung linked list
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newBooking; // Tambahkan di akhir
    }
}

// Fungsi utama untuk menginput data booking
void insertBooking() {
    Booking* newBooking = createBooking("Budi", "+62 12345678910", 20, "Regular", 3); // Dummy awal

    // Input Nama
    do {
        printf("Input Full Name [3..30]: ");
        scanf("%[^\n]", newBooking->name); getchar();
        if (strlen(newBooking->name) < 3 || strlen(newBooking->name) > 30) {
            printf("Full name length must be between 3 and 30\n");
        }
    } while (strlen(newBooking->name) < 3 || strlen(newBooking->name) > 30);

    // Input Nomor Telepon
    do {
        printf("Input Phone Number: ");
        scanf("%[^\n]", newBooking->phone); getchar();
        if (!isValidPhone(newBooking->phone)) {
            printf("Invalid Phone Number\n");
        }
    } while (!isValidPhone(newBooking->phone));

    // Input Umur
    do {
        printf("Input Age: ");
        scanf("%d", &newBooking->age); getchar();
        if (newBooking->age < 18) {
            printf("Age must be minimum 18\n");
        }
    } while (newBooking->age < 18);

    // Input Tipe Kamar
    do {
        printf("Input Room Type: ");
        scanf("%[^\n]", newBooking->roomType); getchar();
        if (!isValidType(newBooking->roomType)) {
            printf("Room Type is not Valid!\n");
        }
    } while (!isValidType(newBooking->roomType));

    generateBookingID(newBooking->id, newBooking->roomType); // Buat ID otomatis
    insertNode(newBooking);     // Masukkan ke hashTable
    countBooking++;             // Tambahkan jumlah booking

    // Tampilkan data hasil input
    printf("Booking ID: %s\n", newBooking->id);
    printf("Name: %s\n", newBooking->name);
    printf("Phone: %s\n", newBooking->phone);
    printf("Room Type: %s\n", newBooking->roomType);
    printf("Night Stay: %d night(s)\n", newBooking->duration);

    system("pause");
    return;
}

// Menampilkan semua booking yang tersimpan di hashTable
void viewBooking() {
    if (countBooking == 0) {
        printf("There is no booking!\n");
        system("pause");
        return;
    }

    for (int i = 0; i < SIZE_TABLE; i++) {
        Booking* temp = hashTable[i];
        while (temp != NULL) {
            printf("Booking ID: %s\n", temp->id);
            printf("Name: %s\n", temp->name);
            printf("Phone: %s\n", temp->phone);
            printf("Room Type: %s\n", temp->roomType);
            printf("Night Stay: %d night(s)\n", temp->duration);
            printf("=======================================\n");
            temp = temp->next;
        }
    }
}

// Menghapus booking berdasarkan ID
void removeBooking() {
    viewBooking();

    if (countBooking == 0) {
        printf("There is no booking!\n");
        system("pause");
        return;
    }

    char id[6];
    printf("Input Booking ID (Case Sensitive): ");
    fgets(id, sizeof(id), stdin);

    int hashKey = hash(id);
    Booking* temp = hashTable[hashKey];

    if (temp == NULL) {
        printf("Failed to Delete, There is No Data!\n");
        system("pause");
        return;
    }

    if (strcmp(temp->id, id) == 0) {
        hashTable[hashKey] = temp->next;
        free(temp);
        countBooking--;
        printf("BookingID %s is Successfully Deleted!\n", id);
        system("pause");
        return;
    }

    while (temp->next != NULL) {
        if (strcmp(temp->next->id, id) == 0) {
            Booking* toDelete = temp->next;
            temp->next = toDelete->next;
            free(toDelete);
            countBooking--;
            printf("BookingID %s is Successfully Deleted!\n", id);
            system("pause");
            return;
        }
        temp = temp->next;
    }

    printf("Failed to Delete, There is No Data!\n");
}

// Menu utama program
void menu() {
    int choice = 0;
    do {
        system("cls");
        puts("Hotel GrAnsylvania");
        puts("1. Booking Hotel");
        puts("2. View Booking");
        puts("3. Delete Booking");
        puts("4. Exit");
        printf("> ");
        scanf("%d", &choice); getchar();

        switch (choice) {
            case 1: insertBooking(); break;
            case 2: viewBooking(); break;
            case 3: removeBooking(); break;
            case 4: puts("Thank You for Using Our Service!"); break;
            default:
                puts("Invalid Choice!");
                system("pause");
                break;
        }
    } while (choice != 4);
}

// Fungsi utama program	
int main() {
    srand(time(NULL));  // Untuk seed angka acak
    menu();             // Jalankan menu utama
    return 0;
}

