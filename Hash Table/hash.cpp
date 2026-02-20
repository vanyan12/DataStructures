#include <iostream>
#include <vector>

class HashTable {
private:

    struct Node{
        int data;
        Node* next;
    };


    int size_;
    std::vector<Node*> table_;

    int hash(int data, int K) {
        int idx = data % K;
        if (idx < 0){ // prevent negative indexing
            return idx += K;
        }

        return idx;
    }

    double get_balance_factor(){
        int K = table_.size();
        return (double)size_ / K;
    }

    // Time - O(1) push-front into front
    // Memory - O(1) one node is allocated
    void insert(int data, std::vector<Node*>& table){
        int idx = hash(data, table.size());

        table[idx] = new Node {data, table[idx]}; // new node points to old head

    }

    // Time - O(K + N) iterate K buckets and all N nodes are rewired once 
    // Memory - O(K)
    void resize(){
        int new_size = table_.size() * 2;

        std::vector<Node*> new_table(new_size, nullptr);

        // Rewire old nodes into new_table
        for (Node*& node : table_){
            while (node)
            {
                Node* n = node; // detach current node form old list
                node = node->next; // forward step


                int idx = hash(n->data, new_size); // calculate the new index 
                n->next = new_table[idx]; // push-front in new_table
                new_table[idx] = n;
            }
            
        }

        table_.swap(new_table);

    }

public:
    // Time - O(K)
    // Memory - O(K)
    HashTable(int cap) 
        : size_(0), 
        table_(std::max(cap, 10), nullptr) 
    {}

    ~HashTable() {
        for (Node* head : table_) {
            while (head) {
                Node* tmp = head;
                head = head->next;
                delete tmp;
            }
        }
    }

    // Time - O(1) *O(K + N) when resized
    // Memory - O(1) *O(K)
    void insert(int data){
        if(get_balance_factor() > 0.5){
            resize();
        }

        insert(data, table_);
        ++size_;

    }


    // Time - O(1) | O(N)
    // Memory - O(1)
    bool find(int data){
        if(table_[hash(data, table_.size())] == nullptr){
            return false;
        }

        Node* head = table_[hash(data, table_.size())];

        while (head)
        {
            if(head->data == data){
                return true;
            }
            head = head->next;
        }

        return false;
    }


    // Time - O(N)
    // Memory - O(1)
    void erase(int data){
        // The index in array is null
        if (table_[hash(data, table_.size())] == nullptr){
            return;
        }

        Node* node = table_[hash(data, table_.size())];

        // Head should be deleted
        if (node->data == data){
            table_[hash(data, table_.size())] = node->next;
            delete node;
            --size_;
            return;
        }

        // Value is in the list
        while (node->next != nullptr)
        {
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


    // Time - O(K + N)
    // Memory - O(1)
    void print(){
        for(auto head : table_){
            while(head){
                std::cout<<head->data<<std::endl;
                head = head->next;
            }
        }
    }

    int size(){
        return size_;
    }
};

int main(){

    HashTable ht(10);

    std::cout << "=== Insert test ===\n";

    ht.insert(1);
    ht.insert(5);
    ht.insert(6);
    ht.insert(15);
    ht.insert(11);
    ht.insert(49);

    std::cout << "=== Size test (should be 6) ===\n" << ht.size() << "\n\n";

    std::cout << "=== Print table ===\n";
    ht.print();
    std::cout << "\n";

    std::cout << "=== Find test ===\n";
    std::cout << "Find 1 (expect 1): " << ht.find(1) << "\n";
    std::cout << "Find 6 (expect 1): " << ht.find(6) << "\n";
    std::cout << "Find 99 (expect 0): " << ht.find(99) << "\n\n";

    std::cout << "=== Insert duplicates ===\n";
    ht.insert(1);
    ht.insert(1);
    std::cout << "Size after duplicates (expect 8): " << ht.size() << "\n\n";


    std::cout << "=== Erase test ===\n";
    ht.erase(1);  // erase one occurrence
    std::cout << "Size after erase one 1 (expect 7): " << ht.size() << "\n";
    std::cout << "Find 1 (expect 1, still exists): " << ht.find(1) << "\n\n";

    std::cout << "=== Resize stress test ===\n";
    for (int i = 0; i < 100; ++i) {
        ht.insert(i);
    }
    std::cout << "Size after bulk insert (expect 100+?): " << ht.size() << "\n";
    std::cout << "Find 50 (expect 1): " << ht.find(50) << "\n";
    std::cout << "Find 150 (expect 0): " << ht.find(150) << "\n\n";

    std::cout << "=== Negative values test ===\n";
    ht.insert(-12);
    ht.insert(-7);
    std::cout << "Find -12 (expect 1): " << ht.find(-12) << "\n";
    std::cout << "Find -7 (expect 1): " << ht.find(-7) << "\n";

    return 0;
}

