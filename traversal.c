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
