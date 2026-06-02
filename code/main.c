#include <stdio.h>
#include "ancestorsTree.c"

int main(void)
{
    struct RoadMap *head = NULL;
    struct RoadMap *totalRoadMap = NULL;
    struct FamilyTreeNode *familyTreeDFS = NULL;
    struct FamilyTreeNode *familyTreeBFS = NULL;
    int totalCost = 0;
    int currentCity = 0;

#if defined(LARGE_CASE)
    printf("This is a large case of the program\n");
#elif defined(MEDIUM_CASE)
    printf("This is a medium case of the program\n");
#else
    printf("This is a small case of the program\n");
#endif

    printf("Ancestors' tree:\n");

    bfsSearch(&familyTreeBFS);

    printf("BFS -> Names:\n");
    printTreeBFS(familyTreeBFS);

    printf("Partial road map:\n");
    createRoadMapBFS(familyTreeBFS, &currentCity, &head, &totalRoadMap, &totalCost);
    printTotalRoadMap(totalRoadMap, totalCost);

    deletePartialRoadMap(&head);
    deleteTotalRoadMap(&totalRoadMap);

    printf("---------------------------------\n");

    totalCost = 0;
    currentCity = 0;

    dfsSearch(&familyTreeDFS);

    printf("DFS -> Names:\n");
    printTreeDFS(familyTreeDFS, 0);

    printf("Partial road map:\n");
    createRoadMapDFS(familyTreeDFS->mother_parents, &currentCity, &head, &totalRoadMap, &totalCost);
    createRoadMapDFS(familyTreeDFS->father_parents, &currentCity, &head, &totalRoadMap, &totalCost);
    printTotalRoadMap(totalRoadMap, totalCost);

    deletePartialRoadMap(&head);
    deleteTotalRoadMap(&totalRoadMap);
    deleteTree(&familyTreeDFS);
    deleteTree(&familyTreeBFS);

    return 0;
}
