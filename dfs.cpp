#include <iostream>
using namespace std;

#define MAX 100

class stack {
    int top;
    int arr[MAX];
public:
    stack() {
        top = -1;
    }

    void push(int val) {
        if (top == MAX - 1) {
            cout << "Stack Overflow" << endl;
        } else {
            arr[++top] = val;
        }
    }

    int pop() {
        if (top == -1) {
            cout << "Stack Underflow" << endl;
            return -1;
        } else {
            return arr[top--];
        }
    }

    bool isEmpty() {
        return (top == -1);
    }
};

class Graph {
    int adjMatrix[MAX][MAX];
    int visited[MAX];
    int v;

public:
    Graph(int vertices) {
        v = vertices;
        for (int i = 1; i <= v; i++) {
            visited[i] = 0;
            for (int j = 1; j <= v; j++) {
                adjMatrix[i][j] = 0;
            }
        }
    }

    void addEdge(int src, int dest) {
        adjMatrix[src][dest] = 1;
        adjMatrix[dest][src] = 1;
    }

    void printAdjMatrix() {
        cout << "Adjacency Matrix:\n";
        for (int i = 1; i <= v; i++) {
            for (int j = 1; j <= v; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    void dfs(int start, int goal) {
        stack s;
        s.push(start);
        visited[start] = 1;

        cout << "DFS Path: ";

        while (!s.isEmpty()) {
            int current = s.pop();
            cout << current << " ";

            if (current == goal) {
                cout << "\nGoal reached!" << endl;
                return;
            }


            for (int i = v; i >= 1; i--) {
                if (adjMatrix[current][i] == 1 && visited[i] == 0) {
                    s.push(i);
                    visited[i] = 1;
                }
            }
        }

        cout << "\nNo path to the goal." << endl;
    }
};

int main() {
    int e, n, startpoint, goal;

    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> e;

    Graph g(n);

    cout << "Enter edges (source destination):\n";
    for (int i = 0; i < e; i++) {
        int src, dest;
        cout << "Source: ";
        cin >> src;
        cout << "Destination: ";
        cin >> dest;
        g.addEdge(src, dest);
    }

    g.printAdjMatrix();

    cout << "Enter starting point for DFS: ";
    cin >> startpoint;
    cout << "Enter goal point: ";
    cin >> goal;

    g.dfs(startpoint, goal);

    return 0;
}
