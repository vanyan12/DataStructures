#include <stdio.h>
#include <stdlib.h>

// T(n) = O(n)
// Space = O(n) | O(logn)

// Recursion uses function call stack, where at any time could be at most h of tree calls
// For balanced trees that will become O(logn) and for skewed trees O(n)
dfs_inorder(Node* root){
    if(root == NULL){
        return
    }

    dfs_inorder(root->left);
    printf("%d ", root->val); // process the root
    dfs_inorder(root->right);
}


// T(n) = O(n)
// Space = O(h) where h is the height of the tree

// We sue stack where at any time could be at most h of tree nodes
// For balanced trees that will become O(logn) and for skewed trees O(n)
def_inorder_iter(Node* root){
    if(root == NULL){
        return
    }

    std::stack<Node*> s;

    // Go in depth untill you encounter NULL
    while(root){
        s.push(root);
        root = root->left;
    }

    while(!s.empty()){
        Node* top = s.top();

        s.pop();

        std::cout<<top->val<<std::endl;

        Node* curr = top->right;

        while(curr){
            s.push(curr);
            curr = curr->left;
        }
    }
    
}

// T(n) = O(n)
// Space = O(w) where w is the max width of the tree

// If tree is fully binary tree => deepest level has 2^h nodes where h is height of tree. h = logN => 2^logN = N so S(n) = O(n)
bfs_traversal(Node* root){
    if(root == NULL){
        return;
    }

    std::queue<Node*> q;

    q.push(root);

    while(!q.empty()){
        auto curr = q.front();

        q.pop();

        std::cout<<curr->val<<std::endl;

        if(curr->left){
            q.push(curr->left);
        }

        if(curr->right){
            q.push(curr->right);
        }

    }

}


bfs_traversal_order_aware(Node* root){
    if(root == NULL){
        return;
    }

    std::queue<Node*> q;

    q.push(root);

    while(!q.empty()){
        int cnt = q.size(); // # of nodes at each level 

        // This makes the traversal level aware
        // When cnt is 0 we have processed the level and could to some actions
        while(cnt > 0){
            auto curr = q.front();

            q.pop();

            std::cout<<curr->val<<std::endl;

            if(curr->left){
                q.push(curr->left);
            }

            if(curr->right){
                q.push(curr->right);
            }

            cnt--;

        }

        std::cout<< std::endl; // eg. new line after each level
    }
}


// Calculates height (max depth) of tree
// Uses post-order traversal, passing information (height of node) from bottom to up
int dfs_post(Node* root){
    if(root == nullptr){
        return 0;
    }

    int l = dfs_post(root->left);
    int r = dfs_post(root->right;

    return std::max(l,r) + 1;
}

// Calculates height (max depth) of tree
// Uses pre-order traversal, passing information from top to bottom


// int& - menas passing int type variable h by reference
// Without the &, the function will create a local copy of h, 
// and any updates inside the function won’t affect the original variable in main

int dfs_pre(Node* root, int cnt, int& h){
    // If reaches leaf node, assign to height the max of cnt(which is current level) and h(which is the height itself)
    // Why max? Cuz there are many leaves, in each case it should compare already found height with its level
    if(root == nullptr){
        h = std::max(cnt, h);
        return;
    }

    dfs_pre(root->left, cnt+1, h);
    dfs_pre(root->right, cnt+1, h);

}

// Usage:
// int h = 0;
// dfs_pre(root, 0, h);


// TASK: find the path of the tree, which has the biggest sum of nodes

// Post-order variant
int max_sum_path(Node* root){
    if(root == nullptr){
        return 0;
    }

    int l_max = max_sum_path(root->left);
    int r_max = max_sum_path(root->right);

    return std::max(l_max, r_max) + root->val;
}

// Pre-order variant
int max_sum_path2(Node* root, int sum, int& max){
    if(root == nullptr){
        max = std::max(sum, max);
        return 0;
    }

    max_sum_path2(root->left, sum + root->val, max);
    max_sum_path2(root->right, sum + root->val, max);

}


