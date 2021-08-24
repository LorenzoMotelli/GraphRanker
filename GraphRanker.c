#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INF 4294967295
#define MAX 5000
#define RED 'r'
#define BLACK 'b'

int graphReadBefore = 0;
int insertions = 0;
long int numberOfNodes, topGraph;
char row[MAX];

typedef struct listNode{
    int name;   //the name is based on how many graph are read before
    long long int valueOfThePath; //the sum of minimum path stating from 0 to every other node in graph
    char color;
    struct listNode *parent;
    struct listNode *right;
    struct listNode *left;
}node_t;

#define NIL (&nil)
static node_t nil = { -1, -1, BLACK, &nil, &nil, &nil };

long long int takeGraph(long int nodes);
long long int dijkstra(long int source, long int graph[numberOfNodes][numberOfNodes]);
long long int sumPaths(const long int distancesFrom0[]);
void addGraph(node_t **root, long long int pathDistance);
void insertFixup(node_t **root, node_t *z);
void deleteGraph(node_t **root, node_t *z);
node_t *treeSuccessor(node_t *x);
node_t *treeMinimum(node_t *x);
void deleteFixup(node_t **root, node_t *x);
void leftRotate(node_t **root,node_t *x);
void rightRotate(node_t **root,node_t *x);
void TopK(node_t **root, int k);


int main(){
    char input[MAX];
    char converter[MAX];
    char *tmp;
    int i, k = 0 ;
    node_t *root = NIL;

    tmp = fgets(input, MAX, stdin);
    for(i = 0; i < MAX; i++){
        if(' ' == input[i]){
            k = i+1;
            break;
        }
        converter[i] = input[i];
    }
    i = 0;
    numberOfNodes = strtol(converter, &tmp, 0);
    while('\0' != input[k]){
        converter[i] = input[k];
        i++;
        k++;
    }
    topGraph = strtol(converter, &tmp, 0);
    while(NULL != fgets(input, MAX, stdin)){
        if(0 == strncmp("A", input, 1)){
            //printf("\n\nCHECKING THE GRAPH: %d\n", graphReadBefore);
            long long int sum = takeGraph(numberOfNodes);
            addGraph(&root, sum);
            if(insertions >= topGraph){
                node_t *nodeToDelete = root;
                while(NIL != nodeToDelete->right || NIL != nodeToDelete->left){
                    if(nodeToDelete->right->valueOfThePath > nodeToDelete->valueOfThePath){
                        nodeToDelete = nodeToDelete->right;
                    }
                    else if(nodeToDelete->left->valueOfThePath > nodeToDelete->valueOfThePath){
                        nodeToDelete = nodeToDelete->left;
                    }
                    else{
                        break;
                    }
                }
                deleteGraph(&root, nodeToDelete);
            }
            graphReadBefore++;
            insertions++;
        }
        if(0 == strncmp("T", input, 1)){
            TopK(&root, topGraph);
            printf("\n");
        }
    }
    //printf("\n");
    return 0;
}

/**
 * take the graph matrix and invoke the dijkstra algorithm to find the minimum path from the source and the other nodes
 * return the value of the paths' sum
*/
long long int takeGraph(long int nodes){
    long int graphMatrix[nodes][nodes];
    char *inputDist;
    int i, j, best = 1;
    for(i = 0; i < nodes; i++){
        inputDist = fgets(row, MAX, stdin);
        for(j = 0; j < nodes; j++){
            graphMatrix[i][j] = strtol(inputDist, &inputDist, 10);
            inputDist++;
        }
    }
    for(int k = 1; k < nodes; k++){
        if(0 != graphMatrix[0][k]){
            best = 0;
        }
    }
    if(1 == best) {
        //printf("The minimum path of this graph is:0\n");
        return 0;
    }
    return dijkstra(0, graphMatrix);
}

