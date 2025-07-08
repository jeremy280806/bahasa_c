#include <stdio.h>
#include <stdlib.h>


// 1. Bikin struct
struct Node{
	int val; // value angka
	struct Node *left, *right; 
	// pointer yang simpan address node kiri dan kanan
};

typedef struct Node Node;  // Tambahan ini

// 2. Bikin function newNode
Node* newNode(int val){
	// a. malloc
	Node* node = (Node*)malloc(sizeof(Node));
	// b. masukin data ke node baru
	node->val = val;
	node->left = node->right = NULL;
	// c. return nodenya
	return node;
}

// 3. Insert
Node* insertNode(Node* root, int val){
	// a. Kalau rootnya masih NULL --> BASE CASE REKURSIF
	if(root == NULL){
		return newNode(val);
//		return node;

	} 
	// b. Kalau value yang mau diinsert lebih kecil dari root
	else if (val < root->val){
		root->left = insertNode(root->left, val);
	} 
	// c. Kalau value yang mau diinsert lebih besar dari root
	else if (val > root->val){
		root->right = insertNode(root->right, val);
	}
	return root;
}

Node* searchNode(Node* root, int val){
	// a. Kalau rootnya masih NULL --> BASE CASE REKURSIF
	if(root == NULL || root->val == val){
		return root;
	} 
	// b. Kalau value yang mau diinsert lebih kecil dari root
	else if (val < root->val){
		return searchNode(root->left, val);
	} 
	// c. Kalau value yang mau diinsert lebih besar dari root
	else if (val > root->val){
		return searchNode(root->right, val);
	}
}

// Helper function untuk mencari nilai terkecil (min value) di subtree
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}


Node* removeNode(Node* curr, int val){
	// 1. Kalau misal BST kita kosong, kita langsung return
	if(curr == NULL){
		return curr;
	}
	
	// 2. Buat function rekursif untuk geser curr ke kiri/kanan tergantung value
	if (val < curr->val){
		curr->left = removeNode(curr->left, val);
	} else if (val > curr->val){
		curr->right = removeNode(curr->right, val);
	} else{
		// 3. Kalau misal value yang mau dihapus sudah sesuai dengan value node skrg
		// Case 1: Node yg mau dihapus tanpa anak
		if(curr->left == NULL && curr->right == NULL){
			free(curr);
			return NULL;
		}
		// Case 2: Punya satu anak (kanan atau kiri)
		else if(curr->left == NULL){
			Node* temp = curr->right;
			free(curr);
			return temp;
		} else if (curr->right == NULL){
			Node* temp = curr->left;
			free(curr);
			return temp;
		}
		// CASE 3: Punya dua anak
        else {
            Node* temp = minValueNode(curr->right); // cari inorder successor
            curr->val = temp->val; // ganti nilai sekarang dengan successor
            curr->right = removeNode(curr->right, temp->val); // hapus successor
		}
	}
	return curr;
}

// 7. Print inorder BST
void printInOrder(Node* root) {
    if (root != NULL) {
        printInOrder(root->left);
        printf("%d ", root->val);
        printInOrder(root->right);
    }
}

// 8. Main
int main() {
    Node* root = NULL;

    // Insert nilai
    root = insertNode(root, 8);
    root = insertNode(root, 10);
    root = insertNode(root, 7);
    root = insertNode(root, 9);
    root = insertNode(root, 5);

    printf("Isi BST (Inorder): ");
    printInOrder(root);
    printf("\n");

    // Search node
    Node* search = searchNode(root, 10);
    if (search != NULL)
        printf("Node %d ditemukan!\n", search->val);
    else
        printf("Node tidak ditemukan.\n");

    // Remove node
    root = removeNode(root, 10);
    root = removeNode(root, 8);

    printf("BST setelah penghapusan (Inorder): ");
    printInOrder(root);
    printf("\n");

    return 0;
}

