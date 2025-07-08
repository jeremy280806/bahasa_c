#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>

typedef struct Booking{
	char id[6];
	char name[31];
	char phone[16];
	int age;
	char roomType[8];
	int duration;
}Booking;

typedef struct AVLNode{
	Booking data;
	int height;
	struct AVLNode *left, *right;
} AVLNode;

int max(int a, int b){
	return (a > b) ? a : b;
}

int height(AVLNode* node){
	return node ? node->height : 0;
} 

int getBalance(AVLNode* node){
	return node ? height(node->left) - height(node->right) : 0;
}

AVLNode* createNode(Booking data){
	AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
	newNode->data = data;
	newNode->height = 1;
	newNode->left = newNode->right = NULL;
	return newNode;
}

AVLNode* rightRotate(AVLNode* y){
	AVLNode* x = y->left;
	AVLNode* z = x->right;
	
	x->right = y;
	y->left = z;
	
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;
	return x;
}

AVLNode* leftRotate(AVLNode* x){
	AVLNode* y = x->right;
	AVLNode* z = y->left;
	
	y->left = x;
	x->right = z;
	
	y->height = max(height(y->left), height(y->right));
	x->height = max(height(x->left), height(x->right));
	return y;
}

int compareID(char* id1, char* id2){
	return strcmp(id1, id2);
}

void generateBookingID(char* id, char* roomType, int number){
	char prefix[3];
	prefix[0] = toupper(roomType[0]);
	prefix[1] = toupper(roomType[1]);
	prefix[2] = '\0';
	sprintf(id, "%s%03d", prefix, number % 1000);
}

AVLNode* insertNode(AVLNode* node, Booking data){
	if(!node) return createNode(data);	
	if(compareID(data.id, node->data.id) < 0)
		node->left = insertNode(node->left, data);
	else if(compareID(data.id, node->data.id) > 0)
		node->right = insertNode(node->right, data);
	else
		return node;
	
	node->height = 1 + max(height(node->left), height(node->right));
	
	int balance = getBalance(node);
	
	if(balance > 1 && compareID(data.id, node->left->data.id) < 0)
		return rightRotate(node); 
	if(balance < -1 && compareID(data.id, node->right->data.id) > 0)
		return leftRotate(node);
	if(balance > 1 && compareID(data.id, node->right->data.id) < 0){
		node->left = rightRotate(node->left);
		return rightRotate(node);
	} 
	if(balance < -1 && compareID(data.id, node->right->data.id) > 0){
		node->right = rightRotate(node->right);
		return leftRotate(node); 
	}
	return node;
}

AVLNode* minValueNode(AVLNode* node){
	AVLNode* current = node;
	while(current->left != NULL)
		current = current->left;
	return current;
}

AVLNode* deleteNode(AVLNode* root, char id[]){
	if(!root) return root;
	
	if(compareID(id, root->data.id) < 0)
		root->left = deleteNode(root->left, id);
	else if(compareID(id, root->data.id) > 0)
		root->right = deleteNode(root->right, id);
	else{
		if(!root->left || !root->right){
			AVLNode* temp = root->left ? root->left : root->right;
			free(root);
			return temp;
		}
		AVLNode* temp = minValueNode(root->right);
		root->data = temp->data;
		root->right = deleteNode(root->right, temp->data.id);
	}
	root->height = 1 + max(height(root->left), height(root->right));
	int balance = getBalance(root);
	
	if(balance > 1 && getBalance(root->left))
		return rightRotate(root);
	if(balance > 1 && getBalance(root->right)){
		root->left = leftRotate(root->left);
		return rightRotate(root);  
	}    
	
	if(balance < -1 && getBalance(root->right))
		return leftRotate(root);
	if(balance < -1 && getBalance(root->right)){
		root->right = leftRotate(root->right);
		return  rightRotate(root); 
	} 
	return root;
}

void inOrder(AVLNode* root){
	if(root){
		inOrder(root->left);
		printf("ID: %s | Name: %s | Phone: %s | Type: %s | Duration: %d\n", 
		root->data.id, root->data.name, root->data.phone, root->data.roomType, root->data.duration);
		inOrder(root->right);
	}
}

void menu(){
	AVLNode* root = NULL;
	int choice;
	do{
		printf("\n=== HOTEL AVL MENU ===\n");
		printf("1. Insert Booking (Manual ID)\n");
		printf("2. Insert Booking (Auto ID from number)\n");
		printf("3. View All Booking\n");
		printf("4. Delete Booking by ID\n");
		printf("5. Exits\n");
		printf("Choose: ");
		scanf("%d", &choice); getchar();
		
		if(choice == 1 || choice == 2){
			Booking b;
			printf("Input Name: "); fgets(b.name, sizeof(b.name), stdin); b.name[strcspn(b.name, "\n")] = 0;
			printf("Input Phone: "); fgets(b.phone, sizeof(b.phone), stdin); b.phone[strcspn(b.phone, "\n")] = 0;
			printf("Input Age: "); scanf("%d", &b.age); getchar();
			printf("Input Room Type (Regular/Deluxe/Suite): "); fgets(b.roomType, sizeof(b.roomType), stdin); b.roomType[strcspn(b.roomType, "\n")] = 0;
			printf("Input Duration(nights): "); scanf("%d", &b.duration); getchar();
			if(choice == 1){
				printf("Input booking ID (format: XX999): ");
				fgets(b.id, sizeof(b.id),stdin); b.id[strcspn(b.id, "\n")] = 0; 
			}
			else{
				int num;
				printf("Input unique number (will be combined into ID): ");
				scanf("%d", &num); getchar();
				generateBookingID(b.id, b.roomType, num); 
			}
			root = insertNode(root, b);
			printf("Booking saved with ID: %s", b.id);
		}
		else if(choice == 3){
			printf("=== All booking ===\n");
			inOrder(root); 
		}
		else if(choice == 4){
			char id[6];
			printf("Input ID to Delete: ");
			fgets(id, sizeof(id), stdin); id[strcspn(id,"\n")] = 0;
			root = deleteNode(root, id); 
			printf("Delete Exits\n");
		}
	}while(choice != 5);
}

int main(){
	srand(time(NULL));
	menu();
	return 0;
}