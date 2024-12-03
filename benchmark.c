#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#define N 1000
#define M 1000
#define P 1000
#define S 1000
#define INITIAL_SIZE 4

int **A1;
int **A2;
int *A3;
int A[N][M], B[N][P], C[P][M];

enum couleur {
    Blanc,
    Gris,
    Noir,
};

struct graph {
    int nVertex;
    int* sizes;
    int* capacities;
    int** listAdj;
    int** weights;
};

struct queue {
    int* vertices;
    int len;
};

struct queue* createQueue(int size) {
    struct queue* q = malloc(sizeof(struct queue));
    q->vertices = malloc(size * sizeof(int));
    q->len = 0;
    return q;
}

bool isEmpty(struct queue* q) {
    return q->len == 0;
}

void addToQueue(struct queue* q, int vertex) {
    q->vertices[q->len++] = vertex;
}

int removeFromQueue(struct queue* q) {
    if (isEmpty(q)) {
        return -1;
    }
    return q->vertices[--q->len];
}

struct graph* createGraph(int nVertex) {
    struct graph* g = malloc(sizeof(struct graph));
    g->nVertex = nVertex;
    g->listAdj = malloc(nVertex * sizeof(int*));
    g->sizes = malloc(nVertex * sizeof(int));
    g->capacities = malloc(nVertex * sizeof(int*));
    g->weights = malloc(nVertex * sizeof(int*));

    for (int i = 0; i < nVertex; i++) {
        g->listAdj[i] = malloc(INITIAL_SIZE * sizeof(int));
        g->weights[i] = malloc(INITIAL_SIZE * sizeof(int));
        g->sizes[i] = 0;
        g->capacities[i] = INITIAL_SIZE;
    }
    return g;
}

void addEdge(struct graph* g, int src, int dest, int weight) {
    if (g->sizes[src] >= g->capacities[src]) {
        g->capacities[src] *= 2;
        g->listAdj[src] = realloc(g->listAdj[src], g->capacities[src] * sizeof(int));
        g->weights[src] = realloc(g->weights[src], g->capacities[src] * sizeof(int));
    }
    g->listAdj[src][g->sizes[src]] = dest;
    g->weights[src][g->sizes[src]] = weight;
    g->sizes[src]++;

    if (g->sizes[dest] >= g->capacities[dest]) {
        g->capacities[dest] *= 2;
        g->listAdj[dest] = realloc(g->listAdj[dest], g->capacities[dest] * sizeof(int));
        g->weights[dest] = realloc(g->weights[dest], g->capacities[dest] * sizeof(int));
    }
    g->listAdj[dest][g->sizes[dest]] = src;
    g->weights[dest][g->sizes[dest]] = weight;
    g->sizes[dest]++;
}

void generateRandomGraph(struct graph* g, int maxEdges) {
    srand(time(NULL));
    int edgesCount = 0;
    while (edgesCount < maxEdges) {
        int src = rand() % g->nVertex;
        int dest = rand() % g->nVertex;
        if (src != dest) {
            bool exists = false;
            for (int i = 0; i < g->sizes[src]; i++) {
                if (g->listAdj[src][i] == dest) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                int weight = rand() % 10 + 1;
                addEdge(g, src, dest, weight);
                edgesCount++;
            }
        }
    }
}

void bfs(struct graph* g, int source) {
    struct queue* q = createQueue(g->nVertex);
    enum couleur* couleurs = malloc(g->nVertex * sizeof(enum couleur));

    for (int i = 0; i < g->nVertex; i++) {
        couleurs[i] = Blanc;
    }

    addToQueue(q, source);
    couleurs[source] = Gris;
    //printf("Ordre de découverte (BFS): ");

    while (!isEmpty(q)) {
        int u = removeFromQueue(q);
        //printf("%d ", u);

        for (int i = 0; i < g->sizes[u]; i++) {
            int v = g->listAdj[u][i];
            if (couleurs[v] == Blanc) {
                addToQueue(q, v);
                couleurs[v] = Gris;
            }
        }
        couleurs[u] = Noir;
    }
    //printf("\n");
    free(couleurs);
    free(q->vertices);
    free(q);
}

