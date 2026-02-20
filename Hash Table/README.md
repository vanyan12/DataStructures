# Hash Table Implementation - Deep Dive

## Overview

This is a **chaining-based hash table** that resolves collisions by storing multiple values in each bucket as a linked list. It dynamically resizes when load factor exceeds 50%.

---

## Data Structure

```
HashTable (size: 6, capacity: 10)
┌─────────────────────────────────────────┐
│ table[0] → Node(1) → Node(11)           │
│ table[1] → nullptr                      │
│ table[2] → Node(5)                      │
│ table[3] → nullptr                      │
│ table[4] → nullptr                      │
│ table[5] → Node(6) → Node(15)           │
│ table[6] → nullptr                      │
│ table[7] → nullptr                      │
│ table[8] → nullptr                      │
│ table[9] → Node(49)                     │
└─────────────────────────────────────────┘
```

Each bucket is a **singly-linked list** of nodes, allowing multiple values to coexist when hash collisions occur.

---

## Tricky Parts Explained

### 1. **Negative Modulo Handling** ⚠️

**The Problem:**
```cpp
int hash(int data, int K) {
    int idx = data % K;
    if (idx < 0){           // ← Why is this needed?
        return idx += K;
    }
    return idx;
}
```

In C++, `data % K` can return **negative values** when `data` is negative:
```
-12 % 10 = -2  (NOT 8!)
-7  % 10 = -7  (NOT 3!)
```

**Solution:** Add `K` to convert:
```
-2 + 10 = 8  ✓
-7 + 10 = 3  ✓
```

**Impact:** Without this, you'd get out-of-bounds array access and crashes.

---

### 2. **Private Helper vs Public Insert** 🎯

**Two different `insert()` functions:**

```cpp
// PRIVATE - Internal rehashing helper (push-front, very fast)
void insert(int data, std::vector<Node*>& table){
    int idx = hash(data, table.size());
    table[idx] = new Node {data, table[idx]};  // new node points to old head
}

// PUBLIC - User-facing API
void insert(int data){
    if(get_balance_factor() > 0.5){  // Check load (size / capacity)
        resize();  // Dynamically grow when too full
    }
    insert(data, table_);  // Call private helper
    ++size_;
}
```

**Why two versions?**
- **Private `insert(data, table)`**: Used during resizing to rehash nodes without counting them twice.
- **Public `insert(data)`**: Checks if table is too full, resizes if needed, then inserts.

**Critical:** The private version does **push-front** (puts new node at head of chain). This is much faster than searching to the end.

---

### 3. **Resize Logic & Node Reuse** 🔄

```cpp
void resize(){
    int new_size = table_.size() * 2;
    std::vector<Node*> new_table(new_size, nullptr);

    for (Node*& node : table_){  // Reference to pointer (important!)
        while (node){
            Node* n = node;        // Grab current node
            node = node->next;     // Advance old list pointer
            
            int idx = hash(n->data, new_size);
            n->next = new_table[idx];  // Relink to new bucket
            new_table[idx] = n;
        }
    }

    table_.swap(new_table);  // Swap the entire vector object
}
```

**Why this works:**

1. **`Node*& node`** - This is a reference to a pointer. Allows `node = node->next` to safely update the table's pointers.

2. **Nodes are REUSED** - We don't create new nodes. We just take existing ones and move them to new buckets.

3. **Rehashing** - For each node, we recalculate where it belongs in the bigger table using `hash()` with the new size.

4. **`table_.swap(new_table)`** - Swaps the two vectors. Old vector is destroyed when function ends.

**Flow:**
```
Old table_:    [Node chains with size 10]
                        ↓
        Iterate and rehash all nodes
                        ↓
new_table:     [Node chains with size 20]
                        ↓
        Swap: table_ now = new_table
                        ↓
Old vector automatically deleted (destructor called)
```

---

### 4. **Load Factor Threshold** ⚖️

```cpp
double get_balance_factor(){
    int K = table_.size();
    return (double)size_ / K;
}

void insert(int data){
    if(get_balance_factor() > 0.5){  // 50% threshold
        resize();
    }
    insert(data, table_);
    ++size_;
}
```

**What happens at 0.5?**

Load Factor = (number of items) / (bucket capacity)

```
At 0.5:
- 5 items in 10 buckets
- On average 0.5 items per bucket
- This is a good safe point before chains get long
```

**Why check BEFORE insert?**
- If you check after inserting, you might temporarily exceed 0.5
- Better to resize proactively before table gets crowded

**Resizing doubles capacity:**
```cpp
int new_size = table_.size() * 2;  // 10 → 20 → 40 → 80...
```

After resize, load factor drops back down:
```
Was: 5 items, 10 buckets = 0.5
After adding 1 more: 6 items, 10 buckets = 0.6 → RESIZE
Now: 6 items, 20 buckets = 0.3 (much better)
```

---

### 5. **Erase Head Node Handling** 🗑️

