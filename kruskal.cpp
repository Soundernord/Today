#include <iostream>
using namespace std;

const int MAX = 100;
int parent[MAX];

int find(int i) {
    while (parent[i] != i)
        i = parent[i];
    return i;
}

void unionSets(int i, int j) {
    int a = find(i);
    int b = find(j);
    parent[a] = b;
}

void sortEdges(int edges[][3], int E) {
    for (int i = 0; i < E - 1; i++) {
        for (int j = 0; j < E - i - 1; j++) {
            if (edges[j][2] > edges[j + 1][2]) {
                for (int k = 0; k < 3; k++) {
                    int temp = edges[j][k];
                    edges[j][k] = edges[j + 1][k];
                    edges[j + 1][k] = temp;
                }
            }
        }
    }
}

void kruskalMST(int edges[][3], int V, int E) {
    for (int i = 0; i < V; i++)
        parent[i] = i;

    sortEdges(edges, E);

    cout << "\nMinimum Spanning Tree (Kruskal):\n";
    cout << "Departments Connected\tDistance\n";

    int count = 0;
    for (int i = 0; i < E && count < V - 1; i++) {
        int src = edges[i][0];
        int dest = edges[i][1];
        int weight = edges[i][2];

        if (find(src) != find(dest)) {
            unionSets(src, dest);
            cout << "Department " << src + 1 << " - Department " << dest + 1
                 << "\t\t" << weight << " meters\n";
            count++;
        }
    }
}

int main() {
    int V, E, edges[MAX][3];

    cin >> V >> E;

    for (int i = 0; i < E; i++)
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];

    kruskalMST(edges, V, E);

    return 0;
}
