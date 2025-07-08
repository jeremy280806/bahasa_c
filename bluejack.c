
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct GameNode {
    char title[30];
    char genre[20];
    int stock;
    int height;
    struct GameNode *left, *right;
} GameNode;

// AVL Utilities
int height(GameNode *node) {
    return node ? node->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void updateHeight(GameNode *node) {
    node->height = 1 + max(height(node->left), height(node->right));
}

int getBalance(GameNode *node) {
    return node ? height(node->left) - height(node->right) : 0;
}

GameNode* rotateRight(GameNode *y) {
    GameNode *x = y->left; // y : node yang tidak seimbang 
    GameNode *T2 = x->right; // T2 : anak kanan dari x 

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

GameNode* rotateLeft(GameNode *x) {
    GameNode *y = x->right;
    GameNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// Node creation
GameNode* createNode(char *title, char *genre, int stock) {
    GameNode *node = (GameNode*)malloc(sizeof(GameNode));
    strcpy(node->title, title);
    strcpy(node->genre, genre);
    node->stock = stock;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

// Insert node
GameNode* insert(GameNode* node, char *title, char *genre, int stock) {
    if (!node) return createNode(title, genre, stock);

   // int cmp = strcmp(title, node->title);
    if (strcmp(title, node->title) < 0)
        node->left = insert(node->left, title, genre, stock);
    else if (strcmp(title, node->title) > 0)
        node->right = insert(node->right, title, genre, stock);
    else
        return node;

    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1 && strcmp(title, node->left->title) < 0)
        return rotateRight(node);
    if (balance < -1 && strcmp(title, node->right->title) > 0)
        return rotateLeft(node);
    if (balance > 1 && strcmp(title, node->left->title) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && strcmp(title, node->right->title) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// In-order display
/*
void printHeader() {
    printf("+---------------------------+----------------------+-------------+\n");
    printf("| %-25s | %-20s | %-11s |\n", "Game Title", "Game Genre", "Game Stock");
    printf("+---------------------------+----------------------+-------------+\n");
}

void inOrder(GameNode *root) {
    static int called = 0;
    if (!called) {
        printHeader();
        called = 1;
    }

    if (root) {
        inOrder(root->left);
        printf("| %-25s | %-20s | %-11d |\n", root->title, root->genre, root->stock);
        inOrder(root->right);
    }

    // Reset static flag agar header ditampilkan ulang saat dipanggil lagi
    if (root == NULL) called = 0;
}*/

void inOrder(GameNode *root) {
    if (root) {
        inOrder(root->left);
        printf("Title: %s | Genre: %s | Stock: %d\n", root->title, root->genre, root->stock);
        inOrder(root->right);
    }
}

// Search node
GameNode* search(GameNode *root, char *title) {
    if (!root || strcmp(root->title, title) == 0)
        return root;
    else if (strcmp(title, root->title) < 0)
        return search(root->left, title);
    else
        return search(root->right, title);
}

// Validation
int isGenreValid(char *genre) {
    return strcmp(genre, "Action") == 0 ||
           strcmp(genre, "RPG") == 0 ||
           strcmp(genre, "Adventure") == 0 ||
           strcmp(genre, "Card Game") == 0;
}

void toTitleCase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (i == 0 || (i > 0 && str[i - 1] == ' '))
            str[i] = toupper(str[i]);
        else
            str[i] = tolower(str[i]);
    }
}

// Insert game
GameNode* insertGame(GameNode *root) {
    char title[30], genre[20];
    int stock;

    do {
        printf("Input Game Title [5-25 chars]: ");
        scanf(" %[^\n]", title);
    } while (strlen(title) < 5 || strlen(title) > 25);

    if (search(root, title)) {
        printf("Title already exists!\n");
        return root;
    }

    do {
        printf("Input Game Genre [Action | RPG | Adventure | Card Game]: ");
            printf("Input Game Stock [min 1]: ");
        scanf("%d", &stock);
    } while (stock < 1);

    printf("Game successfully inserted!\n");
    return insert(root, title, genre, stock);
}

// Minimum node
GameNode* minValueNode(GameNode* node) {
    GameNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Delete node
GameNode* deleteNode(GameNode* root, char *title) {
    if (!root) return root;

    int cmp = strcmp(title, root->title);
    if (cmp < 0)
        root->left = deleteNode(root->left, title);
    else if (cmp > 0)
        root->right = deleteNode(root->right, title);
    else {
        if (!root->left || !root->right) {
            GameNode *temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }
        GameNode *temp = minValueNode(root->right);
        strcpy(root->title, temp->title);
        strcpy(root->genre, temp->genre);
        root->stock = temp->stock;
        root->right = deleteNode(root->right, temp->title);
    }

    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Update stock
GameNode* updateStock(GameNode* root) {
    char title[30], action[10];
    int amount;

    printf("Enter game title to update: ");
    scanf(" %[^\n]", title);
    GameNode *target = search(root, title);
    if (!target) {
        printf("Data not found\n");
        return root;
    }

    do {
        printf("Input update type [add | remove]: ");
        scanf(" %[^\n]", action);
        for (int i = 0; action[i]; i++) action[i] = tolower(action[i]);
    } while (strcmp(action, "add") != 0 && strcmp(action, "remove") != 0);

    if (strcmp(action, "add") == 0) {
        do {
            printf("Enter quantity to add [min 1]: ");
            scanf("%d", &amount);
        } while (amount < 1);
        target->stock += amount;
        printf("Stock successfully added.\n");
    } else {
        do {
            printf("Enter quantity to remove [1 - %d]: ", target->stock);
            scanf("%d", &amount);
        } while (amount < 1 || amount > target->stock);
        target->stock -= amount;
        if (target->stock == 0) {
            printf("Stock is now 0. Game removed from warehouse.\n");
            root = deleteNode(root, title);
        } else {
            printf("Stock successfully reduced.\n");
        }
    }

    return root;
}

// Menu
void menu() {
    printf("\n=== Bluejack GShop ===\n");
    printf("1. Insert Game\n");
    printf("2. View Game\n");
    printf("3. Update Stock\n");
    printf("4. Exit\n");
    printf("Choose: ");
}

// Main
int main() {
    int choice;
    GameNode *root = NULL;

    do {
        menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                root = insertGame(root);
                break;
            case 2:
                if (!root) {
                    printf("Warehouse is empty!\n");
                } else {
                    inOrder(root);
                }
                break;
            case 3:
                root = updateStock(root);
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid input!\n");
        }
    } while (choice != 4);

    return 0;
}
