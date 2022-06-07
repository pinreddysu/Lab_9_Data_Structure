#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define size 26
#define Filename "graph.txt"
#define max_char 60
typedef struct AdjListNode
{
    char dest;
    int weight;
    struct AdjListNode* next;
}llnode;

typedef struct AdjList
{
    //char label[26];
    //struct AdjList *next;
    char label;
    llnode *head;  // pointer to head node of list
}ll_arra;

typedef struct Graph
{
    int V;
    //char arr[26];
    //int *visited;
    ll_arra* array;
}graph_t;

void topologica_sort(graph_t *graph, int size1);
char * removeSpacesFromStr(char *string);
llnode* newAdjListNode(char dest, int weight);
graph_t* createGraph(int V, char *vertex_ins);
void addEdge(graph_t* graph, char src, char dest, int weight, int Flag);
void printGraph(graph_t* graph, int tot_vertex);
void print_topological_search_results(char origin[],int critical_path[], int max_path[], char queue[], int size1);
/*
 AUTHOR: Yash Nikumb, Suchith Pinreddy 
 FILENAME: main.c
 SPECIFICATION: to build the adjacency list for a directed graph from a graph file and run the topological sort on the graph where the maximum path length and critical path
 FOR: CS 2413 Data Structures Section 002
*/
int main()
{
   FILE *infile;
    infile = fopen(Filename, "r");
    int val;
    int val1;
    int size2 = 0;
    char vertex[max_char];
    char *vertex_wo_space;
    char *word;
    char edges[max_char];
    char total_nodes[1];
    char source;
    char destination;
    char weight_char[5];
    int weight_conv;
    
    //word[2] = '\0';
    fgets(total_nodes, max_char, infile);
    val = atoi(total_nodes);
    //printf("VAL: %d\n", val);
    char undirect[1];
    fgets(undirect, max_char, infile);
    val1 = atoi(undirect);
    //printf("VAL!: %d\n", val1);
    fgets(vertex, max_char, infile);
    vertex_wo_space = removeSpacesFromStr(vertex);
    //printf("VERTEX: %s", vertex_wo_space);
    graph_t* graph = createGraph(val, vertex_wo_space);
    while(fgets(edges, max_char, infile)){
        int ver_weight = 0;
        //printf("Vertex_weight %s\n", edges);
        //val1 = atoi(vertex_weight);
        word = removeSpacesFromStr(edges);
        //printf("EDGES: %s\t", word);
        source = word[0];
        //printf("SORCE: %c\t", source);
        destination = word[1];
        //printf("DEST: %c\t", destination);
        //strncpy(weight_char, word, 1);
        //weight_conv = atoi(weight_char);
        for (int i = 0; i < strlen(word); i++){
          if(isdigit(word[i])){
            int digit = word[i] - '0';
            ver_weight  = ver_weight * 10 +digit; 
          }
        }
        //printf("WEIGHT %d\n", weight);
        //is_alpha(vertex_weight);
       addEdge(graph, source, destination, ver_weight, val1);
    }
    printf("\n Adjacency list: \n");
    printf("  Number of vertices %d:\n",val);
    printGraph(graph, val);
    //bfs(graph, val, 'C');
    size2 = graph->V;
    topologica_sort(graph, size2);
    return 0;
}

 
/* NAME: removeSpacesFromStr
 PARAMETERS: char *string
 PURPOSE: The function removes the spaces from the string
 PRECONDITION: The actual parmeters passed should be a char pointer
 POSTCONDITION: It should remove all spaces from the string in the file
*/
char * removeSpacesFromStr(char *string)
{
    
    int non_space_count = 0;

    
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] != ' ')
        {
            string[non_space_count] = string[i];
            non_space_count++;//non_space_count incremented
        }
    }

    string[non_space_count] = '\0';
    return string;
} 
/* NAME: newAdjListNode
 PARAMETERS: char dest, int weight
 PURPOSE: It is used to print new adjaent list node
 PRECONDITION: The variables passed should be a char and an int value
 POSTCONDITION: It should add adj list node correctly 
*/
llnode* newAdjListNode(char dest, int weight)
{
    llnode* newNode = (llnode*) malloc(sizeof(llnode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
graph_t* createGraph(int V, char *vertex_ins)
{
    graph_t* graph = (graph_t*) malloc(sizeof(graph_t));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (ll_arra*) malloc(V * sizeof(ll_arra));

    // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    for (int j = 0; j < V; ++j)
        graph->array[j].label = vertex_ins[j];

    return graph;
}
/* NAME: addEdge
 PARAMETERS: graph_t* graph, char src, char dest, int weight
 PURPOSE: The purpose of the function is to join vertex and edges 
 PRECONDITION: The variables passed should be a graph, char and two int values simultaneously
 POSTCONDITION: It should join edges to vertices correctly
*/
void addEdge(graph_t* graph, char src, char dest, int weight, int Flag)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the beginin
    llnode* newNode = newAdjListNode(dest, weight);

    //***Changed. you need to add adge in src node. But you were always adding in 0
    llnode* temp=graph->array[src - 'A'].head;

    if(temp==NULL)  // First element of the list
      graph->array[src - 'A'].head=newNode;
    else
    {
        while(temp->next!=NULL) // finding the last element of the list
            temp=temp->next;
        temp->next=newNode; // Adding current element to the last
    }
    //printf("x%d\n", Flag);
    if(!(Flag)){

    // Since graph is undirected, add an edge from dest to src also
        newNode = newAdjListNode(src, weight);

    //***Changed. you need to add adge in dest node. But you were always adding in 1
        temp = graph->array[dest - 'A'].head;

        if(temp==NULL) // First element of the list
            graph->array[dest - 'A'].head=newNode;
        else
        {
            while(temp->next!=NULL) // finding the last element of the list
                temp=temp->next;
            temp->next=newNode; // Adding current element to the last
        }
    }
}
/* NAME: printGraph
 PARAMETERS:  graph_t* graph, int tot_vertex
 PURPOSE: The purpose of the function is to print the adjacency list out.
 PRECONDITION: The variables as actual parameters should be a struct type and an integer
 POSTCONDITION: Function should print the correct adjacency list.
*/

void printGraph(graph_t* graph, int tot_vertex)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        llnode* pCrawl = graph->array[v].head;
        //struct AdjList *temp;
        //temp = graph->array;
        //printf("%s\n", temp[v]);
        //printf("FLAG %c\n", graph->array[v].label);
        printf("  %c: ", graph->array[v].label);
        while (pCrawl)
        {
            printf(" (%c, %d) -> ", pCrawl->dest,pCrawl->weight);
            pCrawl = pCrawl->next;
        }
        printf(" (nil)\n");
    }
}
 /* NAME: topologica_sort 
 PARAMETERS: graph_t *graph, int size1)
 PURPOSE: The function calculates the critical path max_path origin and the vertex. 
 PRECONDITION: The actual variables passed should be a graph and an int value 
 POSTCONDITION: It should print all the info correctly
*/
void topologica_sort(graph_t *graph, int size1){
  llnode *ptr = NULL;
  llnode *ptr1 = NULL;
  llnode *ptr2 = NULL;
  llnode *ptr3 = NULL;
  int max_path[size1];
  int weight_arr[size1];
  int indeg[size1];
  int critical_path[size1];
  for(int i =0; i <size1; i++){
    max_path[i] = 0;
    weight_arr[i] = 0;
    indeg[i] = 0;
    critical_path[i] = 0;
  }
  //nt weight_arr[size1];
  //char largori[8]= {'\0'};
  //int status[size] = {1}
  char queue[size1];
  char origin[size1];
  for(int i =0; i <size1; i++){
    queue[i] = '\0';
    origin[i] = '\0';
    //indeg[i] = 0;
    //critical_path[i] = 0;
  }
  //char queue[size1];
  char ver;
  int ver1;
  char temp1;
  char x;
  char temp2[8]={'\0'};
  int temp;
  int flag = 0;
  int front1 = -1;
  char deter[size] = {'\0'};
  //char origin[size] = {'\0'};
  int front =-1;
  int rear = -1;
  for(int i =0; i < graph->V; ++i){
    ptr1 = graph->array[i].head;
    while(ptr1!=NULL){
      indeg[ptr1->dest - 'A'] += 1;
      ptr1 = ptr1->next;
    }
  }
  for(int i =0; i < graph->V; ++i){
    ptr2 = graph->array[i].head;
    flag = 0;
    while(ptr2!=NULL){
      //critical_path[ptr2->dest - 'A'] += 1;
      flag =1;
      ptr2 = ptr2->next;
    }
    if(flag==0){
      critical_path[graph->array[i].label - 'A'] = 1;
    }
  }
  for(int j = 0; j < graph->V; ++j){
    if(indeg[j]==0){
      queue[++rear] = j+'A';
    }
  }
  ver = queue[0];
  front++;
  while(front<=rear){
    ver1 = front;
    ptr = graph->array[queue[front++] - 'A'].head;
    while(ptr!=NULL){
      if(max_path[ptr->dest - 'A'] <= ptr->weight + max_path[graph->array[queue[ver1]-'A'].label-'A']){
        max_path[ptr->dest - 'A'] = max_path[graph->array[queue[ver1]-'A'].label-'A'] + ptr->weight;
      origin[ptr->dest -'A'] = graph->array[queue[ver1]-'A'].label;
        }
      indeg[ptr->dest - 'A'] -= 1;
      if(indeg[ptr->dest-'A'] == 0){
        queue[++rear] = ptr->dest;
        }
      ptr = ptr->next;
    }
    ver = queue[front];
    }
  char p;
  for(int k = 0; k<graph->V; k++){

    p = origin[k];
    if(p!= '\0'&& critical_path[p-'A'] == 0){
      
      critical_path[p-'A'] = 1;
    }
    }
  printf("\nTopological Search Results: \n\n");
  print_topological_search_results(origin, critical_path, max_path, queue, size1);
  
} 
/* NAME: print_topological_search_results
 PARAMETERS: char origin[],int critical_path[], int max_path[], char queue[], int size1
 PURPOSE: The function prints the topological search results i.e the vertex origin max path and queue.
 PRECONDITION: The actual arguments passed should be a char string, 2 int values, a char value and another int value simulataneously
 POSTCONDITION: It should print all the information of the topological graph correctly
*/
void print_topological_search_results(char origin[],int critical_path[], int max_path[], char queue[], int size1){
  printf("Vertex\t Origin\t Max_Path\tCritical_Path\n\n");
  for (int i = 0; i<size1; i++){
    printf("  %c\t\t\t%c\t   %d\t      \t\t%d\n",queue[i], origin[queue[i]-'A'], max_path[queue[i]-'A'], critical_path[queue[i]-'A']);
  }
}


