#include <stdio.h>
#define SIZE 100  // Ukuran maksimum heap

// Array untuk menyimpan heap
int heap[SIZE];

// Variabel counter untuk menunjukkan posisi index setelah elemen terakhir
int counter = 1;

// Fungsi untuk menukar dua nilai
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Fungsi untuk menambahkan elemen ke dalam Max Heap
void push(int val){
    int current = counter;      // Tempatkan elemen baru di index terakhir
    heap[current] = val;        // Simpan nilai

	// Selama
	// 1. Belum sampai root
	// 2. Parentnya lebih kecil
	// Maka: Tukar dengan parentnya
    // Naikkan elemen ke atas jika lebih besar dari parent-nya
    while(current > 1 && heap[current / 2] < heap[current]){
        swap(&heap[current], &heap[current / 2]); // Tukar dengan parent
        current = current / 2;                    // Pindah ke posisi parent
    }

    counter++;  // Naikkan jumlah elemen
    printf("Insert success for %d\n", val);
}

// Fungsi untuk menghapus elemen terbesar (root) dari heap
int pop(){
    if(counter == 1){
        printf("Heap is empty!\n");  // Heap kosong
        return -1;
    }

    int maxValue = heap[1];                      // Ambil elemen di root
    heap[1] = heap[counter - 1];                 // Pindahkan elemen terakhir ke root
    counter--;                                   // Kurangi jumlah elemen

    int current = 1;

    // Turunkan elemen ke bawah jika lebih kecil dari salah satu anaknya
    while(current * 2 < counter){
        int left = current * 2;
        int right = left + 1;
        int largest = current;

        if(left < counter && heap[left] > heap[largest])
            largest = left;
        if(right < counter && heap[right] > heap[largest])
            largest = right;

        if(largest == current) break;

        swap(&heap[current], &heap[largest]);
        current = largest;	
    }

    return maxValue;
}

// Fungsi untuk mencetak isi heap
void printHeap(){
    printf("Heap content: ");
    for(int i = 1; i < counter; i++){
        printf("%d ", heap[i]);
    }
    printf("\n============\n");
}

// Fungsi heapify (untuk heap sort)
void heapify(int arr[], int n, int i){
    int largest = i;              // Inisialisasi root sebagai terbesar
    int left = 2 * i + 1;         // Index anak kiri
    int right = 2 * i + 2;        // Index anak kanan

    // Jika anak kiri lebih besar dari root
    if(left < n && arr[left] > arr[largest])
        largest = left;

    // Jika anak kanan lebih besar dari root
    if(right < n && arr[right] > arr[largest])
        largest = right;

    // Jika largest bukan root, tukar dan teruskan heapify
    if(largest != i){
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// Fungsi untuk melakukan heap sort
void heapSort(int arr[], int n){
    // Bangun max heap dari array
    for(int i = n / 2 - 1; i >= 0; i--){
        heapify(arr, n, i);
    }

    // Satu per satu keluarkan elemen dari heap
    for(int i = n - 1; i > 0; i--){
        swap(&arr[0], &arr[i]);  // Pindahkan root ke akhir array
        heapify(arr, i, 0);      // Heapify ulang heap yang tersisa
    }
}

// Fungsi untuk mencetak array
void printArr(int arr[], int n){
    for(int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n===================\n");
}

// Fungsi utama
int main(){
    // Bagian 1: Demonstrasi Max Heap (push dan pop)
    push(20);
    push(15);
    push(13);
    push(9);
    push(5);
    push(7);
    push(30);
    printHeap();

    printf("Pop: %d\n", pop()); // Ambil elemen terbesar
    printHeap();

    // Bagian 2: Demonstrasi Heap Sort
    int arr[10] = {3, -1, 11, 23, 5, 7, 1, 2, -5, 8};
    printArr(arr, 10);
    heapSort(arr, 10);
    printArr(arr, 10);

    return 0;
}

