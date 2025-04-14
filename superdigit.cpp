int* bfs(int n, int m, int edges_rows, int edges_columns, int** edges, int s, int* result_count) {
    int adjMatrix[n+1][n+1];
    for(int i=0; i<=n; i++) {
        for(int j=0; j<=n; j++) {
            adjMatrix[i][j] = 0;
        }
    }

    for(int i=0; i<m; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;
    }

    int distance[n+1];
    bool visited[n+1];
    for(int i=0; i<=n; i++) {
        distance[i] = -1;
        visited[i] = false;
    }

    int queue[n+1];
    int front = 0;
    int rear = 0;
    queue[rear] = s;
    rear++;
    distance[s] = 0;
    visited[s] = true;

    while(front != rear) {
        int current = queue[front];
        front++;

        for(int i=1; i<=n; i++) {
            if(adjMatrix[current][i] == 1 && visited[i] == false) {
                queue[rear] = i;
                rear++;
                distance[i] = distance[current] + 6;
                visited[i] = true;
            }
        }
    }

    *result_count = n-1;
    int* result = malloc((*result_count) * sizeof(int));
    int index = 0;
    for(int i=1; i<=n; i++) {
        if(i != s) {
            result[index] = distance[i];
            index++;
        }
    }

    return result;
}