```cpp
void erase(int data){
    if (table_[hash(data, table_.size())] == nullptr){
        return;  // Bucket is empty, nothing to delete
    }

    Node* node = table_[hash(data, table_.size())];

    // Case 1: Head node matches
    if (node->data == data){
        table_[hash(data, table_.size())] = node->next;  // Update bucket!
        delete node;
        --size_;
        return;
    }

    // Case 2: Node is in the middle or end of chain
    while (node->next != nullptr){
        if (node->next->data == data){
            Node* tmp = node->next->next;
            delete node->next;
            node->next = tmp;
            --size_;
            return;
        }
        node = node->next;
    }
}
```

**Why two cases?**

**Head deletion (Case 1):**
- The node at the bucket is being deleted
- Must update `table_[idx]` to point to the next node
- Otherwise bucket pointer becomes dangling

**Interior deletion (Case 2):**
- Node is somewhere in the chain
- Update the previous node's `next` pointer to skip the deleted node

**Example:**
```
Bucket before erase(11):
table[1] → Node(11) → Node(1) → nullptr

Erase(11):
- Find 11 at head
- Update table[1] = Node(1)
- Delete the Node(11)

Bucket after:
table[1] → Node(1) → nullptr
```

If you forget to update the bucket pointer, you get a **crash** when you try to access the deleted node.

---

## Complexity Analysis

### Time Complexity

| Operation | Best Case | Average Case | Worst Case |
|-----------|-----------|--------------|------------|
| **insert** | O(1) | O(1) * | O(n) |
| **find** | O(1) | O(1) * | O(n) |
| **erase** | O(1) | O(1) * | O(n) |
| **resize** | O(n) | O(n) | O(n) |

*Average case assumes load factor stays around 0.5 via resizing

### Detailed Breakdown

**insert(data):**
```
1. Calculate hash:    ~instant
2. Check if resize:   ~instant
   If yes: rehash all nodes = full table work
3. Add new node:      ~instant
```

**find(data):**
```
1. Calculate hash:    ~instant
2. Walk through chain: anywhere from 1 to n nodes
```

**erase(data):**
```
1. Calculate hash:    ~instant
2. Find in chain:     anywhere from 1 to n nodes
3. Delete:            ~instant
```

**resize():**
```
1. Create new array:       ~instant
2. For each bucket:        K times
   For each node:          N times total (not K×N!)
     Rehash and relink:    ~instant
━━━━━━━━━━━━━━━━━━━━━━
Total:                    O(n + k)
```

### Space Complexity

```cpp
HashTable ht(10);  // 10 buckets, no data yet

ht.insert(5);      // table uses: 10 pointers + 1 node = O(10)
ht.insert(5);      // table uses: 10 pointers + 2 nodes = O(10 + 2)
// ... insert 3 more times
// size = 5, load = 0.5, total_items = 5 + capacity = 15

ht.insert(6);      // Exceeds 0.5! RESIZE called
// Now: 20 buckets + 6 nodes = O(26)
```

Total space = bucket array + actual nodes = O(capacity + size)

---

## Key Implementation Detail: Node Reuse During Resize

**Your implementation reuses nodes instead of copying them:**

```cpp
// INSIDE RESIZE
Node* n = node;           // Just grab the pointer
n->next = new_table[idx]; // Relink it
new_table[idx] = n;       // Move to new bucket
```

**Efficient:**
- No allocation of new nodes
- No copying data
- Just pointer reassignment

**IF you copied instead (bad):**
```cpp
Node* copy = new Node{n->data, nullptr};  // ← Extra allocation!
new_table[idx] = copy;                    // ← Copy created
// Later have to delete both old and new
```

This saves memory and time during resize.

---

## Edge Cases & Gotchas

### ❌ Negative Numbers
```cpp
ht.insert(-12);
ht.find(-12);  // Works! Handled by modulo fix
```

### ❌ Duplicates Allowed
```cpp
ht.insert(5);
ht.insert(5);  // Both stored, size increases
ht.size();     // Returns 2
```
The implementation allows duplicates through chaining.

### ❌ Erase Only Removes First Match
```cpp
ht.insert(5);
ht.insert(5);
ht.erase(5);   // Removes only first occurrence
ht.find(5);    // Still true!
```

### ⚠️ Load Factor Timing
```cpp
if(get_balance_factor() > 0.5){  // Check BEFORE insert
    resize();
}
insert(data, table_);
```
If you check after insert, you might exceed 0.5 briefly before the next insert triggers resize.

---

## Your Implementation Status

✅ **Handles negative keys** - `if (idx < 0) idx += K`  
✅ **Dynamic resizing** - Doubles capacity when load > 0.5  
✅ **Fast insertion** - Push-front into chains  
✅ **Full memory cleanup** - Destructor frees all nodes  
✅ **Collision handling** - Chaining via linked lists  
✅ **Test coverage** - Comprehensive main() tests  
❌ **Prevents duplicates** - Values can repeat  
❌ **Doesn't shrink** - Never reduces capacity  

---

## Test Cases Your Code Covers

From your `main()` function:

1. **Insert and size check** - Verify count is correct
2. **Print all values** - Traverses all buckets and chains
3. **Find existing and non-existing** - Tests lookup
4. **Duplicate insertion** - Allows same value multiple times
5. **Erase with duplicates** - Removes only first occurrence
6. **Bulk insert and resize** - Handles 100+ items
7. **Negative values** - Tests modulo fix for negative numbers

