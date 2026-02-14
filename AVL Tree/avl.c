#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

static int max_int(int a, int b) {
    return (a > b) ? a : b;
}

static int height_node(Node* node) {
    return node ? node->height : 0;
}

static int get_balance(Node* node) {
    return node ? height_node(node->left) - height_node(node->right) : 0;
}

static void update_height(Node* node) {
    if (node != NULL) {
        node->height = 1 + max_int(height_node(node->left), height_node(node->right));
    }
}

static Node* right_rotate(Node* node) {
    Node* root = node->left;
    Node* node_left = root->right;

    node->left = node_left;
    root->right = node;

    update_height(node);
    update_height(root);

    return root;
}

static Node* left_rotate(Node* node) {
    Node* root = node->right;
    Node* node_right = root->left;

    node->right = node_right;
    root->left = node;

    update_height(node);
    update_height(root);

    return root;
}

static Node* find_successor(Node* node) {
    Node* current = node;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node* insert_node(Node* node, int val) {
    if (node == NULL) {
        Node* n = (Node*)malloc(sizeof(Node));
        n->height = 1;
        n->left = NULL;
        n->right = NULL;
        n->val = val;
        return n;
    }

    if (val < node->val) {
        node->left = insert_node(node->left, val);
    } else if (val > node->val) {
        node->right = insert_node(node->right, val);
    } else {
        return node;
    }

    update_height(node);

    int balance = get_balance(node);

    if (balance > 1 && val < node->left->val) {
        return right_rotate(node);
    }
    if (balance < -1 && val > node->right->val) {
        return left_rotate(node);
    }
    if (balance > 1 && val > node->left->val) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    if (balance < -1 && val < node->right->val) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

Node* delete_node(Node* node, int val) {
    if (node == NULL) return NULL;

    if (val < node->val) {
        node->left = delete_node(node->left, val);
    } else if (val > node->val) {
        node->right = delete_node(node->right, val);
    } else {
        if (node->left == NULL) {
            Node* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            Node* temp = node->left;
            free(node);
            return temp;
        }

        Node* temp = find_successor(node->right);
        node->val = temp->val;
        node->right = delete_node(node->right, temp->val);
    }

    update_height(node);

    int balance = get_balance(node);

    if (balance > 1 && get_balance(node->left) >= 0) {
        return right_rotate(node);
    }
    if (balance < -1 && get_balance(node->right) <= 0) {
        return left_rotate(node);
    }
    if (balance > 1 && get_balance(node->left) < 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    if (balance < -1 && get_balance(node->right) > 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

void free_tree(Node* node) {
    if (node != NULL) {
        free_tree(node->left);
        free_tree(node->right);
        free(node);
    }
}

// Traversal functions
void inorder(Node* node, void (*visit)(int)) {
    if (node != NULL) {
        inorder(node->left, visit);
        visit(node->val);
        inorder(node->right, visit);
    }
}

void preorder(Node* node, void (*visit)(int)) {
    if (node != NULL) {
        visit(node->val);
        preorder(node->left, visit);
        preorder(node->right, visit);
    }
}

void postorder(Node* node, void (*visit)(int)) {
    if (node != NULL) {
        postorder(node->left, visit);
        postorder(node->right, visit);
        visit(node->val);
    }
}

// Example print function
static void print_value(int val) {
    printf("%d ", val);
}

int main(void) {
    Node* root = NULL;

    root = insert_node(root, 10);
    root = insert_node(root, 20);
    root = insert_node(root, 30);
    root = insert_node(root, 40);

    printf("Inorder: ");
    inorder(root, print_value);
    printf("\n");

    printf("Preorder: ");
    preorder(root, print_value);
    printf("\n");

    printf("Postorder: ");
    postorder(root, print_value);
    printf("\n");

    root = delete_node(root, 30);

    printf("Inorder after deletion: ");
    inorder(root, print_value);
    printf("\n");

    free_tree(root);
    return 0;
}
