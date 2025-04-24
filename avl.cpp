 #include <iostream>
#include <string>
using namespace std;

struct node {
    int player_id;
    int score;
    int height;
    node *left, *right;

    node(int id, int s) {
        player_id = id;
        score = s;
        height = 1;
        left = right = nullptr;
    }
};

class avltree {
    node* root;

    int getheight(node* n) {
        return n ? n->height : 0;
    }

    int getbalance(node* n) {
        return n ? getheight(n->left) - getheight(n->right) : 0;
    }

    node* rotate_right(node* y) {
        node* x = y->left;
        node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getheight(y->left), getheight(y->right)) + 1;
        x->height = max(getheight(x->left), getheight(x->right)) + 1;

        return x;
    }

    node* rotate_left(node* x) {
        node* y = x->right;
        node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getheight(x->left), getheight(x->right)) + 1;
        y->height = max(getheight(y->left), getheight(y->right)) + 1;

        return y;
    }

    node* insert(node* n, int playerid, int score) {
        if (!n)
            return new node(playerid, score);

        if (playerid < n->player_id)
            n->left = insert(n->left, playerid, score);
        else if (playerid > n->player_id)
            n->right = insert(n->right, playerid, score);
        else {
            cout << "Player already exists!\n";
            return n;
        }

        n->height = 1 + max(getheight(n->left), getheight(n->right));

        int balance = getbalance(n);

        if (balance > 1 && playerid < n->left->player_id)
            return rotate_right(n);

        if (balance < -1 && playerid > n->right->player_id)
            return rotate_left(n);

        if (balance > 1 && playerid > n->left->player_id) {
            n->left = rotate_left(n->left);
            return rotate_right(n);
        }

        if (balance < -1 && playerid < n->right->player_id) {
            n->right = rotate_right(n->right);
            return rotate_left(n);
        }

        return n;
    }

    node* findmin(node* n) {
        while (n->left)
            n = n->left;
        return n;
    }

    node* remove(node* root, int playerid) {
        if (!root)
            return root;

        if (playerid < root->player_id)
            root->left = remove(root->left, playerid);
        else if (playerid > root->player_id)
            root->right = remove(root->right, playerid);
        else {
            if (!root->left || !root->right) {
                node* temp = root->left ? root->left : root->right;
                delete root;
                return temp;
            }

            node* temp = findmin(root->right);
            root->player_id = temp->player_id;
            root->score = temp->score;
            root->right = remove(root->right, temp->player_id);
        }

        root->height = 1 + max(getheight(root->left), getheight(root->right));

        int balance = getbalance(root);

        if (balance > 1 && getbalance(root->left) >= 0)
            return rotate_right(root);

        if (balance > 1 && getbalance(root->left) < 0) {
            root->left = rotate_left(root->left);
            return rotate_right(root);
        }

        if (balance < -1 && getbalance(root->right) <= 0)
            return rotate_left(root);

        if (balance < -1 && getbalance(root->right) > 0) {
            root->right = rotate_right(root->right);
            return rotate_left(root);
        }

        return root;
    }

    void inorder(node* n) {
        if (n) {
            inorder(n->left);
            cout << "Player ID: " << n->player_id << " | Score: " << n->score << endl;
            inorder(n->right);
        }
    }

public:
    avltree() : root(nullptr) {}

    void insertplayer(int playerid, int score) {
        root = insert(root, playerid, score);
    }

    void removeplayer(int playerid) {
        root = remove(root, playerid);
    }

    void displayLeaderboard() {
        cout << "\n--- Leaderboard ---\n";
        inorder(root);
    }
};

int main() {
    avltree game;
    int choice, id, score;

    do {
        cout << "\n1. Insert Player\n2. Remove Player\n3. Display Leaderboard\n4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Player ID and Score: ";
                cin >> id >> score;
                game.insertplayer(id, score);
                break;
            case 2:
                cout << "Enter Player ID to remove: ";
                cin >> id;
                game.removeplayer(id);
                break;
            case 3:
                game.displayLeaderboard();
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
