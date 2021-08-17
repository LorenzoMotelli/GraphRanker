#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define INF 4294967295
#define RED 'r'
#define BLACK 'b'

int graphReadBefore = 0;
int insertions = 0;

typedef struct listNode{
    int name;   //the name is based on how many graph are read before
    long long int valueOfThePath; //the sum of minimum path stating from 0 to every other node in graph
    char color;
    struct listNode *parent;
    struct listNode *right;
    struct listNode *left;
}node_t;

long long int takeGraph(long int nodes);
long long int dijkstra(long int source, long int numberOfVertex, long int graph[numberOfVertex][numberOfVertex]);
long long int sumPaths(long int distancesFrom0[], int range);
node_t *addGraph(node_t **root, node_t *newNode);
void insertFixup(node_t **root, node_t *node);
void deleteGraph(node_t **root);
void deleteFixup(node_t **root, node_t *node);
void leftRotate(node_t **root,node_t *node);
void rightRotate(node_t **root,node_t *node);
void TopK(node_t *root);


int main(){
    long int numberOfNodes, topGraph;
    char input[8];
    node_t *treeHead = NULL;
    node_t *nodeToAdd = malloc(sizeof(node_t));

    scanf("%ld,%ld", &numberOfNodes, &topGraph);

    while(1){
        scanf("%s", input);
        if(0 == strcmp("AggiungiGrafo",input)){
            long long int sum = takeGraph(numberOfNodes);
            nodeToAdd->name = graphReadBefore;
            nodeToAdd->valueOfThePath = sum;
            nodeToAdd->color = RED;
            nodeToAdd->left = NULL;
            nodeToAdd->right = NULL;
            nodeToAdd->parent = NULL;
            addGraph(&treeHead, nodeToAdd);
            graphReadBefore++;
            insertions++;
            printf("Graph added\n");
            insertFixup(&treeHead, nodeToAdd);
            if(insertions > topGraph){
                printf("I'm going to delete a graph because we reached the max numebr of graphs required\n");
                deleteGraph(&treeHead);
            }
        }
        if(0 == strcmp("TopK",input)) TopK(treeHead);
    }
    free(treeHead);
    free(nodeToAdd);
    return 0;
}

/**
 * take the graph matrix and invoke the dijkstra algorithm to find the minimum path from the source and the other nodes
 * return the value of the paths' sum
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

/**
 * dijksta algorithm
 * return the value of the minimum distance between the source 0 and the other nodes in the graph
*/
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

/**
 * function to sum the paths
*/
long long int sumPaths(long int distancesFrom0[], int range){
    int total = 0;
    for(int i = 0; i < range; i++){
        total = total + distancesFrom0[i]; 
    }
    return total;
}

/**
 * add the graph given by the input
*/
node_t *addGraph(node_t **root, node_t *newNode){
    printf("I'm in addGraph\n");
    if(NULL == (*root)){
        printf("Root is NULL\n");
        (*root) = newNode;
        return (*root);
    }
    if(newNode->valueOfThePath < (*root)->valueOfThePath){
        printf("Go left\n");
        (*root)->left = addGraph(&(*root)->left, newNode);
        (*root)->left->parent = (*root);
        printf("New node parent:%d\n", (*root)->parent->name);
    }
    else{
        printf("Go right\n");
        (*root)->right = addGraph(&(*root)->right, newNode);
        (*root)->right->parent = (*root);
        printf("New node parent:%d\n", (*root)->parent->name);
    }
    return (*root);
}

/**
 * function to balance the tree after an insertion of a node
*/
void insertFixup(node_t **root, node_t *node){
    printf("I'm in the insert fixup function\n");
    while(node != (*root) && node->parent->color == RED){
        printf("I'm in the while loop\n");
        node_t *x = node->parent;
        if(RED == x->color){
            printf("X is red\n");
            if(x == x->parent->left){
                printf("Initilization of y\n");
                node_t *y = x->parent->right;
                if(RED == y->color){
                    printf("Change colors\n");
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    node = x->parent;
                }
                else{
                    printf("Need a left rotation\n");
                    if(node == x->right){
                        node = x;
                        leftRotate(root, node);
                        x = node->parent;
                    }
                    printf("Change color and do a right rotation\n");
                    x->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(root, x->parent);
                }
            }
        }
        else{
            printf("Initialization of y\n");
            node_t *y = x->parent->left;
            if(RED == y->color){
                printf("Y was red, so change of colors\n");
                x->color = BLACK;
                y->color = BLACK;
                x->parent->color = RED;
                node = x->parent;
            }
            else{
                printf("Y is black\n");
                if(node == x->left){
                    printf("Need a right rotation\n");
                    node = x;
                    rightRotate(root, node);
                    x = node->parent;
                }
                printf("Change colors and do a left rotation\n");
                x->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
            }
        }
    }
    printf("The root color is now black\n");
    (*root)->color = BLACK;
}

