#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// structure of an undirected graph
struct edge
{
 // vertices
 long vertex1;
 long vertex2;
};

struct graph
{
 long n_vertex;
 long n_edges;
 struct edge *list_of_edges;
};

struct graph *readgraph(char *filename)
{
 FILE *fd;
 struct edge *list_of_edges;
 struct graph *G;
 long vertex1,vertex2;
 long i,no_of_edges,no_of_vertices;

 printf("Reading file %s\n",filename);

 fd=fopen(filename,"r");
 if(fd == NULL){
	perror("Error opening the file.\n");
	exit(-1);
 }
 /** Complete here  **/
 char line[256];
 char * pch;
 i=0;
 G = (struct graph*)malloc(sizeof(struct graph));
 no_of_vertices = strtol(fgets(line, sizeof(line), fd),NULL, 10);
 no_of_edges = strtol(fgets(line, sizeof(line), fd),NULL, 10);
 G->n_vertex = no_of_vertices;
 G->n_edges = no_of_edges;
 list_of_edges = (struct edge*)malloc(no_of_edges * sizeof(struct edge));

 while (fgets(line, sizeof(line), fd)) {
     pch = strtok (line," ");
     vertex1 = strtol(pch, NULL, 10);
     pch = strtok(NULL, " ");
     vertex2 = strtol(pch, NULL, 10);
     list_of_edges[i].vertex1 = vertex1;
     list_of_edges[i].vertex2 = vertex2;
     i++;
 }

 G->list_of_edges = list_of_edges;

 free(list_of_edges);

 printf("V = %ld  E = %ld\n",G->n_vertex,G->n_edges);

 return G;
}

int main(int argc, char **argv)
{
 long i;
 long *degree;
 double start_time, run_time;
 struct graph *G;

 if (argc != 2){
    printf("Please enter filename containing the graph structure.\n");
    exit(0);
 }

 G = readgraph(argv[1]);

 start_time = omp_get_wtime();
 /** Complete **/
 degree = (long*)malloc(G->n_vertex * sizeof(long));
 #pragma omp parallel for shared(degree,G)
     for(i = 0; i < G->n_edges; i++){
         #pragma omp atomic
         degree[G->list_of_edges[i].vertex1] ++;
         degree[G->list_of_edges[i].vertex2] ++;
     }

 run_time = omp_get_wtime() - start_time;

 for(i = 0; i < G->n_vertex; i++){
     printf("%ld\n", degree[i]);
 }

 printf("Time to calculte all degrees (sequential): %lf\n", run_time);
 free(degree);
 free(G);
}
