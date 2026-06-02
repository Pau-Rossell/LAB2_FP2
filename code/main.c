#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "small.h"
struct RoadMap *head = NULL;
struct RoadMap *totalRoadMap = NULL;
struct FamilyTreeNode *familyTreeDFS = NULL; // Family Tree que plenarem amb DFS
struct FamilyTreeNode *familyTreeBFS = NULL; // Family Tree que plenarem amb BFS
struct parents
{
    int mParentsID;
    int fParentsID;
};
#include "ancestorsTree.c"
int main()
{
    int totalCost;

    //DFS 

    familyTreeDFS = malloc(sizeof(struct FamilyTreeNode));
    familyTreeBFS = malloc(sizeof(struct FamilyTreeNode));

    head = NULL;
    totalRoadMap = NULL;

    printf("Ancestors' tree:\n\n");

    dfsSearch(familyTreeDFS);

    printf("DFS -> Names:\n");
    printTree(familyTreeDFS);

    //printf("\nPartial road map:\n");
    //printPartialRoadMap();

    //printf("\nTotal Road Map:\n");
    //printTotalRoadMap(0);

    printf("\n----------------------------------\n\n");
    
    
    //BFS

    //head = NULL;
    //totalRoadMap = NULL;

    //printf("BFS -> Names:\n");

    //bfsSearch(familyTreeBFS);

    //printTree(familyTreeBFS);

    //printf("\nPartial road map:\n");
    //printPartialRoadMap();

    //printf("\nTotal Road Map:\n");
   // printTotalRoadMap(0);


    //CLEAN MEMORY

    //deleteAllRoadMap();
    // deleteTotalRoadMap();

    // deleteTree(familyTreeDFS);
    // deleteTree(familyTreeBFS);
    return 0;
}