/**
 * delete the node (graph) with the maximum path distance
 * (the node at the most right in the tree)
*/
void deleteGraph(node_t **root){
    node_t *currentNode = malloc(sizeof(node_t));
    node_t *x = NULL;

    printf("I'm in the fuction deleteGraph\n");
    currentNode = *root;
    printf("currentNode initialized\n");
    while(NULL != currentNode->right && NULL != currentNode->left){
        printf("I'm searching the node to delete:\n");
        if(currentNode->right->valueOfThePath >= currentNode->valueOfThePath){
            printf("The greater graph is currentNode->right\n");
            currentNode = currentNode->right;
        }
        else{
            printf("The greater graph is currentNode->right\n");
            currentNode = currentNode->left;
        }
    }
    x = currentNode->parent;
    free(currentNode);
    deleteFixup(root, x);

    return;
}

/**
 * function to balance the tree after an erasure of a node
*/
void deleteFixup(node_t **root, node_t *node){
    while (node != *root && node->color == BLACK){
		if (node == node->parent->left){
			node_t *x = node->parent->right;
			if (x->color == RED){
				x->color = BLACK;
				node->parent->color = RED;
				leftRotate(root, node->parent);
				x = node->parent->right;
			}
			if (x->left->color == BLACK && x->right->color == BLACK){
				x->color = RED;
				node = node->parent;
			}
			else{
				if (x->right->color == BLACK){
					x->left->color = BLACK;
					x->color = RED;
					rightRotate(root, x);
					x = node->parent->right;
				}
				x->color = node->parent->color;
				node->parent->color = BLACK;
				x->right->color = BLACK;
				leftRotate(root, node->parent);
				node = *root;
			}
		}
		else{
			node_t *x = node->parent->left;
			if (x->color == RED){
				x->color = BLACK;
				node->parent->color = RED;
				rightRotate(root, node->parent);
				x = node->parent->left;
			}
			if (x->right->color == BLACK && x->left->color == BLACK){
				x->color = RED;
				node = node->parent;
			}
			else{
				if (x->left->color == BLACK){
					x->right->color = BLACK;
					x->color = RED;
					leftRotate(root, x);
					x = node->parent->left;
				}
				x->color = node->parent->color;
				node->parent->color = BLACK;
				x->left->color = BLACK;
				rightRotate(root, node->parent);
				node = *root;
			}
		}
	}
	node->color = BLACK;
    return;
}

/**
 * Right and Left rotation of the nodes in the tree to balance it after an insertion or an erasure
*/

void leftRotate(node_t **root, node_t *node){
    node_t *rightNode = node->right;
    node->right = rightNode->left;
    if(NULL != rightNode){
        rightNode->right->parent = rightNode;
    }
    rightNode->parent = node->parent;
    if(NULL == node->parent){
        *root = rightNode;
    }
    else if(node == node->parent->right){
        node->parent->right = rightNode;
    }
    else{
        node->parent->left = rightNode;
    }
    rightNode->left = node;
    rightNode->right = rightNode;
}

void rightRotate(node_t **root, node_t *node){
    node_t *leftNode = node->left;
    node->left = leftNode->right;
    if(NULL != leftNode){
        leftNode->left->parent = leftNode;
    }
    leftNode->parent = node->parent;
    if(NULL == node->parent){
        *root = leftNode;
    }
    else if(node == node->parent->left){
        node->parent->left = leftNode;
    }
    else{
        node->parent->right = leftNode;
    }
    leftNode->right = node;
    leftNode->left = leftNode;
}

/**
 * inorder print names of the graphs
*/
void TopK(node_t *root){
    printf("I'm in TopK\n");
    if(NULL == root){
        printf("The root is NULL, exiting the function\n");
        return;
    }
    TopK(root->left);
    printf("%d ", root->name);
    TopK(root->right);
}