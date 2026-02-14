#include <iostream>
#include <vector>
#include <queue>
#include <set>

struct Node {
    int val;
    std::vector<Node*> N; // neighbors
};

void bfs_graph(Node* root) {
    if (root == nullptr) {
        return;
    }

    std::set<Node*> seen;
    std::queue<Node*> q;

    q.push(root);
    seen.insert(root);

    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();

        std::cout << curr->val << std::endl;

        // Loop through all neighbors of current node
        for (auto next : curr->N) {  
            if (seen.find(next) == seen.end()) { // not visited yet
                q.push(next);
                seen.insert(next); // mark as seen
            }
        }
    }
}