/* 
Test cases:
Example directed graph file – passed FILENAME: graph.txt
Output: 
Adjacency list: 
  Number of vertices 7:
  A:  (B, 4) ->  (nil)
  B:  (C, 2) ->  (D, 2) ->  (E, 2) ->  (nil)
  C:  (E, 1) ->  (nil)
  D:  (E, 9) ->  (nil)
  E:  (F, 3) ->  (nil)
  F:  (nil)
  G:  (D, 5) ->  (nil)

Topological Search Results: 

Vertex   Origin  Max_Path   Critical_Path

  A                0                1
  G                0                0
  B         A      4                1
  C         B      6                0
  D         B      6                1
  E         D      15               1
  F         E      18               1

2. Directed graph file where weights are all the same – passed.If weigths are the same it can have different types of graphs. In this graph whenever indegree is zero, the parent which helps the indegree to be zero will be in the origin .
FILENAME: graph_same_weight.txt
Output: 
 Adjacency list: 
  Number of vertices 7:
  A:  (B, 7) ->  (nil)
  B:  (C, 7) ->  (D, 7) ->  (E, 7) ->  (nil)
  C:  (E, 7) ->  (nil)
  D:  (E, 7) ->  (nil)
  E:  (F, 7) ->  (nil)
  F:  (nil)
  G:  (D, 7) ->  (nil)

Topological Search Results: 

Vertex   Origin  Max_Path   Critical_Path

  A                0                1
  G                0                0
  B         A      7                1
  C         B      14               0
  D         B      14               1
  E         D      21               1
  F         E      28               1 
iii. Graph Analysis
1. Big O of finding the indegree of each vertex: O(V+E)
2. Big O of the determining the critical path: O(V)
3. Big O of the topological sort that tracks the maximum path lengths and critical path: O(V+E)
4. Big O of the overall storage requirements for the topological sort that tracks the maximum path lengths and critical path: The storage requirements are V+E as we have used array of linked list to store vertex and edges. 
 Team Members: 
1. Suchith Pinreddy: 
a) void topologica_sort(graph_t *graph, int size1); = Implemented
b) char * removeSpacesFromStr(char *string); = Debugged
c) llnode* newAdjListNode(char dest, int weight); = Checked
d) raph_t* createGraph(int V, char *vertex_ins); = Checked
e) void addEdge(graph_t* graph, char src, char dest, int weight, int Flag);= Implemeted
f) void printGraph(graph_t* graph, int tot_vertex); =Checked
g) void print_topological_search_results(char origin[],int critical_path[], int max_path[], char queue[], int size1); = implemeted

2. Yash Nikumb:  
a) void topological_sort(graph_t *graph, int size1); = debugged
b) char * removeSpacesFromStr(char *string); = Debugged
c) llnode* newAdjListNode(char dest, int weight); = Implemeted
d) raph_t* createGraph(int V, char *vertex_ins); = Implemeted
e) void addEdge(graph_t* graph, char src, char dest, int weight, int Flag);= Debugged
f) void printGraph(graph_t* graph, int tot_vertex); =Checked
g) void print_topological_search_results(char origin[],int critical_path[], int max_path[], char queue[], int size1); = Checked
*/