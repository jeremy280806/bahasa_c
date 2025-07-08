#include<stdio.h>

#define SIZE 100 // ukuran max heap

int heap[SIZE];

int counter = 1;

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void push(int val){
	int current = counter;
	heap[current] = val;
	
	while(current > 1 && heap[current / 2] > heap[current]){
		swap(&heap[current], &heap[current / 2]);
		current = current / 2;
	}
	counter++;
	printf("Insert success for %d\n", val);
}

int pop(){
	if(counter == 1){
		printf("Heap is empty!\n");
		return -1;
	}
	int maxValue = heap[1];
	heap[1] = heap[counter - 1];
	counter--;
	
	int current = 1;
	
	while(current * 2 < counter){
		int left = current * 2;
		int right = left + 1;
		int largest = current;
		
		if(left < counter && heap[left] > heap[largest])
			largest = left;
		if(right < counter && heap[right] > heap[largest])
			largest = right;
		
		if(largest = current) break;
			
		swap(&heap[current] , &heap[largest]);
		current = largest;
	}
	return maxValue; 
}

void printHeap(){
	printf("Heap content: ");
	for(int i = 1; i < counter; i++){
		printf("%d ", heap[i]);
	} 
	printf("\n============\n");
}

void heapify(int arr[], int n, int i){
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	
	if(left < n && arr[left] > arr[largest])
		largest = left;
	if(right < n && arr[right] > arr[largest])
		largest = right;
	
	if(largest != i){
		swap(&arr[i], &arr[largest]);
		heapify(arr, n, largest);
	}
}

void heapSort(int arr[], int n){
	for(int i = n / 2 - 1; i >= 0; i++){
		heapify(arr, n, i);
	}
	for(int i = n - 1; i > 0; i--){
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}
}

void printArr(int arr[], int n){
	for(int i = 0; i < n; i++){
		printf("%d ", arr[i]); 
	}
	printf("\n================\n"); 
}

int main(){
	push(20);
    push(15);
    push(13);
    push(9);
    push(5);
    push(7);
    push(30);
    printHeap();

    printf("Pop: %d\n", pop());
    printHeap();

    int arr[10] = {3, -1, 11, 23, 5, 7, 1, 2, -5, 8};
    printArr(arr, 10);
    heapSort(arr, 10);
    printArr(arr, 10);

    return 0;
}