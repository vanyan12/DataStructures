# Tree Traversal Algorithms

This file contains various implementations of tree traversal algorithms in C/C++, demonstrating both depth-first search (DFS) and breadth-first search (BFS) approaches, along with practical applications.

## Overview

The code implements:
- **DFS Traversals**: In-order (recursive and iterative)
- **BFS Traversals**: Standard and level-aware variants
- **Practical Applications**: Tree height calculation and maximum sum path finding

---

## Important Concepts & Subtle Details

### 1. DFS In-Order Traversal (Recursive)

**Time Complexity**: O(n)  
**Space Complexity**: O(h) where h is the height of the tree
- O(log n) for balanced trees
- O(n) for skewed trees

**Key Insight**: The space complexity comes from the function call stack. At any point during recursion, the stack contains at most h function calls (the depth of the current path).

**Traversal Order**: Left → Root → Right

### 2. DFS In-Order Traversal (Iterative)

**Time Complexity**: O(n)  
**Space Complexity**: O(h)

**Subtle Implementation Details**:
- Uses an explicit stack instead of the call stack
- **Two-phase approach**:
  1. First, push all left children until reaching NULL (lines 33-36)
  2. Then, for each popped node, process it and repeat the same for its right subtree (lines 47-50)

**Why this works**: This mimics the recursive approach by ensuring all left children are processed before the parent, then the right children.

### 3. BFS Traversal (Standard)

**Time Complexity**: O(n)  
**Space Complexity**: O(w) where w is the maximum width of the tree

**Critical Note**: For a complete binary tree, the last level contains 2^h nodes. Since h = log n, the space complexity becomes O(2^(log n)) = O(n) in the worst case.

**Implementation**: Uses a queue to process nodes level by level.

### 4. BFS Traversal (Level-Aware)

**Key Innovation**: The use of `int cnt = q.size()` (line 98)

**Why this matters**:
- Captures the number of nodes at the current level BEFORE adding children
- Allows processing each level separately
- Enables level-based operations (e.g., printing a newline after each level)

**Subtle Detail**: The count must be captured before the inner while loop, not recalculated inside it, because the queue size changes as children are added.

### 5. Tree Height Calculation - Post-Order Approach

**Strategy**: Bottom-up (post-order traversal)

**How it works**:
- Recurses to the leaves first
- Returns 0 for NULL nodes
- Each node returns `max(left_height, right_height) + 1`

**Advantage**: Clean and intuitive - height is naturally computed from leaf to root.

### 6. Tree Height Calculation - Pre-Order Approach

**Strategy**: Top-down (pre-order traversal)

**Critical Detail**: Uses **pass-by-reference** (`int& h`) on line 147

**Why reference is essential**:
- Without `&`, each recursive call gets a copy of `h`
- Changes in deeper recursion levels wouldn't affect the original variable
- With `&`, all recursive calls share the same `h` variable

**How it works**:
- `cnt` tracks the current depth (incremented as we go deeper)
- When reaching NULL (past a leaf), compare `cnt` with current max height
- The reference parameter `h` accumulates the maximum across all paths

**Usage Pattern**:
```c
int h = 0;
dfs_pre(root, 0, h);
// h now contains the height
```

### 7. Maximum Sum Path - Post-Order Variant

**Approach**: Bottom-up calculation

**Logic**:
- Returns 0 for NULL nodes
- For each node, calculates max sum from left and right subtrees
- Returns the larger sum plus current node's value

**Subtle Point**: This finds the maximum sum from root to any leaf by building up from the leaves.

### 8. Maximum Sum Path - Pre-Order Variant

**Approach**: Top-down calculation

**Key Differences from post-order**:
- Accumulates sum as it traverses down (`sum + root->val`)
- Uses a reference parameter `max` to track the global maximum
- Compares the accumulated sum at NULL nodes (leaf boundaries)

**Why sum is accumulated in the recursive call** (line 186-187): The sum includes the current node before recursing to children, building the path sum from root to leaf.

---

## Common Pitfalls & Best Practices

1. **Stack vs Queue Confusion**: DFS uses a stack (or recursion), BFS uses a queue. Mixing these will break the traversal order.

2. **Level-Aware BFS**: Always capture `q.size()` before the inner loop; otherwise, you'll process children added during the same iteration.

3. **Reference Parameters**: When you need to modify a value across recursive calls (like `h` or `max`), use `int&` to pass by reference.

4. **NULL Checks**: Always check for NULL at the beginning of recursive functions to avoid segmentation faults.

5. **Space Complexity Analysis**: 
   - Recursive DFS: Call stack depth = tree height
   - Iterative DFS: Explicit stack size = tree height  
   - BFS: Queue size = maximum tree width (can be O(n) for complete trees)

---

## Syntax Notes

⚠️ **Mixed C/C++ Syntax**: This file mixes C and C++ features:
- Uses `std::stack`, `std::queue`, `std::cout` (C++)
- Uses `printf` and `NULL` (C convention)
- For consistency, consider using either all C++ (`nullptr`, `std::cout`) or all C (`printf`, custom stack/queue implementations)

---

## Applications

These traversal patterns are fundamental for:
- **Binary Search Trees**: In-order gives sorted output
- **Expression Trees**: Pre/post/in-order for different notations
- **Level-Order Processing**: Finding tree width, level-based serialization
- **Path Problems**: Maximum path sum, path existence, etc.
