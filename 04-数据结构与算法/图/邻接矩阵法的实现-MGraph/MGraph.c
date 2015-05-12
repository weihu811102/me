#include <malloc.h>
#include <stdio.h>
#include "MGraph.h"
#include "LinkQueue.h"

typedef struct _tag_MGraph
{
    int count;
    MVertex** v;  // 定点v
    int** matrix; // 用来保存边
} TMGraph;

MGraph* MGraph_Create(MVertex** v, int n)  // O(n)
{
    TMGraph* ret = NULL;
    
    if( (v != NULL ) && (n > 0) ){    
        ret = (TMGraph*)malloc(sizeof(TMGraph));
        
        if( ret != NULL ){        
            int* p = NULL;            
            ret->count = n;            
            ret->v = (MVertex**)malloc(sizeof(MVertex*) * n);            
            ret->matrix = (int**)malloc(sizeof(int*) * n);            
            p = (int*)calloc(n * n, sizeof(int));     

            if( (ret->v != NULL) && (ret->matrix != NULL) && (p != NULL) ){            
                int i = 0;                
                for(i=0; i<n; i++) {               
                    ret->v[i] = v[i];
                    ret->matrix[i] = p + i * n;
                }
            }
            else{            
                free(p);
                free(ret->matrix);
                free(ret->v);
                free(ret);                
                ret = NULL;
            }
        }
    }
    
    return ret;
}

void MGraph_Destroy(MGraph* graph) // O(1)
{
    TMGraph* tGraph = (TMGraph*)graph;
    
    if( tGraph != NULL ) {   
        free(tGraph->v);
        free(tGraph->matrix[0]);
        free(tGraph->matrix);
        free(tGraph);
    }
}

void MGraph_Clear(MGraph* graph) // O(n*n)
{
    TMGraph* tGraph = (TMGraph*)graph;
    
    if( tGraph != NULL )
    {
        int i = 0;
        int j = 0;        
        for(i=0; i<tGraph->count; i++){        
            for(j=0; j<tGraph->count; j++){            
                tGraph->matrix[i][j] = 0;
            }
        }
    }
}

int MGraph_AddEdge(MGraph* graph, int v1, int v2, int w) // O(1)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int ret = (tGraph != NULL);
    
    ret = ret && (0 <= v1) && (v1 < tGraph->count);
    ret = ret && (0 <= v2) && (v2 < tGraph->count);
    ret = ret && (0 <= w);
    
    if( ret ){    
        tGraph->matrix[v1][v2] = w;
    }    
    return ret;
}

int MGraph_RemoveEdge(MGraph* graph, int v1, int v2) // O(1)
{
    int ret = MGraph_GetEdge(graph, v1, v2);
    
    if( ret != 0 ){    
        ((TMGraph*)graph)->matrix[v1][v2] = 0;
    }
    
    return ret;
}

int MGraph_GetEdge(MGraph* graph, int v1, int v2) // O(1)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int condition = (tGraph != NULL);
    int ret = 0;
    
    condition = condition && (0 <= v1) && (v1 < tGraph->count);
    condition = condition && (0 <= v2) && (v2 < tGraph->count);
    
    if( condition ){    
        ret = tGraph->matrix[v1][v2];
    }    
    return ret;
}

int MGraph_TD(MGraph* graph, int v) // O(n) 返回于定点v相关的边数即顶点v的度 这是有向图 无向图的话会重复
{
    TMGraph* tGraph = (TMGraph*)graph;
    int condition = (tGraph != NULL);
    int ret = 0;
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    
    if( condition ) {   
        int i = 0;        
        for(i=0; i<tGraph->count; i++){        
            if( tGraph->matrix[v][i] != 0 ) {           
                ret++;
            }
            
            if( tGraph->matrix[i][v] != 0 ){            
                ret++;
            }
        }
    }    
    return ret;
}

int MGraph_VertexCount(MGraph* graph) // O(1)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int ret = 0;
    
    if( tGraph != NULL ) {   
        ret = tGraph->count;
    }    
    return ret;
}

