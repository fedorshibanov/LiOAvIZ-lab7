#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 20

typedef struct Node {
    int v;
    struct Node *next;
} Node;

int adj[MAX_N][MAX_N];
Node *list[MAX_N];
int visited[MAX_N];

void print_adj(int n) {
    printf("Adjacency matrix:\n    ");
    for (int j = 0; j < n; j++) printf("v%-2d ", j + 1);
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("v%-2d ", i + 1);
        for (int j = 0; j < n; j++) printf("%-3d ", adj[i][j]);
        printf("\n");
    }
}

void print_lists(int n) {
    printf("Adjacency lists:\n");
    for (int i = 0; i < n; i++) {
        printf("v%-2d: ", i + 1);
        for (Node *p = list[i]; p != NULL; p = p->next) printf("v%d -> ", p->v + 1);
        printf("NULL\n");
    }
}

void build_lists(int n) {
    for (int i = 0; i < n; i++) {
        Node **tail = &list[i];
        for (int j = 0; j < n; j++)
            if (adj[i][j]) {
                *tail = malloc(sizeof(Node));
                (*tail)->v = j;
                (*tail)->next = NULL;
                tail = &(*tail)->next;
            }
    }
}

void free_lists(int n) {
    for (int i = 0; i < n; i++) {
        Node *p = list[i];
        while (p != NULL) {
            Node *t = p;
            p = p->next;
            free(t);
        }
        list[i] = NULL;
    }
}

void reset_visited(int n) {
    for (int i = 0; i < n; i++) visited[i] = 0;
}

void dfs(int n, int v) {
    visited[v] = 1;
    printf("v%d ", v + 1);
    for (int i = 0; i < n; i++)
        if (adj[v][i] == 1 && !visited[i]) dfs(n, i);
}

void dfs_list(int v) {
    visited[v] = 1;
    printf("v%d ", v + 1);
    for (Node *p = list[v]; p != NULL; p = p->next)
        if (!visited[p->v]) dfs_list(p->v);
}

void dfs_stack(int n, int start) {
    int stack[MAX_N], pos[MAX_N] = {0};
    int top = 0;

    visited[start] = 1;
    printf("v%d ", start + 1);
    stack[top++] = start;

    while (top > 0) {
        int v = stack[top - 1];
        int i = pos[v];
        while (i < n && !(adj[v][i] == 1 && !visited[i])) i++;
        if (i < n) {
            pos[v] = i + 1;
            visited[i] = 1;
            printf("v%d ", i + 1);
            stack[top++] = i;
        } else {
            top--;
        }
    }
}

int main(void) {
    int n;

    printf("Enter number of vertices (1..%d): ", MAX_N);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        printf("Invalid input.\n");
        return 1;
    }
    srand((unsigned int)time(NULL));

    printf("\n=== Task 1 ===\n");

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            adj[i][j] = adj[j][i] = rand() % 2;
    print_adj(n);

    printf("DFS order: ");
    reset_visited(n);
    for (int v = 0; v < n; v++)
        if (!visited[v]) dfs(n, v);
    printf("\n");

    printf("\n=== Task 1.3* ===\n");

    build_lists(n);
    print_lists(n);

    printf("DFS order: ");
    reset_visited(n);
    for (int v = 0; v < n; v++)
        if (!visited[v]) dfs_list(v);
    printf("\n");

    printf("\n=== Task 2* ===\n");

    printf("DFS order: ");
    reset_visited(n);
    for (int v = 0; v < n; v++)
        if (!visited[v]) dfs_stack(n, v);
    printf("\n");

    free_lists(n);
    return 0;
}