/**
 * Dijkstra algorithm
 * return the value of the minimum distance between the source 0 and the other nodes in the graph
*/
long long int dijkstra(long int source, long int graph[numberOfNodes][numberOfNodes]){
    long int costMatrix[numberOfNodes][numberOfNodes];
    long int distance[numberOfNodes]; //distance[i] = the shortest distance between source and i
    long int nodeVisited[numberOfNodes]; //nodeVisited[i] = 1 if the graph(#i) is included
    long int minimumDistance = INF;
    long int lastVisited = 0;

    //initialization of the cost matrix
    for(int i = 0; i < numberOfNodes; i++){
        for(int j = 0; j < numberOfNodes; j++){
            if(0 == graph[i][j]){
                costMatrix[i][j] = INF;
            }
            else{
                costMatrix[i][j] = graph[i][j];
            }
        }
    }

    //initialization of distances and the list of node visited
    for(int i = 0; i < numberOfNodes; i++){
        distance[i] = costMatrix[source][i];
        if(i == source){
            distance[source] = 0; //the distance from the source and itself is always 0
        }
        nodeVisited[i] = 0;
    }
    nodeVisited[source] = 1;

    for(int count = 0; count < numberOfNodes; count++){
        minimumDistance = INF;
        for(int i = 0; i < numberOfNodes; i++){
            if(distance[i] < minimumDistance && !nodeVisited[i]){
                minimumDistance = distance[i];
                lastVisited = i;
            }
        }
        nodeVisited[lastVisited] = 1;
        for(int i = 0; i < numberOfNodes; i++){
            if(!nodeVisited[i]){
                if((minimumDistance + costMatrix[lastVisited][i]) < distance[i]){
                    distance[i] = minimumDistance + costMatrix[lastVisited][i];
                }
            }
        }
    }
    for(int i = 0; i < numberOfNodes; i++){
        if(!nodeVisited[i]){
            distance[i] = 0;
        }
    }
    long long int sumOfPath = sumPaths(distance);
    return sumOfPath;
}

/**
 * function to sum the paths
*/
long long int sumPaths(const long int distancesFrom0[]){
    long long int total = 0;
    for(int i = 1; i < numberOfNodes; i++){
        total = total + distancesFrom0[i];
    }
    //printf("The minimum path of this graph is:%lld\n", total);
    return total;
}

/**
 * add the graph given by the input
*/
void addGraph(node_t **root, long long int pathDistance){
    //printf("addGraph started\n");
    node_t *y = NIL;
    node_t *x = *root;
    //fin the last node useful
    while(x != NIL){
        y = x;
        if(pathDistance < x->valueOfThePath){
            //printf("Go left\n");
            x = x->left;
        }
        else{
            //printf("Go right\n");
            x = x->right;
        }
    }
    node_t *newNode = malloc((sizeof(node_t)));
    newNode->name = graphReadBefore;
    newNode->valueOfThePath = pathDistance;
    newNode->parent = y;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->color = RED;
    if(NIL == y){
        //printf("Y is NIL so it's the first graph to be added\n");
        *root = newNode;
    }
    else{
        if(newNode->valueOfThePath < y->valueOfThePath){
            //printf("The new graph is smaller, go left\n");
            y->left = newNode;
        }
        else{
            //printf("The new graph is bigger, go right\n");
            y->right = newNode;
        }
    }
    insertFixup(root, newNode);
    //printf("The root is the graph %d\n", (*root)->name);
}

/**
 * function to balance the tree after an insertion of a node
*/
void insertFixup(node_t **root, node_t *z){
    //printf("insterFixup started\n");
    while (z != (*root) && z->parent->color == RED){
        node_t *x = z->parent;
        if (x->color == RED){
            //printf("The parent is red\n");
            if (x == x->parent->left){
                //printf("The parent is a left child\n");
                node_t *y = x->parent->right;
                if (y->color == RED){
                    //printf("The uncle is red, switch colors and the hew node became it's grand parent\n");
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    z = x->parent;
                }
                else{
                    //printf("The uncle is black\n");
                    if (z == x->right){
                        //printf("The new node is a right child, need a left rotation\n");
                        z = x;
                        leftRotate(root, z);
                        x = z->parent;
                    }
                    //printf("switch colors and right rotation\n");
                    x->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(root, x->parent);
                }
            }
            else{
                //printf("The parent is a right child\n");
                node_t *y = x->parent->left;
                if (y->color == RED){
                    //printf("The uncle is red, switch colors and the hew node became it's grand parent\n");
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    z = x->parent;
                }
                else{
                    //printf("The uncle is black\n");
                    if (z == x->left){
                        //printf("The new node is a left child, need a right rotation\n");
                        z = x;
                        rightRotate(root, z);
                        x = z->parent;
                    }
                    //printf("switch colors and left rotation\n");
                    x->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(root, x->parent);
                }
            }
        }
    }
    (*root)->color = BLACK;
}

