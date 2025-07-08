#include<stdio.h>
#define SIZE 100

int heap[SIZE]; // ukuran heap min
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
	int minValue = heap[1];
	heap[1] = heap[counter - 1];
	counter--;
	
	int current = 1;
	
	while(current * 2 < counter){
		int left = current * 2;
		int right = left + 1; 
		int smallest = current;
		
		if(left < counter && heap[left] < heap[smallest])
			smallest = left;
		if(right < counter && heap[right] < heap[smallest])
			smallest = right;
		if(smallest == current) break;
		
		swap(&heap[current], &heap[smallest]);
		current = smallest;
	}
	return minValue;
}

void printHeap(){
	printf("Heap content: ");
	for(int i = 1; i < counter; i++){
		printf("%d ", heap[i]);
	}
	printf("\n============\n");
}

int main(){
	push(30); 
	push(20); 
	push(40); 
	push(10); 
	push(35); 
	push(5); 
 	printHeap();
 	
 	printf("Pop: %d\n", pop());
 	printHeap();
 	printf("Pop: %d\n", pop());
 	printHeap(); 
}