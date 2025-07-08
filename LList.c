#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur Node (khusus DLL)
typedef struct Node {
    int number;
    struct Node* next;
    struct Node* prev;
} Node;

Node* head = NULL, *tail = NULL;

// Fungsi membuat node baru
Node* newNode(int number) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->number = number;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Push Head
void pushHead(int number) {
    Node* node = newNode(number);
    if (head == NULL) {
        head = tail = node;
        return;
    }
    node->next = head;
    head->prev = node;
    head = node;
}

// Push Tail
void pushTail(int number) {
    Node* node = newNode(number);
    if (head == NULL) {
        head = tail = node;
        return;
    }
    tail->next = node;
    node->prev = tail;
    tail = node;
}

// Push Mid
void pushMid(int number) {
    Node* node = newNode(number);
    if (head == NULL) {
        head = tail = node;
        return;
    } else if (number < head->number) {
        pushHead(number);
    } else if (number > tail->number) {
        pushTail(number);
    } else {
        Node* curr = head;
        while (curr->next->number < number) {
            curr = curr->next;
        }
        node->next = curr->next;
        node->prev = curr;
        if (curr->next != NULL) curr->next->prev = node;
        curr->next = node;
    }
}

// Pop Head
void popHead() {
    if (head == NULL) return;
    if (head == tail) {
        free(head);
        head = tail = NULL;
        return;
    }
    Node* toDelete = head;
    head = head->next;
    head->prev = NULL;
    free(toDelete);
}

// Pop Tail
void popTail() {
    if (head == NULL) return;
    if (head == tail) {
        free(head);
        head = tail = NULL;
        return;
    }
    Node* toDelete = tail;
    tail = tail->prev;
    tail->next = NULL;
    free(toDelete);
}

// Pop Mid
void popMid(int number) {
    if (head == NULL) return;
    if (head == tail && head->number == number) {
        free(head);
        head = tail = NULL;
        return;
    }
    if (head->number == number) {
        popHead();
        return;
    }
    if (tail->number == number) {
        popTail();
        return;
    }

    Node* curr = head->next;
    while (curr != NULL && curr->number != number) {
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("Data not found\n");
        return;
    }
    curr->prev->next = curr->next;
    if (curr->next != NULL) {
        curr->next->prev = curr->prev;
    }
    free(curr);
}

// Cetak dari depan
void print() {
    Node* curr = head;
    printf("Isi Linked List: ");
    while (curr) {
        printf("%d", curr->number);
        if (curr->next != NULL) printf(" -> ");
        curr = curr->next;
    }
    printf(" -> NULL\n");
}

// Cetak dari belakang
void printReverse() {
    Node* curr = tail;
    printf("NULL");
    while (curr) {
        printf(" <- %d", curr->number);
        curr = curr->prev;
    }
    printf("\n");
}

int main() {
    pushHead(30);
    pushTail(50);
    pushHead(10);
    pushMid(40);
    pushMid(20);

    printf("Isi Linked List setelah push:\n");
    print();

    printf("\nIsi Linked List (dari belakang):\n");
    printReverse();

    printf("\nSetelah popMid(30):\n");
    popMid(30);
    print();

    printf("\nSetelah popHead:\n");
    popHead();
    print();

    printf("\nSetelah popTail:\n");
    popTail();
    print();

    return 0;
}

