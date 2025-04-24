#include<iostream>
using namespace std;

#define MAX 100

class queue {
    int front, rear;
    int arr[MAX];
public:
    queue() {
        front = rear = -1;
    }

    void enqueue(int val) {
        if(rear == MAX - 1) {
            cout << "Queue Overflow" << endl;
        } else {
            if(front == -1) front = 0;
            arr[++rear] = val;
        }
    }

    int dequeue() {
        if(front == -1 || front > rear) {
            cout << "Queue Underflow" << endl;
            return -1;
        } else {
            return arr[front++];
        }
    }

    bool isEmpty() {
        return (front == -1 || front > rear);
    }
};

class Graph {
    int adjMatrix[MAX][MAX];
    int visited[MAX];
    int v;

public:
    Graph(int vertices) {
        v = vertices;
        for(int i = 1; i <= v; i++) {
            visited[i] = 0;
            for(int j = 1; j <= v; j++) {
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
        for(int i = 1; i <=v; i++) {
            for(int j = 1; j <= v; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    void bfs(int start, int goal) {
        queue q;
        q.enqueue(start);
        visited[start] = 1;

        cout << "BFS Path: ";

        while(!q.isEmpty()) {
            int current = q.dequeue();
            cout << current << " ";

            if(current == goal) {
                cout << "\nGoal reached!" << endl;
                return;
            }

            for(int i = 1; i <= v; i++) {
                if(adjMatrix[current][i] == 1 && visited[i] == 0) {
                    q.enqueue(i);
                    visited[i] = 1;
                }
            }
        }

        cout << "\nNo path to the goal." << endl;
    }
};

int main() {
    int e,n, startpoint, goal;

    cout << "Enter number of vertices: ";
    cin >> n;
 cout << "Enter number of edges: ";
    cin >> e;

    Graph g(n);

    cout << "Enter edges (source destination):\n";
    for(int i = 0; i < e; i++) {
        int src, dest;
        cout << "Source: ";
        cin >> src;
        cout << "Destination: ";
        cin >> dest;
        g.addEdge(src, dest);
    }

    g.printAdjMatrix();

    cout << "Enter starting point for BFS: ";
    cin >> startpoint;
    cout << "Enter goal point: ";
    cin >> goal;

    g.bfs(startpoint, goal);

    return 0;
}
