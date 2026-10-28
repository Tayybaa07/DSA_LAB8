#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

// Binary min heap of ListNode* ordered by val.
class MinHeap {
    vector<ListNode*> a;
    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (a[p]->val <= a[i]->val) break;
            swap(a[p], a[i]);
            i = p;
        }
    }
    void siftDown(int i) {
        int n = a.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, m = i;
            if (l < n && a[l]->val < a[m]->val) m = l;
            if (r < n && a[r]->val < a[m]->val) m = r;
            if (m == i) break;
            swap(a[m], a[i]);
            i = m;
        }
    }
public:
    bool empty() const { return a.empty(); }
    void push(ListNode* x) { a.push_back(x); siftUp(a.size() - 1); }
    ListNode* pop() {
        ListNode* top = a[0];
        a[0] = a.back();
        a.pop_back();
        if (!a.empty()) siftDown(0);
        return top;
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    MinHeap h;
    for (ListNode* head : lists)
        if (head) h.push(head);          // skip empty lists

    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (!h.empty()) {
        ListNode* smallest = h.pop();
        tail->next = smallest;           // relink existing nodes
        tail = smallest;
        if (smallest->next) h.push(smallest->next);
    }
    return dummy.next;
}

void printList(ListNode* h) {
    if (!h) { cout << "(empty)\n"; return; }
    for (; h; h = h->next) cout << h->val << (h->next ? " -> " : "\n");
}

int main() {
    int k;
    cout << "Enter number of lists k: ";
    cin >> k;
    vector<ListNode*> lists(k, nullptr);
    for (int i = 0; i < k; i++) {
        int m;
        cout << "List " << i + 1 << " - size, then sorted elements: ";
        cin >> m;
        ListNode* tail = nullptr;
        for (int j = 0; j < m; j++) {
            int x;
            cin >> x;
            ListNode* nd = new ListNode(x);
            if (!lists[i]) lists[i] = tail = nd;
            else { tail->next = nd; tail = nd; }
        }
    }
    ListNode* res = mergeKLists(lists);
    cout << "Merged list: ";
    printList(res);
    while (res) { ListNode* t = res; res = res->next; delete t; }
    return 0;
}