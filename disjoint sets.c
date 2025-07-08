#include <stdio.h>          // Untuk fungsi input/output (printf)
#include <stdlib.h>         // Untuk alokasi memori (malloc, free)
#include <stdbool.h>        // Untuk tipe data boolean: true, false

#define MAX_VERTICES 255    // Batas maksimum simpul (vertex) dalam graf

int parent[MAX_VERTICES];   // Array untuk menyimpan parent dari tiap vertex dalam Disjoint Set

// Struktur untuk sisi (edge) pada graf
struct Edge {
    int source;             // Titik awal sisi
    int destination;        // Titik tujuan sisi
};

// Struktur graf
struct Graph {
    int verticesCount;      // Jumlah simpul (vertex)
    int edgesCount;         // Jumlah sisi (edge)
    struct Edge *edge;      // Array dinamis berisi semua sisi
};

// Fungsi untuk membuat graf dengan jumlah simpul dan sisi tertentu
struct Graph *createGraph(int verticesCount, int edgesCount){
    struct Graph *newGraph = (struct Graph*)malloc(sizeof(struct Graph));         // Alokasi memori untuk struktur graf
    newGraph->verticesCount = verticesCount;                                      // Simpan jumlah simpul
    newGraph->edgesCount = edgesCount;                                            // Simpan jumlah sisi
    newGraph->edge = (struct Edge*)malloc(sizeof(struct Edge) * edgesCount);     // Alokasi memori untuk array sisi
    return newGraph;                                                              // Kembalikan graf baru
}

// Fungsi untuk inisialisasi Disjoint Set (setiap vertex jadi parent dirinya sendiri)
void makeSet(int verticesCount){
    for(int i = 0; i < verticesCount; i++){
        parent[i] = i;    // Setiap simpul adalah root dari dirinya sendiri
    }
}

// Fungsi untuk mencari root dari sebuah simpul (dengan rekursi)
int findParent(int vertex){
    if(parent[vertex] == vertex){
        return vertex;    // Jika parent-nya adalah dirinya sendiri, berarti dia root
    }
    return findParent(parent[vertex]);  // Rekursif cari root-nya

    // Untuk optimasi, bisa aktifkan ini (path compression):
    // parent[vertex] = findParent(parent[vertex]);
    // return parent[vertex];
}

// Fungsi untuk melakukan union antar simpul dan mendeteksi siklus
bool join(struct Graph *graph){
    for(int i = 0; i < graph->edgesCount; i++){ // Periksa setiap sisi dalam graf
        int sourceParent = findParent(graph->edge[i].source);         // Cari parent dari titik awal
        int destinationParent = findParent(graph->edge[i].destination); // Cari parent dari titik tujuan

        if(sourceParent == destinationParent){ // Jika keduanya sudah satu set, berarti ada siklus
            return true;
        }

        parent[sourceParent] = destinationParent; // Union: gabungkan dua set
    }

    return false; // Jika semua sisi diperiksa tanpa menemukan siklus
}

// Fungsi pembungkus: inisialisasi dan jalankan pengecekan siklus
bool isCyclic(struct Graph *graph){
    makeSet(graph->verticesCount);  // Inisialisasi semua simpul ke set masing-masing
    return join(graph);             // Cek dan gabungkan jika perlu
}

// Fungsi utama
int main(){
	/*
    // Buat graf dengan 4 simpul dan 4 sisi
    struct Graph *graph = createGraph(4, 4); 

    // Definisikan sisi-sisinya (edge list)
    graph->edge[0].source = 0;
    graph->edge[0].destination = 2;

    graph->edge[1].source = 2;
    graph->edge[1].destination = 1;

    graph->edge[2].source = 1;
    graph->edge[2].destination = 3;

    graph->edge[3].source = 2;
    graph->edge[3].destination = 4;
*/

// Contoh tidak cyclic
    struct Graph *graph = createGraph(4, 3); // sisi buat tempat simpul, jika simpul ada 4 & sisi ada 4, tetapi isi sisi 3 tidak cylic / no output, sisi yang diminta dan dijalankan harus sama
    
    graph->edge[0].source = 0;
    graph->edge[0].destination = 2;

    graph->edge[1].source = 2;
    graph->edge[1].destination = 1;

    graph->edge[2].source = 1;
    graph->edge[2].destination = 3;

    // graph->edge[3] tidak ada



    // Jalankan deteksi siklus
    if(isCyclic(graph)){
        printf("This graph is cyclic!\n");      // Jika ditemukan siklus
    } else {
        printf("This graph is not cyclic.\n");  // Jika graf aman
    }

    // Bebaskan memori setelah digunakan
    free(graph->edge);
    free(graph);

    return 0;
}