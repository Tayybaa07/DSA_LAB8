#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct Node {
    int key, count, height;
    Node *left, *right;
    Node(int k) : key(k), count(1), height(1), left(nullptr), right(nullptr) {}
};

enum Criterion { MAX_C = 1, MIN_C = 2, SUM_C = 3 };

class AVLTree {
    Node* root = nullptr;

    static int h(Node* n) { return n ? n->height : 0; }
    static int bf(Node* n) { return n ? h(n->left) - h(n->right) : 0; }
    static void upd(Node* n) { n->height = 1 + max(h(n->left), h(n->right)); }

    Node* rotR(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        upd(y); upd(x);
        cout << "   [rotation] right rotation at " << y->key << "\n";
        return x;
    }
    Node* rotL(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        upd(x); upd(y);
        cout << "   [rotation] left rotation at " << x->key << "\n";
        return y;
    }
    // Rebalance only if needed.
    Node* rebalance(Node* n) {
        upd(n);
        int b = bf(n);
        if (b > 1) {                         // left heavy
            if (bf(n->left) < 0) n->left = rotL(n->left);   
            return rotR(n);                 
        }
        if (b < -1) {                        // right heavy
            if (bf(n->right) > 0) n->right = rotR(n->right); 
            return rotL(n);                 
        }
        return n;
    }

    Node* insert(Node* n, int k) {
        if (!n) return new Node(k);
        if (k < n->key) n->left = insert(n->left, k);
        else if (k > n->key) n->right = insert(n->right, k);
        else { n->count++; return n; }
        return rebalance(n);
    }

    Node* minNode(Node* n) { while (n->left) n = n->left; return n; }

    // Removes one occurrence of k.
    Node* remove(Node* n, int k) {
        if (!n) return nullptr;
        if (k < n->key) n->left = remove(n->left, k);
        else if (k > n->key) n->right = remove(n->right, k);
        else {
            if (n->count > 1) { n->count--; return n; }
            if (!n->left || !n->right) {
                Node* c = n->left ? n->left : n->right;
                delete n;
                return c;
            }
            Node* s = minNode(n->right);     // inorder successor
            n->key = s->key;
            n->count = s->count;
            s->count = 1;                    // so the recursive call removes the node fully
            n->right = remove(n->right, s->key);
        }
        return rebalance(n);
    }

    void print(Node* n, int depth) {         // sideways print
        if (!n) return;
        print(n->right, depth + 1);
        cout << string(depth * 6, ' ') << n->key;
        if (n->count > 1) cout << "(x" << n->count << ")";
        cout << " [h=" << n->height << ",bf=" << bf(n) << "]\n";
        print(n->left, depth + 1);
    }

    void destroy(Node* n) { if (n) { destroy(n->left); destroy(n->right); delete n; } }

    // Collects up to k elements, largest first 
    void collectMax(Node* n, int& k, vector<int>& out) {
        if (!n || k <= 0) return;
        collectMax(n->right, k, out);
        for (int i = 0; i < n->count && k > 0; i++, k--) out.push_back(n->key);
        collectMax(n->left, k, out);
    }
    // Collects up to k elements, smallest first 
    void collectMin(Node* n, int& k, vector<int>& out) {
        if (!n || k <= 0) return;
        collectMin(n->left, k, out);
        for (int i = 0; i < n->count && k > 0; i++, k--) out.push_back(n->key);
        collectMin(n->right, k, out);
    }

public:
    ~AVLTree() { destroy(root); }
    bool empty() const { return root == nullptr; }

    void insert(int k) {
        cout << "Insert " << k << ":\n";
        root = insert(root, k);
        display();
    }
    void removeOne(int k) { root = remove(root, k); }
    void display() {
        if (!root) { cout << "   (tree is empty)\n"; return; }
        print(root, 0);
        cout << "\n";
    }

    //  SUM = the k largest elements together with their sum.
    vector<int> topK(int k, Criterion c) {
        vector<int> out;
        if (c == MIN_C) collectMin(root, k, out);
        else collectMax(root, k, out);
        return out;
    }

    void query(int k, Criterion c) {
        vector<int> v = topK(k, c);
        const char* name = c == MAX_C ? "MAX" : c == MIN_C ? "MIN" : "SUM";
        cout << "Top " << k << " (" << name << "): ";
        long long sum = 0;
        for (int x : v) { cout << x << " "; sum += x; }
        if (v.empty()) cout << "(none)";
        if (c == SUM_C) cout << "| sum = " << sum;
        cout << "\n";
    }

    // Delete top k; 
    void deleteTopK(int k, Criterion c) {
        vector<int> v = topK(k, c);
        for (int x : v) {
            cout << "Delete " << x << ":\n";
            removeOne(x);
            display();
        }
        if (v.empty()) cout << "Nothing to delete.\n";
    }
};

Criterion askCriterion() {
    int c;
    cout << "Criterion (1=Max, 2=Min, 3=Sum of top-k): ";
    cin >> c;
    return c == 2 ? MIN_C : c == 3 ? SUM_C : MAX_C;
}

int main() {
    AVLTree t;
    int ch;
    do {
        cout << "\n1. Insert multiple elements\n2. Query top-k\n3. Delete top-k\n4. Display tree\n0. Exit\nChoice: ";
        if (!(cin >> ch)) break;
        if (ch == 1) {
            int n, x;
            cout << "How many elements? ";
            cin >> n;
            cout << "Enter elements: ";
            while (n-- > 0) { cin >> x; t.insert(x); }
        } else if (ch == 2) {
            int k;
            cout << "k = ";
            cin >> k;
            t.query(k, askCriterion());
        } else if (ch == 3) {
            int k;
            cout << "k = ";
            cin >> k;
            t.deleteTopK(k, askCriterion());
        } else if (ch == 4) t.display();
    } while (ch != 0);
    return 0;
}