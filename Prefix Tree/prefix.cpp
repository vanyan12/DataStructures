#include <array>
#include <string_view>
#include <string>
#include <vector>


class PrefixTree {
    struct Node {
        // {} means default (value) initialization. int - 0, bool - false, ptr - nullptr
        std::array<Node*, 26> next{};
        bool is_word{};
        int cnt{};

        // Returns the node pointer from the current nodes next(array of 26 nodes) array with corresponding index of c
        Node* get_next(char c){
            return next[c - 'a'];
        }

        void set_next(char c){
            next[c - 'a'] = new Node();
        }
    };

private:
    Node* root_;

    // Time - O(M) M is the total number of nodes in tree/subtree 
    // Memory - 
    void dfs_all_words(Node* root, std::string& path, std::vector<std::string>& words){
        //finds all words starting from root node

        if(!root) return;


        for(int i = 0; i < 26; i++){
            auto child = root->next[i];
            if(child){
                char c = static_cast<char>('a' + i); // get the corresponding char for the child in the array

                path.push_back(c);
                if(child->is_word){
                    words.push_back(path);
                }

                dfs_all_words(child, path, words);

                path.pop_back(); // remove the last letter when recursive call is returned


            }
        }
    }


public:

    PrefixTree(){
        root_ = new Node();
    }


    // Time - O(L) where L is length of word
    // Space - O(L)
    void insert(std::string_view word) {
        Node* curr = root_;


        for (char c : word) {

            Node* next = curr->get_next(c);

            if(!next){
                curr->set_next(c);
            }
            
            curr = curr->get_next(c);

        }

        curr->is_word = true;
        curr->cnt++;
    }

    void erase(std::string_view word){
        Node* curr = root_;

        for (int i = 0; i < word.size(); i++){

            char c = word[i];

            delete curr->next[i];

            curr = curr->get_next(c);

        }
    }


    // Time - O(L) we should loop through word letters
    // Space - O(1) cuz there is no extra allocations, we use curr which does not depend on word length 
    bool find(std::string_view word) {
        Node* curr = root_;
        for (char c : word){
            curr = curr->get_next(c);

            if(!curr){
                return false;
            }
        }

        return curr->is_word; // maybe that word is the part of other word, but is not word itself
    }

    std::vector<std::string> get_all_words(Node* root){
        std::string path;
        std::vector<std::string> words;

        dfs_all_words(root_, path, words);
        return words;
    }

    // Time - O(|prefix| + M_sub)
    std::vector<std::string> prefix_find(std::string_view prefix) { 
        Node* curr = root_;
        
        for (auto c : prefix){
            Node* next = curr->get_next(c);

            if(!next){
                return{};
            }

            curr = next;
        }

        std::string path(prefix); // without this, dfs_all_words will return only sufficies. So we init at the begining path with prefix
        std::vector<std::string> words;

        // if the prefix itself is a word add it too
        if(curr->is_word){
            words.push_back(path);
        }

        dfs_all_words(curr, path, words);
        return words;

    }


};