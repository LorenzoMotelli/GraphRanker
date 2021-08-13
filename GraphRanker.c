#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define INF 4294967295

int graphReadBefore = 0;
int insertions = 0;
int width = 0;

struct listNode{
    int name;   //the name is based on how many graph are read before
    long long int valueOfThePath; //the sum of minimum path stating from 0 to every other node in graph
    //char color;
    struct listNode *bigger;
    struct listNode *smaller;
};

typedef struct listNode *node;

long long int takeGraph(long int nodes);
long long int dijkstra(long int source, long int numberOfVertex, long int graph[numberOfVertex][numberOfVertex]);
long long int sumPaths(long int distancesFrom0[], int range);
void addGraph(long long int totalSumOfPath, int graphName, int k, struct listNode* headOfTheTree);
int printTree(struct listNode* root, int isLeft, int offset, int depth, char s[20][255]);
void printTreeSons(struct listNode* nodeSx, struct listNode* nodeDx);
void deleteGraph(struct listNode* root);
void remixing(struct listNode* root);
void TopK(int k, struct listNode* root);


int main(){

    long int numberOfNodes, topGraph;
    char input[8];
    struct listNode* treeHead = NULL;

    scanf("%ld,%ld", &numberOfNodes, &topGraph);

    //int arrayOfGraphs[topGraph]; //array in which there will put the graphs
    while(1){
        scanf("%s", input);
        if(0 == strcmp("AggiungiGrafo",input)){
            long long int sum = takeGraph(numberOfNodes);
            addGraph(sum, graphReadBefore, topGraph, treeHead);
            if(insertions > numberOfNodes){
                deleteGraph(treeHead);
                remixing(treeHead);
            }
        }
        if(0 == strcmp("TopK",input)) TopK(topGraph, treeHead);
    }
    return 0;
}

/*
* take the graph matrix and invoke the dijkstra algorithm to find the minimum path from the source and the other nodes
* return lon long int : the value of the path sum
*/
long long int takeGraph(long int nodes){
    long int graphMatrix[nodes][nodes];
    int lines = nodes;
    char inputDist[2*nodes-1];
    int i = 0, j = 0, k = 0;

    while(lines != 0){
        scanf("%s", inputDist);
        for(k = 0; k < 2*nodes-1; k++){
            if(',' != inputDist[k]){
                graphMatrix[i][j] = inputDist[k]-'0';
                j++;
                if(j == nodes){
                    i++;
                    j = 0;
                }
            }
        }
        lines--;
    }

    printf("The Graph matrix:\n\n");
    for(i = 0; i < nodes; i++){
        for(j = 0; j < nodes; j++){
            printf("[%ld] ", graphMatrix[i][j]);
        }
        printf("\n");
    }

    return dijkstra(0, nodes, graphMatrix);
}

long long int dijkstra(long int source, long int numberOfVertex, long int graph[numberOfVertex][numberOfVertex]){
    long int costMatrix[numberOfVertex][numberOfVertex];
    long int distance[numberOfVertex]; //distance[i] = shortest distance between source and i
    long int nodeVisited[numberOfVertex]; //nodeVisited[i] = 1 if i is included
    long int minimumDistance = INF;
    long int lastVisited;

    //initialization of the cost matrix
    for(int i = 0; i < numberOfVertex; i++){
        for(int j = 0; j < numberOfVertex; j++){
            if(0 == graph[i][j]){
                costMatrix[i][j] = INF;
            }
            else{
                costMatrix[i][j] = graph[i][j];
            }
        }
    }

    //initialization of distances and the list of node visited
    for(int i = 0; i < numberOfVertex; i++){
        distance[i] = costMatrix[source][i];
        if(i == source){
            distance[source] = 0; //the distance from the sorce and itself is always 0
        }
        nodeVisited[i] = 0;
    }
    nodeVisited[source] = 1;

    for(int count = 0; count < numberOfVertex; count++){
        minimumDistance = INF;
        for(int i = 0; i < numberOfVertex; i++){
            if(distance[i] < minimumDistance && !nodeVisited[i]){
                minimumDistance = distance[i];
                lastVisited = i;
            }
        }
        nodeVisited[lastVisited] = 1;
        for(int i = 0; i < numberOfVertex; i++){
            if(!nodeVisited[i]){
                if((minimumDistance + costMatrix[lastVisited][i]) < distance[i]){
                    distance[i] = minimumDistance + costMatrix[lastVisited][i];
                }
            }
        }

    }
    long long int sumOfPath = sumPaths(distance, numberOfVertex);
    return sumOfPath;
}

long long int sumPaths(long int distancesFrom0[], int range){
    int total = 0;
    for(int i = 0; i < range; i++){
        total = total + distancesFrom0[i]; 
    }
    return total;
}

void addGraph(long long int totalSumOfPath, int graphName, int k, struct listNode* headOfTheTree){
    printf("The graphs since then are: %d\n", graphName); 
    //adding the new graph in the tree
    if(0 == insertions){
        printf("Insertion of the first graph:\n");
        headOfTheTree = malloc(sizeof(struct listNode));
        headOfTheTree->name = graphName;
        headOfTheTree->valueOfThePath = totalSumOfPath;
        headOfTheTree->bigger = NULL;
        headOfTheTree->smaller = NULL;
        printf("Root initalized\n");
    }

    else{
        printf("Insertion of the %d graph\n", graphName);
        struct listNode* node = malloc(sizeof(struct listNode));
        struct listNode* currentNode = malloc(sizeof(struct listNode));

        node->name = graphName;
        node->valueOfThePath = totalSumOfPath;
        node->bigger = NULL;
        node->smaller = NULL;

        currentNode  = headOfTheTree;
        while(currentNode != NULL){
            if(node->valueOfThePath < currentNode->valueOfThePath){
                currentNode = currentNode->smaller;
            }
            else{
                currentNode = currentNode->bigger;
            }
        }
        currentNode->name = graphName;
        currentNode->valueOfThePath = node->valueOfThePath;
        currentNode->bigger = NULL;
        currentNode->smaller = NULL;
    }

    graphReadBefore++;
    insertions++;
    width++;
    
    return;
}

void deleteGraph(struct listNode* root){
    struct listNode* currentNode = malloc(sizeof(struct listNode));
    currentNode = root;
    while(NULL != currentNode->bigger){
        currentNode = currentNode->bigger;
    }
    if(NULL != currentNode->smaller){
        currentNode = currentNode->smaller;
    }
    free(currentNode);
    return;
}

void remixing(struct listNode* root){
    return;
}

//inorder print
void TopK(int k, struct listNode* root){
    if(NULL != root){
        TopK(k, root->smaller);
        printf("%d", root->name);
        TopK(k, root->bigger);
    }
}