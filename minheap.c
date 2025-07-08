#include <stdio.h>
#define SIZE 100  // Ukuran maksimum heap

// Array untuk menyimpan heap
int heap[SIZE];
int counter = 1; // Menunjukkan index setelah elemen terakhir (heap dimulai dari index 1)

// Fungsi untuk menukar dua nilai
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


// Fungsi untuk menambahkan elemen ke dalam Min Heap
void push(int val){
    int current = counter;        // Tempatkan elemen baru di posisi terakhir
    heap[current] = val;          // Simpan nilai di array heap

    // Percolate up: selama parent lebih besar, tukar posisi
    while(current > 1 && heap[current / 2] > heap[current]){
        swap(&heap[current], &heap[current / 2]);
        current = current / 2;
    }

    counter++;  // Tambah jumlah elemen heap
    printf("Insert success for %d\n", val);
}

// Fungsi untuk menghapus elemen terkecil (root) dari Min Heap
int pop(){
    if(counter == 1){
        printf("Heap is empty!\n");
        return -1;
    }

    int minValue = heap[1];                    // Ambil nilai terkecil (di root)
    heap[1] = heap[counter - 1];               // Ganti root dengan elemen terakhir
    counter--;                                 // Kurangi jumlah elemen

    int current = 1;

    // Percolate down: selama ada anak yang lebih kecil, tukar posisi
    while(current * 2 < counter){
        int left = current * 2;
        int right = left + 1;
        int smallest = current;

        if(left < counter && heap[left] < heap[smallest])
            smallest = left;
        if(right < counter && heap[right] < heap[smallest])
            smallest = right;

        if(smallest == current) break;  // Sudah di posisi yang benar

        swap(&heap[current], &heap[smallest]);
        current = smallest;
    }

    return minValue;
}

// Fungsi untuk mencetak isi heap
void printHeap(){
    printf("Heap content: ");
    for(int i = 1; i < counter; i++){
        printf("%d ", heap[i]);
    }
    printf("\n============\n");
}

// Fungsi utama
int main(){
    // Masukkan data ke Min Heap
    push(30);
    push(20);
    push(40);
    push(10);
    push(35);
    push(5);

    // Cetak heap saat ini
    printHeap();  // Harus menunjukkan struktur Min Heap

    // Keluarkan elemen terkecil
    printf("Pop: %d\n", pop());
    printHeap();

    printf("Pop: %d\n", pop());
    printHeap();

    return 0;
}

