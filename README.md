# AVL Tree Implementation

A complete, self-balancing binary search tree implementation in C that maintains O(log n) time complexity for insertions, deletions, and searches.

## 📋 Table of Contents
- [What is an AVL Tree?](#-what-is-an-avl-tree)
- [Features](#-features)
- [Data Structures](#-data-structures)
- [API Reference](#-api-reference)
- [Implementation Details](#-implementation-details)
- [Usage Examples](#-usage-examples)
- [Complexity Analysis](#-complexity-analysis)
- [Files](#-files)

## 🌳 What is an AVL Tree?

An AVL tree (named after inventors Adelson-Velsky and Landis) is a self-balancing binary search tree where the heights of the left and right subtrees of any node differ by at most one. This balance property is maintained through rotations.

**Balance Factor** = Height(Left Subtree) - Height(Right Subtree)

For a valid AVL tree, the balance factor of every node must be -1, 0, or 1.

## ✨ Features

- ✅ **Self-balancing**: Automatically maintains balance through rotations
- ✅ **Efficient operations**: O(log n) insert and delete
- ✅ **No duplicates**: Prevents duplicate values
- ✅ **Multiple traversals**: In-order, pre-order, and post-order with callback functions
- ✅ **Memory-safe**: Proper cleanup with `free_tree()`
- ✅ **Single file**: All implementation in one file for easy integration
- ✅ **Lightweight**: Direct node manipulation without wrapper structures

## 📊 Data Structures

### Node Structure
```c
typedef struct Node {
    int val;              // The value stored in the node
    struct Node* left;    // Pointer to left child
    struct Node* right;   // Pointer to right child
    int height;           // Height of this node in the tree
} Node;
```

## 🔧 API Reference

### Core Operations

#### `Node* insert_node(Node* node, int val)`
Inserts a new value into the tree and returns the new root. If the value already exists, returns the tree unchanged.
- **Parameters**: `node` - Root of the tree/subtree, `val` - Value to insert
- **Returns**: New root of the tree after insertion and rebalancing
- **Time Complexity**: O(log n)

**Example**:
```c
Node* root = NULL;
root = insert_node(root, 10);
root = insert_node(root, 20);
```

#### `Node* delete_node(Node* node, int val)`
Removes a value from the tree and returns the new root. Returns `NULL` if tree is empty.
- **Parameters**: `node` - Root of the tree/subtree, `val` - Value to delete
- **Returns**: New root of the tree after deletion and rebalancing
- **Time Complexity**: O(log n)

**Example**:
```c
root = delete_node(root, 20);
```

#### `void free_tree(Node* node)`
Frees all memory associated with the tree (post-order traversal).
- **Parameters**: `node` - Root of the tree to destroy
- **Time Complexity**: O(n)

**Example**:
```c
free_tree(root);
root = NULL;
```

### Traversal Operations

All traversal functions accept a visitor callback function:
```c
typedef void (*visit_fn)(int);
```

#### `void inorder(Node* node, void (*visit)(int))`
Performs in-order traversal (left → root → right). Visits nodes in sorted order.

#### `void preorder(Node* node, void (*visit)(int))`
Performs pre-order traversal (root → left → right).

#### `void postorder(Node* node, void (*visit)(int))`
Performs post-order traversal (left → right → root).

**Time Complexity**: O(n) for all traversals

### Utility Functions (Static/Internal)

These functions are used internally but are part of the implementation:

- `int height_node(Node* node)` - Returns height of a node (0 for NULL)
- `int get_balance(Node* node)` - Returns balance factor of a node
- `void update_height(Node* node)` - Updates height based on children
- `Node* right_rotate(Node* node)` - Performs right rotation
- `Node* left_rotate(Node* node)` - Performs left rotation
- `Node* find_successor(Node* node)` - Finds in-order successor (leftmost in right subtree)

## 🔬 Implementation Details

### Balancing Mechanism

The tree maintains balance through **four types of rotations**:

#### 1. Right Rotation (LL Case)
Used when left subtree is too heavy and insertion was in left-left path.
```
      y                    x
     / \                  / \
    x   C    ------>     A   y
   / \                      / \
  A   B                    B   C
```

**Triggered when**: `balance > 1 && val < node->left->val`

#### 2. Left Rotation (RR Case)
Used when right subtree is too heavy and insertion was in right-right path.
```
    x                        y
   / \                      / \
  A   y      ------>       x   C
     / \                  / \
    B   C                A   B
```

**Triggered when**: `balance < -1 && val > node->right->val`

#### 3. Left-Right Rotation (LR Case)
First left rotation on left child, then right rotation on node.
```
      z                    z                    y
     / \                  / \                  / \
    x   D    ------>     y   D    ------>    x   z
   / \                  / \                 / \ / \
  A   y                x   C               A  B C  D
     / \              / \
    B   C            A   B
```

**Triggered when**: `balance > 1 && val > node->left->val`

#### 4. Right-Left Rotation (RL Case)
First right rotation on right child, then left rotation on node.

**Triggered when**: `balance < -1 && val < node->right->val`

### Height Update Strategy

After every insertion or deletion, the implementation:
1. Updates node heights bottom-up using `update_height()`
2. Calculates balance factor at each node using `get_balance()`
3. Performs rotations if balance factor exceeds [-1, 1]

### Deletion Strategy

For node deletion with two children:
1. Find in-order successor using `find_successor()` (leftmost node in right subtree)
2. Replace node's value with successor's value
3. Recursively delete the successor (which has at most one child)
4. Rebalance from deletion point upward

For nodes with 0 or 1 child, simply replace with child or NULL.

## 💡 Usage Examples

### Basic Usage

```c
#include <stdio.h>
#include <stdlib.h>
#include "avl.c"  // Or compile avl.c with your program

int main(void) {
    Node* root = NULL;

    // Insert values
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

    // Delete a value
    root = delete_node(root, 30);

    printf("Inorder after deletion: ");
    inorder(root, print_value);
    printf("\n");

    // Clean up
    free_tree(root);
    return 0;
}
```

### Custom Visitor Functions

```c
// Count nodes
static int count = 0;
void count_nodes(int val) {
    count++;
}

// Sum all values
static int sum = 0;
void sum_values(int val) {
    sum += val;
}

// Find maximum value (using in-order traversal)
static int max_val = INT_MIN;
void find_max(int val) {
    if (val > max_val) {
        max_val = val;
    }
}

// Usage
count = 0;
inorder(root, count_nodes);
printf("Total nodes: %d\n", count);

sum = 0;
inorder(root, sum_values);
printf("Sum of all values: %d\n", sum);
```

### Building a Balanced Tree from Array

```c
Node* build_tree_from_array(int* arr, int size) {
    Node* root = NULL;
    for (int i = 0; i < size; i++) {
        root = insert_node(root, arr[i]);
    }
    return root;
}

int main(void) {
    int values[] = {50, 25, 75, 10, 30, 60, 80};
    int size = sizeof(values) / sizeof(values[0]);
    
    Node* root = build_tree_from_array(values, size);
    
    printf("Tree in sorted order: ");
    inorder(root, print_value);
    printf("\n");
    
    free_tree(root);
    return 0;
}
```

## ⚡ Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Insert    | O(log n)       | O(log n)*        |
| Delete    | O(log n)       | O(log n)*        |
| Traversal | O(n)           | O(log n)*        |
| Free Tree | O(n)           | O(log n)*        |

\* Space complexity due to recursion stack depth

### Why O(log n)?

The height of an AVL tree with `n` nodes is always O(log n) due to the balance property. This guarantees:
- Maximum height ≈ 1.44 × log₂(n)
- All operations that traverse from root to leaf are O(log n)

## 📁 Files

- **`avl.c`** - Complete implementation file with all AVL tree operations and example usage in `main()`

## 🎯 Key Implementation Highlights

1. **Recursive approach**: All major operations use recursion for clean, maintainable code
2. **Height caching**: Each node stores its height to avoid recalculation
3. **Memory safety**: Proper null checks and cleanup in all operations
4. **Direct node manipulation**: No wrapper structure - work directly with root pointer
5. **Generic traversal**: Visitor pattern (callback functions) allows flexible tree operations
6. **Automatic rebalancing**: Returns new root after every insertion/deletion

## 🔍 Example: Tree Evolution

Starting with an empty tree and inserting 10, 20, 30, 40 (from the included example):

```
Insert 10:      10

Insert 20:      10
                  \
                   20

Insert 30:      20          (Left rotation performed at node 10)
               /  \
              10   30

Insert 40:      20          (Left rotation performed at node 30)
               /  \
              10   30
                     \
                      40

After rebalance: 20
                /  \
               10   30
                     \
                      40
```

**Output from the example program**:
```
Inorder: 10 20 30 40
Preorder: 20 10 30 40
Postorder: 10 40 30 20
Inorder after deletion: 10 20 40
```

## 📝 Important Notes

1. **Always update root**: Since rotations can change the root, always assign the return value:
   ```c
   root = insert_node(root, value);  // ✓ Correct
   insert_node(root, value);          // ✗ Wrong!
   ```

2. **NULL safety**: All functions handle NULL nodes gracefully

3. **No duplicates**: Inserting an existing value has no effect

4. **Memory management**: Always call `free_tree()` when done to prevent memory leaks

5. **Integer values only**: This implementation stores `int` values. To store other types, modify the `Node` structure and comparison logic.

## 🔧 Customization

To use custom data types instead of integers:

```c
typedef struct Node {
    YourDataType data;  // Replace int val
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// Update comparison logic in insert_node and delete_node
// Example: if (compare(val, node->data) < 0)
```

---

**License**: Public Domain  
**Author**: vanyan12  
**Repository**: https://github.com/vanyan12/DataStructures-