/**
 * delete the node (graph) with the maximum path distance
 * (the node at the most right in the tree)
*/
void deleteGraph(node_t **root, node_t *z){
    node_t *y = NIL;
    node_t *x = NIL;
    if(NIL == z->left || NIL == z->right){
        y = z;
    }
    else{
        y = treeSuccessor(z);
    }
    if(NIL != y->left){
        x = y->left;
    }
    else{
        x = y->right;
    }
    x->parent = y->parent;
    if(NIL == y->parent){
        //y is the root
        *root = x;
    }
    else if(y == y->parent->left){
        //y is a left chifd
        y->parent->left = x;
    }
    else{
        y->parent->right = x;
    }
    if(y != z){
        z->name = y->name;
        z->valueOfThePath = y->valueOfThePath;
    }
    if(BLACK == y->color){
        deleteFixup(root, x);
    }
    //printf("Eliminated graph %d\n", y->name);
    free(y);
}

node_t *treeSuccessor(node_t *x){
    if(NIL != x->right){
        return treeMinimum(x->right);
    }
    node_t *y = x->parent;
    while(NIL != y && x == y->right){
        x = y;
        y = y->parent;
    }
    return y;
}

node_t *treeMinimum(node_t *x){
    while(NIL != x->left){
        x = x->left;
    }
    return x;
}

/**
 * function to balance the tree after an erasure of a node
*/
void deleteFixup(node_t **root, node_t *x){
    //printf("Delete fixup started\n");
    while (x != (*root) && x->color == BLACK){
        if (x == x->parent->left){
            //if the node is a left child
            node_t *w = x->parent->right;
            if (w->color == RED){
                //printf("The parent is red\n");
                //if w is red, need recolor and a left rotation
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK){
                //printf("The uncle is red, switch colors and the hew node became it's grand parent\n");
                //if w has two black children it has to be red
                w->color = RED;
                x = x->parent;
            }
            else{
                //printf("The uncle is black\n");
                if (w->right->color == BLACK){
                    //printf("The new node is a left child, need a right rotation\n");
                    //w has the right black child, recolor and right rotation
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                //printf("switch colors and left rotation\n");
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = (*root);
            }
        }
        else{
            //printf("The parent is black\n");
            //same but with right and left changed
            node_t *w = x->parent->left;
            if (w->color == RED){
                //printf("The uncle is red, switch colors and the hew node became it's grand parent\n");
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else{
                if (w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = (*root);
            }
        }
    }
    x->color = BLACK;
}

/**
 * Right and Left rotation of the nodes in the tree to balance it after an insertion or an erasure
*/
void leftRotate(node_t **root, node_t *x){
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != NIL)
        y->left->parent = x;
    if (y != NIL)
        y->parent = x->parent;
    if (x->parent == NIL)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    if (x != NIL)
        x->parent = y;
}

void rightRotate(node_t **root, node_t *x){
    node_t *y = x->left;
    x->left = y->right;
    if (y->right != NIL)
        y->right->parent = x;
    if (y != NIL)
        y->parent = x->parent;
    if (x->parent == NIL)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    if (x != NIL)
        x->parent = y;
}

/**
 * inorder print names of the graphs
*/
void TopK(node_t **root, int k){
    if(NIL == *root){
        return;
    }
    TopK(&(*root)->left, k-1);
    printf("%d " , (*root)->name);
    TopK(&(*root)->right, k-1);
}