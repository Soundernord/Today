#include <iostream>
#define INF 99999
#define MAX 100
using namespace std;

int graph[MAX][MAX];
int distance[MAX];
bool visited[MAX];
int locations;

void initializeGraph(int locs) {
    locations = locs;
    for (int i = 0; i < locations; i++) {
        for (int j = 0; j < locations; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addRoute(int src, int dest, int weight) {
    graph[src][dest] = weight; // directed graph
}

void dijkstra(int source) {
    for (int i = 0; i < locations; i++) {
        distance[i] = INF;
        visited[i] = false;
    }

    distance[source] = 0;

    for (int count = 0; count < locations - 1; count++) {
        int minDist = INF, u = -1;

        for (int i = 0; i < locations; i++) {
            if (!visited[i] && distance[i] < minDist) {
                minDist = distance[i];
                u = i;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        for (int v = 0; v < locations; v++) {
            if (!visited[v] && graph[u][v] != INF &&
                distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
            }
        }
    }

    cout << "Shortest distances from source location " << source << ":\n";
    for (int i = 0; i < locations; i++) {
        if (distance[i] == INF)
            cout << "Location " << i << " : Unreachable\n";
        else
            cout << "Location " << i << " : " << distance[i] << endl;
    }
}

int main() {
    int routes;
    cout << "Enter number of locations and routes: ";
    cin >> locations >> routes;

    initializeGraph(locations);

    cout << "Enter routes in the format (source destination distance):\n";
    for (int i = 0; i < routes; i++) {
        int src, dest, weight;
        cin >> src >> dest >> weight;
        addRoute(src, dest, weight);
    }

    int source;
    cout << "Enter source location: ";
    cin >> source;

    dijkstra(source);

    return 0;
}
