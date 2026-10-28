#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct HNode {
    char ch;
    int freq;
    HNode *left, *right;
    HNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Binary min heap of HNode* ordered by frequency.
class MinHeap {
    vector<HNode*> a;
    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (a[p]->freq <= a[i]->freq) break;
            swap(a[p], a[i]);
            i = p;
        }
    }
    void siftDown(int i) {
        int n = a.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, m = i;
            if (l < n && a[l]->freq < a[m]->freq) m = l;
            if (r < n && a[r]->freq < a[m]->freq) m = r;
            if (m == i) break;
            swap(a[m], a[i]);
            i = m;
        }
    }
public:
    int size() const { return a.size(); }
    void push(HNode* x) { a.push_back(x); siftUp(a.size() - 1); }
    HNode* pop() {
        HNode* top = a[0];
        a[0] = a.back();
        a.pop_back();
        if (!a.empty()) siftDown(0);
        return top;
    }
};

HNode* buildHuffman(const string& S, const vector<int>& f) {
    MinHeap h;
    for (size_t i = 0; i < S.size(); i++) h.push(new HNode(S[i], f[i]));
    if (h.size() == 0) return nullptr;
    while (h.size() > 1) {
        HNode* x = h.pop();           
        HNode* y = h.pop();            
        HNode* z = new HNode('$', x->freq + y->freq);  // internal node
        z->left = x;
        z->right = y;
        h.push(z);
    }
    return h.pop();
}

// Preorder
void preorder(HNode* root, string code, vector<pair<char, string>>& out) {
    if (!root) return;
    if (!root->left && !root->right) {      // leaf
        out.push_back({root->ch, code.empty() ? "0" : code});
        return;
    }
    preorder(root->left, code + "0", out);
    preorder(root->right, code + "1", out);
}

void freeTree(HNode* r) {
    if (!r) return;
    freeTree(r->left);
    freeTree(r->right);
    delete r;
}

int main() {
    int n;
    string S;
    cout << "Enter N: ";
    cin >> n;
    cout << "Enter " << n << " distinct characters (as one string, e.g. abcdef): ";
    cin >> S;
    vector<int> f(n);
    cout << "Enter " << n << " frequencies: ";
    for (int i = 0; i < n; i++) cin >> f[i];

    HNode* root = buildHuffman(S, f);
    vector<pair<char, string>> codes;
    preorder(root, "", codes);

    cout << "\nHuffman codes (preorder):\n";
    for (auto& p : codes) cout << p.first << " : " << p.second << "\n";
    cout << "Codes only: ";
    for (auto& p : codes) cout << p.second << " ";
    cout << "\n";
    freeTree(root);
    return 0;
}