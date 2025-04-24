#include <iostream>
#include <string>
using namespace std;

enum Color { RED, BLACK };

class RBTree {
private:
    struct Node {
        string name;
        Color color;
        Node *left, *right, *parent;

        Node(string d) {
            name = d;
            color = RED;
            left = right = parent = nullptr;
        }
    };

    Node *root;
    Node *NIL;

    void rotateLeft(Node *x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != NIL)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node *x) {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != NIL)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node *k) {
        Node *u;
        while (k->parent != nullptr && k->parent->color == RED) {
            if (k->parent == k->parent->parent->left) {
                u = k->parent->parent->right;
                if (u->color == RED) {
                    k->parent->color = BLACK;
                    u->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        rotateLeft(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rotateRight(k->parent->parent);
                }
            } else {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    k->parent->color = BLACK;
                    u->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rotateRight(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rotateLeft(k->parent->parent);
                }
            }
            if (k == root)
                break;
        }
        root->color = BLACK;
    }

    void inorder(Node *node) {
        if (node != NIL) {
            inorder(node->left);
            cout << node->name << " [" << (node->color == RED ? "R" : "B") << "]  ";
            inorder(node->right);
        }
    }

    Node* search(Node *node, string key) {
        if (node == NIL || key == node->name)
            return node;
        if (key < node->name)
            return search(node->left, key);
        return search(node->right, key);
    }

    void transplant(Node *u, Node *v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    void fixDelete(Node *x) {
        Node *s;
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->right->color == BLACK) {
                        s->left->color = BLACK;
                        s->color = RED;
                        rotateRight(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    s = x->parent->left;
                }
                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->left->color == BLACK) {
                        s->right->color = BLACK;
                        s->color = RED;
                        rotateLeft(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

public:
    RBTree() {
        NIL = new Node("");
        NIL->color = BLACK;
        root = NIL;
    }

    void insert(string name) {
        Node *node = new Node(name);
        node->left = node->right = node->parent = NIL;

        Node *y = nullptr;
        Node *x = root;

        while (x != NIL) {
            y = x;
            if (node->name < x->name)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = y;
        if (y == nullptr)
            root = node;
        else if (node->name < y->name)
            y->left = node;
        else
            y->right = node;

        node->left = NIL;
        node->right = NIL;
        node->color = RED;

        fixInsert(node);
    }

    void deleteDirectory(string name) {
        Node *z = root;
        while (z != NIL) {
            if (z->name == name)
                break;
            else if (name < z->name)
                z = z->left;
            else
                z = z->right;
        }

        if (z == NIL) {
            cout << "Directory not found!\n";
            return;
        }

        Node *y = z;
        Node *x;
        Color yOriginalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = z->right;
            while (y->left != NIL) {
                y = y->left;
            }
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        if (yOriginalColor == BLACK) {
            fixDelete(x);
        }
    }

    void display() {
        cout << "\nDirectory Structure:\n";
        inorder(root);
        cout << "\n";
    }
};

int main() {
    RBTree tree;
    int ch;
    string name;

    do {
        cout << "\n1. Create Directory\n2. Delete Directory\n3. Display Structure\n4. Exit\nEnter choice: ";
        cin >> ch;
        switch (ch) {
            case 1:
                cout << "Enter directory name: ";
                cin >> name;
                tree.insert(name);
                break;
            case 2:
                cout << "Enter directory name to delete: ";
                cin >> name;
                tree.deleteDirectory(name);
                break;
            case 3:
                tree.display();
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice\n";
        }
    } while (ch != 4);

    return 0;
}