void dijkstra(struct graph* g, int source) {
    int* dist = malloc(g->nVertex * sizeof(int));
    bool* visited = malloc(g->nVertex * sizeof(bool));
    struct queue* q = createQueue(g->nVertex);

    for (int i = 0; i < g->nVertex; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    dist[source] = 0;
    addToQueue(q, source);

    while (!isEmpty(q)) {
        int u = removeFromQueue(q);
        if (visited[u]) continue;
        visited[u] = true;

        for (int i = 0; i < g->sizes[u]; i++) {
            int v = g->listAdj[u][i];
            int weight = g->weights[u][i];
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                addToQueue(q, v);
            }
        }
    }
    /*printf("Source = %d:\n", source);
    for (int i = 0; i < g->nVertex; i++) {
        printf("Sommet %d: %d\n", i, dist[i]);
    }*/
    free(dist);
    free(visited);
    free(q->vertices);
    free(q);
}

void freeGraph(struct graph* g) {
    for (int i = 0; i < g->nVertex; i++) {
        free(g->listAdj[i]);
        free(g->weights[i]);
    }
    free(g->listAdj);
    free(g->weights);
    free(g->sizes);
    free(g->capacities);
    free(g);
}

//--------------------------------------------------------------------------
void initialiserMatrices(){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i][j] = 1;
            B[i][j] = 2;
        }
    }
}

void multMatrice(){
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            for(int k=0; k<P; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multScalaire(){
	for(int i=0;i<N;i++){
		A3[i]=A3[i]*S;
	}
}

void add(){
    for (int i=0; i<N; i++){
        for (int j=0; j<M; j++){
            A1[i][j] += A2[i][j];
        }
    }
}

void createWandRFile(int nfichier){
	for(int i=1;i<=nfichier;i++){
		char name[50];
		sprintf(name, "test_fichier%d.txt",i);
		FILE *fp = fopen(name, "w");
		if (fp == NULL) {
		    perror("Error opening file");
		    return;
		}
		for (int i=0; i<1000; i++){
        fprintf(fp, "This is line %d of the file", i);
    	}
    	fclose(fp);
    }
    
	for(int i=1;i<=nfichier;i++){
		char name[50];
		sprintf(name, "test_fichier%d.txt",i);
		FILE *fp = fopen(name, "r");
		if (fp == NULL){
		    perror("Error opening file");
		    return;
		}
		char *buffer = (char *) malloc(sizeof(char *));
		while (!feof(fp)){
			fgets(buffer, sizeof(char *),fp);
		}
		free(buffer);
		fclose(fp);
	}
}

void removeFile(int nfichier){
	for(int i=1;i<=nfichier;i++){
		char name[50];
		sprintf(name, "test_fichier%d.txt",i);
		remove(name);
	}
}

int main(){
	srand(time(NULL));
    A1 = malloc(N * sizeof(int *));
    A2 = malloc(N * sizeof(int *));
	A3 = malloc(N * sizeof(int));
    for(int i=0; i<N; i++){
        A1[i] = malloc(sizeof(int) * N);
        A2[i] = malloc(sizeof(int) * N);
    }
    
	initialiserMatrices();
	int nfichier = 10000;
	int n = 100000;
	int narete=n*20;
	
	int appels = 100;
	for(int i=0;i<appels;i++){
		int aleatoire = rand()%5;
		switch(aleatoire){
			case 0:
				multMatrice();
				break;
			case 1:
				add();
				break;
			case 2:
				createWandRFile(nfichier);
    			removeFile(nfichier);
    			break;
			case 3:
				struct graph* g1 = createGraph(n);
				struct graph* g2 = createGraph(n);
				generateRandomGraph(g1, narete);
				generateRandomGraph(g2, narete);
				bfs(g1, 0);
				dijkstra(g2,0);
				freeGraph(g1);
				freeGraph(g2);
				break;
			case 4:
				multScalaire();
				break;
		}
	}
	/*
	createWandRFile(nfichier);
	removeFile(nfichier);
	createWandRFile(nfichier);
	removeFile(nfichier);
	multMatrice();
	//pour tester seulement les graphes
	struct graph* g1 = createGraph(n);
	struct graph* g2 = createGraph(n);
	generateRandomGraph(g1, narete);
	generateRandomGraph(g2, narete);
	bfs(g1, 0);
	dijkstra(g2,0);
	freeGraph(g1);
	freeGraph(g2);
	removeFile(nfichier);
	*/
    for(int i=0; i<N; i++){
        free(A1[i]);
        free(A2[i]);
    }
    
    free(A1);
    free(A2);
	free(A3);
    return 0;
}
