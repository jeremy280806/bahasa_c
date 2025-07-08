#include<stdio.h>
#include<stdlib.h>

struct Node{
	int value;
	Node* left;
	Node* right;
	int height;
} *root = NULL;

Node* createNode(int value){
	Node* newNode = (Node*) malloc (sizeof(Node));
	newNode->value = value;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->height = 1;
	
	return newNode;
}

// Cari Height sebuah node
int getHeight(Node* n){
	if(n == NULL)
		return 0;
	
	return n->height;
}

// Function bandingin dua buah angka dan kembalikan yang terbesar
int max(int a, int b){
	if(a > b)
		return a;
	
	return b;
}

// Set height node
int setHeight(Node* n){
	// Ambil height dari left child dan right child
	int heightLeft = getHeight(n->left);
	int heightRight = getHeight(n->right);
	
	// Ambil height yang paling besar + 1
	int nodeHeight = max(heightLeft, heightRight) + 1;
	return nodeHeight;
}

// Cari balance factor
int getBalanceFactor(Node* n){
	// Ambil height dari left child dan right child
	int heightLeft = getHeight(n->left);
	int heightRight = getHeight(n->right);
	
	// Kurangkan heightLeft dan heightRight
	int nodeBF = heightLeft - heightRight;
	return nodeBF;
}

// Left Rotation
// Parameter yang dikirim: node yang imbalance (x)
Node* rotateLeft(Node* x){
	// Step 1: Tentukan x, y, z
	// x: node yang imbalance
	// y: right child dari x
	// z: left child dari y
	Node* y = x->right;
	Node* z= y->left;
	
	// Step 2 & 3 boleh dibolak-balik
	// Step 2: Ubah right child milik x menjadi z
	x->right = z;
	
	// Step 3: Ubah left child milik y menjadi x
	y->left = x;
	
	// Step 4: Tentukan height baru untuk x dan y
	x->height = setHeight(x);
	y->height = setHeight(y);
	
	// Step 5: Kembalikan node yang paling atas (y)
	return y;
}

// Right Rotation
// Parameter yang dikirim: node yang imbalance (x)
Node* rotateRight(Node* x){ // -> balik semua arah yang ada di rotateLeft
	// Step 1: Tentukan x, y, z
	// x: node yang imbalance
	// y: left child dari x
	// z: right child dari y
	Node* y = x->left;
	Node* z= y->right;
	
	// Step 2 & 3 boleh dibolak-balik
	// Step 2: Ubah left child milik x menjadi z
	x->left = z;
	
	// Step 3: Ubah right child milik y menjadi x
	y->right = x;
	
	// Step 4: Tentukan height baru untuk x dan y
	x->height = setHeight(x);
	y->height = setHeight(y);
	
	// Step 5: Kembalikan node yang paling atas (y)
	return y;
}

Node* insert(Node* curr, int value){
//	1. Bikin insert kaya BST biasa
	if(curr == NULL)
		return createNode(value);
	
	if(value < curr->value){
//		Jika value baru lebih kecil maka gerak ke kiri
		curr->left = insert(curr->left, value);
	}else if (value > curr->value){
//		Jika value baru lebih besar maka gerak ke kanan
		curr->right = insert(curr->right, value);
	}else{
//		Jika ada yang sama maka ga di apa-apa in (kembalikan node yg sdh ada)
		return curr;
	}
	
//	2. Tentukan height dari node tersebut
	curr->height = setHeight(curr);
	
//	3. Cari balance factor dari node tersebut
	int balanceFactor = getBalanceFactor(curr);
	
//	4. Apabila ada imbalance maka diatasi
	// Case 1: Left Skewed (BF > 1)
	if(balanceFactor > 1){ // Left Skewed
		//Case 1.1: Right Rotation
		//Lurus (value baru yang dimasukkan lebih kecil dari left child yg
		//imbalance
		if(value < curr->left->value){
			return rotateRight(curr);
		}	
		
		//Case 1.2: Left-Right Rotation
		//Segitiga(value baru lebih besar dari left child)
		if(value > curr->left->value){
			//Step 1: Lakukan left rotation di left child dari node yang imbalance
			curr->left = rotateLeft(curr->left);
			//Step 2: Lakukan right rotation di node yang imbalance
			return rotateRight(curr);
		}
	}else if(balanceFactor < -1){// Case 2: Right Skewed
		// Case 2.1: Left Rotate
		if(value > curr->right->value){
			return rotateLeft(curr);
		}
		
		// Case 2.2: Right-Left Rotation
		if(value < curr->right->value){
			curr->right = rotateRight(curr->right);
			return rotateLeft(curr);
		}
	}
	
	// Step 5: Kalau ga ada imbalance maka langsung return curr aja
	return curr;
}

