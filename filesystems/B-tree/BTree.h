#pragma once

#include <vector>


class BTreeNode {
public:
    explicit BTreeNode(int t, bool leaf);
    ~BTreeNode();

    BTreeNode* search(int key);

    bool isFull();

    void insertNonFull(int key);
    void splitChild(int childIndex, BTreeNode* child);

    void Dump(int depth);

    std::vector<int> getElements();
private:
    bool isLeaf;
    int minimalDegree;
    int KeysNum;

    int* Keys;
    BTreeNode** Children;

    friend class BTree;
};


class BTree {
public:
    explicit BTree(int t);
    ~BTree();

    BTreeNode* search(int key);

    std::vector<int> getElements();

    BTree* merge(BTree* anotherTree);

    void insert(int key);
    void Dump();
private:
    BTreeNode* Root;
    int minimalDegree;
};
