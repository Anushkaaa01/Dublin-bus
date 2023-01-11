#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "t2.h"
#define INFINITY 50000
#define MAX_LENGTH 40
int edge_cnt=0;
int vertex_cnt=0;

int edges[7668][7668];

typedef struct vertices vertices;
struct vertices
{
    char name[100];
    char latitude[20];
    char longitude[20];
};
vertices* vertex[7668];

void next_token(char *buf, FILE *f)
{
    int i=0;
    int flag=0; //0 when outside, 1 when inside quotation

	for (;;) {
        char c=fgetc(f);
		buf[i] = c;                

        if (buf[i]=='\"')
        {
            if (flag)
            {
                flag=0;
            }
            else
            {
                flag=1;
            }
            continue;
        }

		if( buf[i] == ',' && !flag) { break; } 
		if( feof(f) ) { break; }     
        if(buf[i]=='\n'){break;}
		if( i <= (MAX_LENGTH-1) ) { ++i; } 
	}
	buf[i] = '\0'; 
    
}

int load_edges(char *fname)
{
    FILE *f;
    char buf[MAX_LENGTH];
    f=fopen("edges.csv","r");
    if(!f)
    {
        printf("Unable to open %s\n",fname);
        return 0;
    }
    for(int i=0;i<3;i++)
    {
        next_token(buf,f);
    }
    int pos=0;
    while(pos<6180)
    {
        next_token(buf,f);
        int from=atoi(buf);
        next_token(buf,f);
        int to=atoi(buf);
        next_token(buf,f);
        int weight=atoi(buf);
        edges[from][to]=weight;
        pos++;
        edge_cnt++;
    }
    fclose(f);
    printf("\n%d edges loaded",edge_cnt);
	return 1;
}

int load_vertices(char* fname)
{   
    FILE *f2;
    char buf2[MAX_LENGTH];
    f2=fopen("vertices.csv","r");
    if(!f2)
    {
        printf("Unable to open %s\n",fname);
        return 0;
    }
    for(int i=0;i<4;i++)
    {
        next_token(buf2,f2);
    }
    int pos=0;
    while(pos<4807)
    {   
        vertices* x=malloc(sizeof(vertices));
        next_token(buf2,f2);
        int stopid=atoi(buf2);
        next_token(buf2,f2);
        strcpy(x->name,buf2);
        next_token(buf2,f2);
        strcpy(x->latitude,buf2);
        next_token(buf2,f2);
        strcpy(x->longitude,buf2);
        pos++;
        vertex_cnt++;
        vertex[stopid]=x;
    }
    fclose(f2);
    printf("\n%d vertices loaded",vertex_cnt);
	return 1;
}
int visited[7668];
int previous[7668];
int distances[7668];

void shortest_path(int startNode, int endNode)
{
    if(startNode==endNode)
    {
        return;
    }
    visited[startNode]=1;

    //updating distances
    for (int i = 0; i < 7668; i++)
    {
        if (edges[startNode][i]>0 && distances[i]>edges[startNode][i]+distances[startNode])
        {
            distances[i]=edges[startNode][i]+distances[startNode];
            previous[i]=startNode;
        }
        
    }

    int shortestDistance=INFINITY;
    int nextNode=0;
    //finding shortest distance
    for (int i = 0; i < 7668; i++)
    {
        if (distances[i]<shortestDistance && visited[i]==0)
        {
            shortestDistance=distances[i];
            nextNode=i;
        }
        
    }
    
    shortest_path(nextNode,endNode);

    
}

int print_path(int startNode, int endNode)
{
    if (startNode==endNode)
    {
        return 0;
    }
    printf("Shortest path from %d to %d is",startNode,endNode);
    shortest_path(startNode,endNode);
    for(int i=0;i<7668;i++)
    {
        printf("%d",previous[i]);
    }
}


int main(){
    //Initialize
    load_edges("edges.csv");
    load_vertices("vertices.csv");
    for (int i = 0; i < 7668; i++)
    {
        visited[i]=0;
        distances[i]=INFINITY;
    }

    int startNode, endNode;
    //take startnode and end node from user

    distances[startNode]=0;
    print_path(10,1006);


}
