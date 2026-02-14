# Tree Traversal Algorithms - Reference Guide

Quick reference for tree traversal implementations with important details highlighted in code.

---

## 📊 Space Complexity: Why it Varies

```c
// Recursion uses function call stack
// At any time: at most h function calls on the stack

// Balanced tree (h = log n):     Space = O(log n)
//       4
//      / \
//     2   6
//    / \ / \
//   1  3 5  7

// Skewed tree (h = n):          Space = O(n)
//   1
//    \
//     2
//      \
//       3
//        \
//         4
```

---

## 🔄 DFS In-Order (Recursive)

```c
dfs_inorder(Node* root){
    if(root == NULL){
        return;
    }
    
    dfs_inorder(root->left);    // 1. Go left
    printf("%d ", root->val);    // 2. Process root
    dfs_inorder(root->right);    // 3. Go right
}
// Order: Left → Root → Right
// Space: O(h) - call stack depth
```

---

## 🔄 DFS In-Order (Iterative)

**Key Pattern**: Two-phase approach

```c
def_inorder_iter(Node* root){
    std::stack<Node*> s;
    
    // PHASE 1: Push all left children
    while(root){
        s.push(root);
        root = root->left;  // Keep going left!
    }
    
    while(!s.empty()){ 
        Node* top = s.top();
        s.pop();
        std::cout<<top->val<<std::endl;  // Process
        
        // PHASE 2: For right child, repeat PHASE 1
        Node* curr = top->right;
        while(curr){
            s.push(curr);
            curr = curr->left;  // Again, go left!
        }
    }
}
```

**Why two phases?** Mimics recursion: "go left until NULL" → "process" → "go right and repeat"

---

## 🌊 BFS Traversal (Standard)

```c
bfs_traversal(Node* root){
    std::queue<Node*> q;
    q.push(root);
    
    while(!q.empty()){ 
        auto curr = q.front();
        q.pop();
        
        std::cout<<curr->val<<std::endl;
        
        if(curr->left)  q.push(curr->left);   // Add children
        if(curr->right) q.push(curr->right);  // to queue
    }
}
```

**Space = O(w)** where w = max width

**For complete binary tree**: Last level has 2^h nodes = O(n) worst case!

---

## 🌊 BFS Level-Aware - The Key Trick!

```c
bfs_traversal_order_aware(Node* root){
    std::queue<Node*> q;
    q.push(root);
    
    while(!q.empty()){ 
        int cnt = q.size();  // ⚡ CRITICAL: Capture size BEFORE loop!
        
        // Process exactly 'cnt' nodes (current level)
        while(cnt > 0){
            auto curr = q.front();
            q.pop();
            std::cout<<curr->val<<std::endl;
            
            if(curr->left)  q.push(curr->left);   // Children go to
            if(curr->right) q.push(curr->right);  // NEXT level
            
            cnt--;
        }
        
        std::cout<< std::endl;  // After each level!
    }
}
```

**⚠️ Why capture size first?**  
Queue size changes as we add children! `cnt` freezes the current level's size.

---

## 📏 Height Calculation: Post-Order (Bottom-Up)

```c
int dfs_post(Node* root){
    if(root == nullptr){
        return 0;  // NULL node = height 0
    }
    
    int l = dfs_post(root->left);   // Get left subtree height
    int r = dfs_post(root->right);  // Get right subtree height
    
    return std::max(l,r) + 1;  // My height = max + 1
}
```

**Flow**: Leaf → Parent → Root (builds answer from bottom)

---

## 📏 Height Calculation: Pre-Order (Top-Down)

```c
int dfs_pre(Node* root, int cnt, int& h){
    //                             ^^^^ 
    //                          MUST be reference!
    
    if(root == nullptr){
        h = std::max(cnt, h);  // Update global max at leaf boundary
        return;
    }
    
    dfs_pre(root->left, cnt+1, h);   // Go deeper, increment count
    dfs_pre(root->right, cnt+1, h);
}

// Usage:
// int h = 0;
// dfs_pre(root, 0, h);  // Pass h by reference
```

**🔑 Critical Detail**: `int& h` (reference parameter)

```c
// ❌ Without &:
int h_local;  // Each recursive call gets its OWN copy
              // Changes don't propagate back!

// ✅ With &:
int& h_ref;   // All recursive calls share the SAME variable
              // Changes are visible everywhere!
```

**Flow**: Root → Parent → Leaf (passes depth down, updates max at boundary)

---

## 💰 Maximum Sum Path: Post-Order

```c
int max_sum_path(Node* root){
    if(root == nullptr){
        return 0;  // No node = 0 sum
    }
    
    int l_max = max_sum_path(root->left);   // Best path in left subtree
    int r_max = max_sum_path(root->right);  // Best path in right subtree
    
    return std::max(l_max, r_max) + root->val;  // Choose better + me
}
```

**Strategy**: "Tell me the best from below, I'll add myself and pass it up"

---

## 💰 Maximum Sum Path: Pre-Order

```c
int max_sum_path2(Node* root, int sum, int& max){
    //                                    ^^^^
    //                                 Reference again!
    
    if(root == nullptr){
        max = std::max(sum, max);  // Reached end of path, update max
        return 0;
    }
    
    max_sum_path2(root->left, sum + root->val, max);   // Add me, go left
    max_sum_path2(root->right, sum + root->val, max);  // Add me, go right
}

// Usage:
// int max_val = 0;
// max_sum_path2(root, 0, max_val);
```

**Strategy**: "I'll build the sum as I go down, check it at the leaves"

---

## ⚠️ Common Pitfalls

| Mistake | Issue | Fix |
|---------|-------|-----|
| `int h` instead of `int& h` | Changes lost in recursion | Use `int& h` |
| `while(!q.empty())` inside another `while(!q.empty())` | Process children same iteration | Capture `cnt = q.size()` first |
| Forgetting `NULL` check | Segmentation fault | Always check `if(root == NULL)` first |
| Using stack for BFS | Wrong traversal order | Use queue for BFS, stack for DFS |

---

## 🎯 Quick Reference

| Algorithm | Data Structure | Space | Use Case |
|-----------|---------------|-------|----------|
| DFS Recursive | Call stack | O(h) | Simple implementation |
| DFS Iterative | Explicit stack | O(h) | Avoid stack overflow |
| BFS Standard | Queue | O(w) | Level-by-level processing |
| BFS Level-Aware | Queue + counter | O(w) | Per-level operations |

**Note**: Mixed C/C++ syntax - uses `std::stack`, `std::queue` but also `printf` and `NULL`