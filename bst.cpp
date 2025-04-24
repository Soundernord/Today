#include<iostream>
using namespace std;

// Node structure for BST
struct node {
    string key;
    string val;
    node* left;
    node* right;

    node(string k, string v) : key(k), val(v), left(nullptr), right(nullptr) {}
};

// Queue node structure
struct queue {
    node* address;
    queue* next;
    queue() : address(nullptr), next(nullptr) {}
};

// Custom Queue class
class Q {
    queue* front;
    queue* rear;
public:
    Q() : front(nullptr), rear(nullptr) {}

    int isempty() {
        return front == nullptr;
    }

    void enqueue(node* data) {
        queue* newnode = new queue();
        newnode->address = data;
        newnode->next = nullptr;

        if (isempty()) {
            front = rear = newnode;
        } else {
            rear->next = newnode;
            rear = newnode;
        }
    }

    void dequeue() {
        if (!isempty()) {
            queue* temp = front;
            front = front->next;
            delete temp;
            if (front == nullptr)
                rear = nullptr;
        }
    }

    node* peek() {
        return front->address;
    }
};

// Dictionary using Binary Search Tree
class dictionary {
    node* root;

public:
    dictionary() : root(nullptr) {}

    node* insert(node* root, string key, string value) {
        node* newNode = new node(key, value);
        if (root == nullptr) return newNode;

        node* current = root;
        node* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (key < current->key)
                current = current->left;
            else if (key > current->key)
                current = current->right;
            else {
                current->val = value;
                delete newNode;
                return root;
            }
        }

        if (key < parent->key)
            parent->left = newNode;
        else
            parent->right = newNode;

        return root;
    }

    void insertion() {
        string k, v;
        cout << "Enter the key: ";
        cin >> k;
        cout << "Enter the value: ";
        cin >> v;
        root = insert(root, k, v);
    }

    node* findMin(node* root) {
        while (root && root->left)
            root = root->left;
        return root;
    }

    node* deleteNode(node* root, string key) {
        if (!root) return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            if (!root->left) {
                node* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                node* temp = root->left;
                delete root;
                return temp;
            }
            node* temp = findMin(root->right);
            root->key = temp->key;
            root->val = temp->val;
            root->right = deleteNode(root->right, temp->key);
        }
        return root;
    }

    void deletion(string key) {
        root = deleteNode(root, key);
    }

    void inorder(node* curr) {
        if (curr != nullptr) {
            inorder(curr->left);
            cout << curr->key << " : " << curr->val << endl;
            inorder(curr->right);
        }
    }

    node* searching(string key) {
        node* curr = root;
        while (curr != nullptr) {
            if (curr->key > key)
                curr = curr->left;
            else if (curr->key < key)
                curr = curr->right;
            else {
                cout << key << " found with value: " << curr->val << endl;
                return curr;
            }
        }
        cout << "Key not found!\n";
        return nullptr;
    }

    node* m_image(node* curr) {
        if (curr == nullptr) return nullptr;
        node* newnode = new node(curr->key, curr->val);
        newnode->left = m_image(curr->right);
        newnode->right = m_image(curr->left);
        return newnode;
    }

    node* c_dictionary(node* copy) {
        if (copy == nullptr) return nullptr;
        node* newnode = new node(copy->key, copy->val);
        newnode->left = c_dictionary(copy->left);
        newnode->right = c_dictionary(copy->right);
        return newnode;
    }

    void d_level(node* curr) {
        if (curr == nullptr) return;
        Q q;
        q.enqueue(curr);
        while (!q.isempty()) {
            node* temp = q.peek();
            q.dequeue();
            cout << temp->key << " : " << temp->val << endl;
            if (temp->left != nullptr) q.enqueue(temp->left);
            if (temp->right != nullptr) q.enqueue(temp->right);
        }
    }

    node* get_root() { return root; }
};

// Main function
int main() {
    dictionary d;
    int choice;

    while (true) {
        cout << "\n1. Insert\n2. Delete\n3. Search\n4. Display\n5. Mirror\n6. Copy\n7. Level-wise Display\n8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                d.insertion();
                break;

            case 2: {
                string key;
                cout << "Enter the key: ";
                cin >> key;
                d.deletion(key);
                break;
            }

            case 3: {
                string key;
                cout << "Enter the key: ";
                cin >> key;
                d.searching(key);
                break;
            }

            case 4:
                d.inorder(d.get_root());
                break;

            case 5: {
                cout << "Mirror image of tree:\n";
                node* mirrorRoot = d.m_image(d.get_root());
                d.inorder(mirrorRoot);
                break;
            }

            case 6: {
                cout << "Copied tree:\n";
                node* copyRoot = d.c_dictionary(d.get_root());
                d.inorder(copyRoot);
                break;
            }

            case 7:
                d.d_level(d.get_root());
                break;

            case 8:
                return 0;

            default:
                cout << "Invalid choice\n";
        }
    }
}
