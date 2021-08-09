#include <stdio.h>
#include <string.h>

#define INF 4294967295

int graphReadBefore = 0;
int treeFull = 0;

struct listNode{
    int name;   //the name is based on how many graph are read before
    int valueOfThePath; //the sum of minimum path stating from 0 to every other node in graph
    //char color;
    //struct node *bigger;
    //struct node *smaller;
};

typedef struct listNode *node;

void takeGraph(int nodes);
void dijkstra(long int graph[][2], int source, int numberOfVertex);
long long int sumPaths(long int distancesFrom0[], int range);
//void addGraph(int totalSumOfPath, int graphName);
//void deleteGraph();
//void rimescolamento();
void TopK(int k);


int main(){

    int numberOfNodes, topGraph;
    char input[8];

    scanf("%d,%d", &numberOfNodes, &topGraph);
    printf("\n\nnumber of nodes: %d", numberOfNodes);
    printf("\n\ntop graph to print: %d\n\n", topGraph);

    //int arrayOfGraphs[topGraph]; //array in which there will put the graphs
    while(1){
        scanf("%s", input);
        if(0 == strcmp("AggiungiGrafo",input)) takeGraph(numberOfNodes);
        if(0 == strcmp("TopK",input)) TopK(topGraph);
    }
    return 0;
}

void takeGraph(int nodes){
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

    dijkstra(graphMatrix, 0, nodes);
}

void dijkstra(long int graph[][2], int source, int numberOfVertex){
    long int costMatrix[numberOfVertex][numberOfVertex];
    long int distance[numberOfVertex]; //distance[i] = shortest distance between source and i
    long int nodeVisited[numberOfVertex]; //nodeVisited[i] = 1 if i is included
    long int minimumDistance = INF;
    long int lastVisited;
    long int pred[numberOfVertex]; 

    //initialization of the cost matrix
    printf("\nInitialization of the cost matrix\n");
    for(int i = 0; i < numberOfVertex; i++){
        for(int j = 0; j < numberOfVertex; j++){
            if(0 == graph[i][j]){
                costMatrix[i][j] = INF;
            }
            else{
                costMatrix[i][j] = graph[i][j];
            }
            printf("[%ld]", costMatrix[i][j]);
        }
        printf("\n");
    }

    //initialization of distances and the list of node visited
    for(int i = 0; i < numberOfVertex; i++){
        distance[i] = costMatrix[source][i];
        if(i == source){
            distance[source] = 0; //the distance from the sorce and itself is always 0
        }
        nodeVisited[i] = 0;
        pred[i] = 0;
    }
    nodeVisited[source] = 1;

    printf("\ndistance array:\n");
    for(int i = 0; i < numberOfVertex; i++){
        printf("[%ld] ", distance[i]);
    }
    printf("\n");

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
                    pred[i] = lastVisited;
                }
            }
        }

    }

    for(int i = 0; i < numberOfVertex; i++){
        if(i != source){
            printf("The distance of node %d is: %ld\t", i, distance[i]);
            printf("The path is: %d", i);
            int j = i;
            while(j != source){
                j = pred[j];
                printf("<-%d", j);
            }
        }
        printf("\n");
    }
    long long int sumOfPath = sumPaths(distance, numberOfVertex);
    printf("Sum of paths starting from 0 is: %lld\n", sumOfPath);
    //addGraph(sumOfPath, graphReadBefore);
    //graphReadBefore++;
}

long long int sumPaths(long int distancesFrom0[], int range){
    int total = 0;
    for(int i = 0; i < range; i++){
        total = total + distancesFrom0[i]; 
    }
    return total;
}

/*void addGraph(int totalSumOfPath, int graphName){

    //adding the new graph in the tree
    
    //code

    //if the tree is full then the graph with the gratest path will be deleted 
    if(1 == treeFull){
        deleteGraph();
        rimescolamento();
    }
    
    return;
}*/

/*void deleteGraph(){
    return;
}*/

/*void rimescolamento(){
    return;
}
*/

void TopK(int k){

}