int MGraph_EdgeCount(MGraph* graph) // O(n*n)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int ret = 0;
    
    if( tGraph != NULL ) {   
        int i = 0;
        int j = 0;        
        for(i=0; i<tGraph->count; i++){        
            for(j=0; j<tGraph->count; j++) {           
                if( tGraph->matrix[i][j] != 0 ){                
                    ret++;
                }
            }
        }
    }    
    return ret;
}

static void recursive_dfs(TMGraph* graph, int v, int visited[], MGraph_Printf* pFunc)
{
    int i = 0;    
    pFunc(graph->v[v]);    
    visited[v] = 1;    
    printf(", ");
    
    for(i=0; i<graph->count; i++){    
        if( (graph->matrix[v][i] != 0) && !visited[i] ){ // 深度优先遍历 直接遍历某个节点边对应的另外一个端点        
            recursive_dfs(graph, i, visited, pFunc);
        }
    }
}

static void bfs(TMGraph* graph, int v, int visited[], MGraph_Printf* pFunc)
{
    LinkQueue* queue = LinkQueue_Create();
    
    if( queue != NULL ){
        LinkQueue_Append(queue, graph->v + v); // 直接+v 跳到v对应的地方 实际上就是 v[v][0]       
        visited[v] = 1; 
        
        while( LinkQueue_Length(queue) > 0 ){
            int i = 0;            
            v = (MVertex**)LinkQueue_Retrieve(queue) - graph->v; // 出队列减去 graph->v 起始地址得到偏移量            
            pFunc(graph->v[v]);            
            printf(", ");
            
            for(i=0; i<graph->count; i++){
                if( (graph->matrix[v][i] != 0) && !visited[i] ){
                    LinkQueue_Append(queue, graph->v + i); // 压人的时候 实际上是压入了定点v  这里的遍历可以理解成一颗大树                
                    visited[i] = 1;
                }
            }
        }
    }    
    LinkQueue_Destroy(queue);
}

void MGraph_DFS(MGraph* graph, int v, MGraph_Printf* pFunc)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int* visited = NULL; // visited[]数组记录了访问过的节点 便于回退
    int condition = (tGraph != NULL);
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    condition = condition && (pFunc != NULL);
    condition = condition && ((visited = (int*)calloc(tGraph->count, sizeof(int))) != NULL);
    
    if( condition ){                
        recursive_dfs(tGraph, v, visited, pFunc);     

        int i = 0;      
        for(i=0; i<tGraph->count; i++){ // 如果有某些节点没有遍历过 继续遍历       
            if( !visited[i] ){            
                recursive_dfs(tGraph, i, visited, pFunc);
            }
        }        
        printf("\n");
    }    
    free(visited);
}

void MGraph_BFS(MGraph* graph, int v, MGraph_Printf* pFunc)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int* visited = NULL;
    int condition = (tGraph != NULL);
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    condition = condition && (pFunc != NULL);
    condition = condition && ((visited = (int*)calloc(tGraph->count, sizeof(int))) != NULL);
    
    if( condition ) {    
        bfs(tGraph, v, visited, pFunc);
        int i = 0;
        for(i=0; i<tGraph->count; i++){        
            if( !visited[i] ){            
                bfs(tGraph, i, visited, pFunc);
            }
        }        
        printf("\n");
    }    
    free(visited);
}

void MGraph_Display(MGraph* graph, MGraph_Printf* pFunc) // O(n*n)
{
    TMGraph* tGraph = (TMGraph*)graph;
    
    if( (tGraph != NULL) && (pFunc != NULL) )
    {
        int i = 0;
        int j = 0;
        
        for(i=0; i<tGraph->count; i++)
        {
            printf("%d:", i);
            pFunc(tGraph->v[i]);
            printf(" ");
        }
        
        printf("\n");
        
        for(i=0; i<tGraph->count; i++)
        {
            for(j=0; j<tGraph->count; j++)
            {
                if( tGraph->matrix[i][j] != 0 )
                {
                    printf("<");
                    pFunc(tGraph->v[i]);
                    printf(", ");
                    pFunc(tGraph->v[j]);
                    printf(", %d", tGraph->matrix[i][j]);
                    printf(">");
                    printf(" ");
                }
            }
        }        
        printf("\n");
    }
}