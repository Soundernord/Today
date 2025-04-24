#include <iostream>
using namespace std;

class BTree {
private:
    int order;
    struct Node {
        int *keys;
        Node **children;
        int keyCount;
        bool isLeaf;

        Node(int o, bool leaf = true) {
            keyCount = 0;
            isLeaf = leaf;
            keys = new int[o - 1];
            children = new Node*[o];
            for (int i = 0; i < o; i++) {
                children[i] = nullptr;
            }
        }
    };

    Node *root;

    void insertNonFull(Node* node, int key);
    void splitChild(Node* parent, int index, Node* child);
    void levelOrder(Node* node);
    void deleteKey(Node* node, int key);
    int findKey(Node* node, int key);
    void removeFromLeaf(Node* node, int index);
    void removeFromNonLeaf(Node* node, int index);
    void fill(Node* node, int index);
    void borrowFromPrev(Node* node, int index);
    void borrowFromNext(Node* node, int index);
    void merge(Node* node, int index);
    void levelOrderHelper(Node* node, int level);

public:
    BTree(int o) {
        order = o;
        root = new Node(order, true);
    }

    void insert(int key);
    void levelOrderDisplay();
    void deleteKey(int key);
    void levelOrderHelper();
};

void BTree::insertNonFull(Node* node, int key) {
    int i = node->keyCount - 1;
    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->keyCount++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->keyCount == order - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

void BTree::splitChild(Node* parent, int index, Node* child) {
    Node* newChild = new Node(order, child->isLeaf);
    newChild->keyCount = order / 2;
    for (int i = 0; i < order / 2; i++) {
        newChild->keys[i] = child->keys[i + order / 2];
    }
    if (!child->isLeaf) {
        for (int i = 0; i < order / 2 + 1; i++) {
            newChild->children[i] = child->children[i + order / 2];
        }
    }
    child->keyCount = order / 2;
    for (int i = parent->keyCount; i >= index + 1; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;
    for (int i = parent->keyCount - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    parent->keys[index] = child->keys[order / 2 - 1];
    parent->keyCount++;
}

void BTree::insert(int key) {
    if (root->keyCount == order - 1) {
        Node* newNode = new Node(order, false);
        newNode->children[0] = root;
        splitChild(newNode, 0, root);
        int i = 0;
        if (newNode->keys[0] < key) {
            i++;
        }
        insertNonFull(newNode->children[i], key);
        root = newNode;
    } else {
        insertNonFull(root, key);
    }
}

void BTree::levelOrder(Node* node) {
    if (node == nullptr) return;
    for (int i = 0; i < node->keyCount; i++) {
        cout << node->keys[i] << " ";
    }
    cout << "| ";
    for (int i = 0; i <= node->keyCount; i++) {
        levelOrder(node->children[i]);
    }
}

int BTree::findKey(Node* node, int key) {
    int index = 0;
    while (index < node->keyCount && node->keys[index] < key) {
        index++;
    }
    return index;
}

void BTree::deleteKey(int key) {
    if (!root) {
        cout << "Tree is empty\n";
        return;
    }
    deleteKey(root, key);
    if (root->keyCount == 0) {
        Node* temp = root;
        if (root->isLeaf) {
            root = nullptr;
        } else {
            root = root->children[0];
        }
        delete temp;
    }
}

void BTree::deleteKey(Node* node, int key) {
    int index = findKey(node, key);
    if (index < node->keyCount && node->keys[index] == key) {
        if (node->isLeaf) {
            removeFromLeaf(node, index);
        } else {
            removeFromNonLeaf(node, index);
        }
    } else {
        if (node->isLeaf) {
            cout << "Key not found\n";
            return;
        }
        bool flag = ((index == node->keyCount) ? true : false);
        if (node->children[index]->keyCount < order / 2) {
            fill(node, index);
        }
        if (flag && index > node->keyCount) {
            deleteKey(node->children[index - 1], key);
        } else {
            deleteKey(node->children[index], key);
        }
    }
}

void BTree::removeFromLeaf(Node* node, int index) {
    for (int i = index + 1; i < node->keyCount; i++) {
        node->keys[i - 1] = node->keys[i];
    }
    node->keyCount--;
}

void BTree::removeFromNonLeaf(Node* node, int index) {
    int key = node->keys[index];
    if (node->children[index]->keyCount >= order / 2) {
        int pred = node->children[index]->keys[node->children[index]->keyCount - 1];
        node->keys[index] = pred;
        deleteKey(node->children[index], pred);
    } else if (node->children[index + 1]->keyCount >= order / 2) {
        int succ = node->children[index + 1]->keys[0];
        node->keys[index] = succ;
        deleteKey(node->children[index + 1], succ);
    } else {
        merge(node, index);
        deleteKey(node->children[index], key);
    }
}

void BTree::fill(Node* node, int index) {
    if (index > 0 && node->children[index - 1]->keyCount >= order / 2) {
        borrowFromPrev(node, index);
    } else if (index < node->keyCount && node->children[index + 1]->keyCount >= order / 2) {
        borrowFromNext(node, index);
    } else {
        if (index < node->keyCount) {
            merge(node, index);
        } else {
            merge(node, index - 1);
        }
    }
}

void BTree::borrowFromPrev(Node* node, int index) {
    Node* child = node->children[index];
    Node* sibling = node->children[index - 1];
    for (int i = child->keyCount - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
    }
    if (!child->isLeaf) {
        for (int i = child->keyCount; i >= 0; i--) {
            child->children[i + 1] = child->children[i];
        }
    }
    child->keys[0] = node->keys[index - 1];
    if (!child->isLeaf) {
        child->children[0] = sibling->children[sibling->keyCount];
    }
    node->keys[index - 1] = sibling->keys[sibling->keyCount - 1];
    child->keyCount++;
    sibling->keyCount--;
}

void BTree::borrowFromNext(Node* node, int index) {
    Node* child = node->children[index];
    Node* sibling = node->children[index + 1];
    child->keys[child->keyCount] = node->keys[index];
    if (!child->isLeaf) {
        child->children[child->keyCount + 1] = sibling->children[0];
    }
    node->keys[index] = sibling->keys[0];
    for (int i = 1; i < sibling->keyCount; i++) {
        sibling->keys[i - 1] = sibling->keys[i];
    }
    if (!sibling->isLeaf) {
        for (int i = 1; i <= sibling->keyCount; i++) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }
    child->keyCount++;
    sibling->keyCount--;
}

void BTree::merge(Node* node, int index) {
    Node* child = node->children[index];
    Node* sibling = node->children[index + 1];
    child->keys[order / 2 - 1] = node->keys[index];
    for (int i = 0; i < sibling->keyCount; i++) {
        child->keys[i + order / 2] = sibling->keys[i];
    }
    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->keyCount; i++) {
            child->children[i + order / 2] = sibling->children[i];
        }
    }
    for (int i = index + 1; i < node->keyCount; i++) {
        node->keys[i - 1] = node->keys[i];
    }
    for (int i = index + 2; i <= node->keyCount; i++) {
        node->children[i - 1] = node->children[i];
    }
    child->keyCount += sibling->keyCount + 1;
    node->keyCount--;
    delete sibling;
}

void BTree::levelOrderHelper() {
    levelOrder(root);
    cout << endl;
}

int main() {
    int o;
    cout << "Enter the order of the B-Tree: ";
    cin >> o;

    BTree tree(o);
    int choice, key;

    do {
        cout << "\n1. Insert\n2. Display (Level Order)\n3. Delete\n4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter key to insert: ";
                cin >> key;
                tree.insert(key);
                break;
            case 2:
                tree.levelOrderHelper();
                break;
            case 3:
                cout << "Enter key to delete: ";
                cin >> key;
                tree.deleteKey(key);
                break;
            case 4:
                break;
            default:
                cout << "Invalid choice\n";
        }
    } while (choice != 4);

    return 0;
}