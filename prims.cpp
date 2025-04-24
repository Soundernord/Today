#include <iostream>
using namespace std;

int minKey(int key[], bool mstSet[], int V) {
    int min = 99999, index = -1;
    for (int i = 0; i < V; i++) {
        if (!mstSet[i] && key[i] < min) {
            min = key[i];
            index = i;
        }
    }
    return index;
}

void printMST(int parent[], int graph[100][100], int V) {
    cout << "\nMinimum Spanning Tree:\n";
    cout << "From - To\tDistance\n";
    for (int i = 1; i < V; i++) {
        cout << parent[i] + 1 << " - " << i + 1 << "\t" << graph[i][parent[i]] << endl;
    }
}

void primMST(int graph[100][100], int V) {
    int key[100], parent[100];
    bool mstSet[100];

    for (int i = 0; i < V; i++) {
        key[i] = 99999;
        mstSet[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int i = 0; i < V - 1; i++) {
        int u = minKey(key, mstSet, V);
        mstSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    printMST(parent, graph, V);
}

int main() {
    int V, graph[100][100];
    cout << "Enter number of departments: ";
    cin >> V;

    cout << "Enter adjacency matrix:\n";
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            cin >> graph[i][j];

    primMST(graph, V);
    return 0;
}