Node* deleteNode(Node* curr, int delValue){
	// Step 1: Lakukan deletion kaya BST biasa
	if(curr == NULL)
		return NULL;
	
	if(delValue < curr->value){
		curr->left = deleteNode(curr->left, delValue);
	}else if(delValue > curr->value){
		curr->right = deleteNode(curr->right, delValue);
	}else{
		// Case 1: yang dihapus leaf
		if(curr->right == NULL && curr->left == NULL){
			free(curr);
			return NULL;
		}
		
		// Case 2: yang dihapus node dgn 1 buah child
		if(curr->left == NULL){ // Ada yang kanan doang
			Node* temp = curr->right;
			free(curr);
			return temp;
		}else if(curr->right == NULL){ // Ada yang kiri doang
			Node* temp = curr->left;
			free(curr);
			return temp;
		}
		
		// Case 3: yang dihapus node dgn 1 buah child
			// Inorder successor: anak kanan paling kiri
		Node* is = curr->right;
		while(is->left != NULL){
			is = is->left;
		}
		
			// Masukkan value di inorder successor ke node yang mau dihapus
		curr->value = is->value;
		
			// hapus inorder successor
		curr->right = deleteNode(curr->right, is->value);
	}
	
	// Step 2: Cari height yang baru
	curr->height = setHeight(curr);
	
	// Step 3: Cari balance factor
	int balanceFactor = getBalanceFactor(curr);
	printf("Node %d BF = %d\n", curr->value, balanceFactor); //Untuk tahu kenapa balance-nya gagal
	
	// Step 4: Apabila ada imbalance maka diatasi
	// Case 1: Left Skewed (BF > 1)
	if(balanceFactor > 1){ // Left Skewed
		//Case 1.1: Right Rotation / Left-Left Case
		//Lurus (BF left child >= 0)
		if(getBalanceFactor(curr->left) >= 0){
			return rotateRight(curr);
		}	
		
		//Case 1.2: Left-Right Rotation
		//Segitiga(BF left child < 0)
		if(getBalanceFactor(curr->left) < 0){
			//Step 1: Lakukan left rotation di left child dari node yang imbalance
			curr->left = rotateLeft(curr->left);
			//Step 2: Lakukan right rotation di node yang imbalance
			return rotateRight(curr);
		}
	}else if(balanceFactor < -1){// Case 2: Right Skewed
		// Case 2.1: Left Rotate / RR Case
		if(getBalanceFactor(curr->right) <= 0){
			return rotateLeft(curr);
		}
		
		// Case 2.2: Right-Left Rotation
		if(getBalanceFactor(curr->right) > 0){
			curr->right = rotateRight(curr->right);
			return rotateLeft(curr);
		}
	}
	
	// Step 5
	return curr;
}

void preorder(Node* curr){
	if(curr != NULL){
		printf("%d ", curr->value);
		preorder(curr->left);
		preorder(curr->right);
	}
}

Node* search(struct Node *root, int val){
    if (root == NULL)  // ?? CEK DULU NULL ATAU NGGAK
        return NULL;

    if (root->value > val)
        return search(root->left, val);
    else if (root->value < val)
        return search(root->right, val);
    
    return root;
}


/*
Node* search(struct Node *root,int val){
		if(root->value > val){
        return search(root->left, val);
    }else if (root->value < val){
        return search(root->right, val);
    }
    
    return root;
}
*/

void updateNode(int oldVal, int newVal){
	// Search dulu node yang mau diupdate
	Node* toUpdate = search(root, oldVal);
	
	// Kalau ketemu
	if(toUpdate != NULL){
		// Remove dulu node yang lama
		root = deleteNode(root, oldVal);
		// Insert node yang baru
		root = insert(root, newVal);
	}
}

int main(){
	// Insert Awal: 10, 8, 20, 2, 15, 23, 25
	root = insert(root, 10);
	root = insert(root, 8);
	root = insert(root, 20);
	root = insert(root, 2);
	root = insert(root, 15);
	root = insert(root, 23);
	root = insert(root, 25);
	updateNode(10, 100);
	preorder(root);
	// Output sblm delete: 10, 8, 2, 20, 15, 23, 25
	printf("\n\nOutput sebelum delete: \n");
	preorder(root);
	printf(" \n ==================== \n");
	// Delete 8
	root = deleteNode(root, 8);
	// Output stlh delete: 20, 10, 2, 15, 23, 25
	printf("\n\nOutput sesudah delete: \n");
	preorder(root);

	// Delete 10
	//root = deleteNode(root, 10);
	// Output: 15, 8, 2, 23, 20, 25
	//printf("\n\nOutput sesudah delete: \n");
	//preorder(root);
//	
//	printf("\n ============ \n");
//	search(root, 8);
//	printf("\n");
	//search(root, 10);
	//printf("\n");
//	search(root, 100);
	
	return 0;
}
