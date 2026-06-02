#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worldJourney.c"

struct parents
{
    int mParentsID;
    int fParentsID;
};

struct bfsQueue
{
    int city_id;
    struct FamilyTreeNode **correspondingNode;
    struct bfsQueue *next;
} *queue = NULL;

int validCityID(int city_id)
{
    return (city_id >= 0) && (city_id < NUMBER_CITIES);
}

int hasCityInfo(int city_id)
{
    if (!validCityID(city_id)) {
        return 0;
    }

    if ((citiesInfo[city_id].mother_name[0] == '\0') &&
        (citiesInfo[city_id].father_name[0] == '\0') &&
        (citiesInfo[city_id].mother_parents_city_id == 0) &&
        (citiesInfo[city_id].father_parents_city_id == 0)) {
        return 0;
    }

    return 1;
}

struct parents fillNode(int citiesInfoIDX, struct FamilyTreeNode **node)
{
    struct parents p;
    int mothers_parents_id = -1;
    int fathers_parents_id = -1;

    p.mParentsID = -1;
    p.fParentsID = -1;

    if ((node == NULL) || !validCityID(citiesInfoIDX)) {
        return p;
    }

    if (*node == NULL) {
        *node = malloc(sizeof(struct FamilyTreeNode));
    }

    if (*node == NULL) {
        printf("Family Tree node cannot be NULL\n");
        exit(EXIT_FAILURE);
    }

    strcpy((*node)->motherName, citiesInfo[citiesInfoIDX].mother_name);
    strcpy((*node)->fatherName, citiesInfo[citiesInfoIDX].father_name);
    (*node)->city_id = citiesInfoIDX;
    (*node)->mother_parents = NULL;
    (*node)->father_parents = NULL;

    if (!hasCityInfo(citiesInfoIDX)) {
        return p;
    }

    mothers_parents_id = citiesInfo[citiesInfoIDX].mother_parents_city_id;
    fathers_parents_id = citiesInfo[citiesInfoIDX].father_parents_city_id;

    if (hasCityInfo(mothers_parents_id)) {
        (*node)->mother_parents = malloc(sizeof(struct FamilyTreeNode));

        if ((*node)->mother_parents == NULL) {
            printf("Family Tree node cannot be NULL\n");
            exit(EXIT_FAILURE);
        }

        p.mParentsID = mothers_parents_id;
    }

    if (hasCityInfo(fathers_parents_id)) {
        (*node)->father_parents = malloc(sizeof(struct FamilyTreeNode));

        if ((*node)->father_parents == NULL) {
            printf("Family Tree node cannot be NULL\n");
            exit(EXIT_FAILURE);
        }

        p.fParentsID = fathers_parents_id;
    }

    return p;
}

void dfsStep(struct FamilyTreeNode **current, int nextIdx)
{
    struct parents p;

    if ((current == NULL) || (nextIdx == -1)) {
        return;
    }

    p = fillNode(nextIdx, current);

    dfsStep(&((*current)->mother_parents), p.mParentsID);
    dfsStep(&((*current)->father_parents), p.fParentsID);
}

void dfsSearch(struct FamilyTreeNode **head)
{
    struct parents p;

    if (head == NULL) {
        return;
    }

    p = fillNode(0, head);

    dfsStep(&((*head)->mother_parents), p.mParentsID);
    dfsStep(&((*head)->father_parents), p.fParentsID);
}

void addToBfsQueue(int city_id, struct FamilyTreeNode **correspondingNode)
{
    struct bfsQueue *newElement;

    if ((city_id == -1) || (correspondingNode == NULL)) {
        return;
    }

    newElement = malloc(sizeof(struct bfsQueue));

    if (newElement == NULL) {
        printf("BFS queue node cannot be NULL\n");
        exit(EXIT_FAILURE);
    }

    newElement->city_id = city_id;
    newElement->correspondingNode = correspondingNode;
    newElement->next = NULL;

    if (queue == NULL) {
        queue = newElement;
    } else {
        struct bfsQueue *tmp = queue;

        while (tmp->next != NULL) {
            tmp = tmp->next;
        }

        tmp->next = newElement;
    }
}

void removeQueueElement(void)
{
    struct bfsQueue *tmp;

    if (queue == NULL) {
        return;
    }

    tmp = queue->next;
    free(queue);
    queue = tmp;
}

void clearBfsQueue(void)
{
    while (queue != NULL) {
        removeQueueElement();
    }
}

void bfsSearch(struct FamilyTreeNode **head)
{
    struct parents p;

    if (head == NULL) {
        return;
    }

    clearBfsQueue();

    p = fillNode(0, head);

    addToBfsQueue(p.mParentsID, &((*head)->mother_parents));
    addToBfsQueue(p.fParentsID, &((*head)->father_parents));

    while (queue != NULL) {
        struct FamilyTreeNode *currentNode;

        p = fillNode(queue->city_id, queue->correspondingNode);
        currentNode = *(queue->correspondingNode);

        addToBfsQueue(p.mParentsID, &(currentNode->mother_parents));
        addToBfsQueue(p.fParentsID, &(currentNode->father_parents));

        removeQueueElement();
    }
}

void printNode(struct FamilyTreeNode *node, int level)
{
    int idx;

    for (idx = 0; idx < level; idx++) {
        printf("->");
    }

    if (level > 0) {
        printf(" ");
    }

    printf("%s and %s (%s)\n",
           node->motherName,
           node->fatherName,
           citiesInfo[node->city_id].city_name);
}

void printTreeDFS(struct FamilyTreeNode *node, int level)
{
    if (node == NULL) {
        return;
    }

    printNode(node, level);
    printTreeDFS(node->mother_parents, level + 1);
    printTreeDFS(node->father_parents, level + 1);
}

int treeHeight(struct FamilyTreeNode *node)
{
    int motherHeight;
    int fatherHeight;

    if (node == NULL) {
        return 0;
    }

    motherHeight = treeHeight(node->mother_parents);
    fatherHeight = treeHeight(node->father_parents);

    if (motherHeight > fatherHeight) {
        return motherHeight + 1;
    }

    return fatherHeight + 1;
}

void printTreeLevel(struct FamilyTreeNode *node, int currentLevel, int targetLevel)
{
    if (node == NULL) {
        return;
    }

    if (currentLevel == targetLevel) {
        printNode(node, currentLevel);
    } else {
        printTreeLevel(node->mother_parents, currentLevel + 1, targetLevel);
        printTreeLevel(node->father_parents, currentLevel + 1, targetLevel);
    }
}

void printTreeBFS(struct FamilyTreeNode *node)
{
    int level;
    int height = treeHeight(node);

    for (level = 0; level < height; level++) {
        printTreeLevel(node, 0, level);
    }
}

void printTree(struct FamilyTreeNode *node)
{
    printTreeDFS(node, 0);
}

void travelToCity(int destination,
                  int *currentCity,
                  struct RoadMap **head,
                  struct RoadMap **totalRoadMap,
                  int *totalCost)
{
    int cost;

    if ((currentCity == NULL) || (destination == -1)) {
        return;
    }

    cost = RouteSearch(*currentCity, destination, head, totalRoadMap);
    *totalCost = *totalCost + cost;
    printRoadMap(*head);
    deletePartialRoadMap(head);
    *currentCity = destination;
}

void createRoadMapDFS(struct FamilyTreeNode *node,
                      int *currentCity,
                      struct RoadMap **head,
                      struct RoadMap **totalRoadMap,
                      int *totalCost)
{
    if (node == NULL) {
        return;
    }

    travelToCity(node->city_id, currentCity, head, totalRoadMap, totalCost);
    createRoadMapDFS(node->mother_parents, currentCity, head, totalRoadMap, totalCost);
    createRoadMapDFS(node->father_parents, currentCity, head, totalRoadMap, totalCost);
}

void createRoadMapLevel(struct FamilyTreeNode *node,
                        int currentLevel,
                        int targetLevel,
                        int *currentCity,
                        struct RoadMap **head,
                        struct RoadMap **totalRoadMap,
                        int *totalCost)
{
    if (node == NULL) {
        return;
    }

    if (currentLevel == targetLevel) {
        travelToCity(node->city_id, currentCity, head, totalRoadMap, totalCost);
    } else {
        createRoadMapLevel(node->mother_parents, currentLevel + 1, targetLevel, currentCity, head, totalRoadMap, totalCost);
        createRoadMapLevel(node->father_parents, currentLevel + 1, targetLevel, currentCity, head, totalRoadMap, totalCost);
    }
}

void createRoadMapBFS(struct FamilyTreeNode *node,
                      int *currentCity,
                      struct RoadMap **head,
                      struct RoadMap **totalRoadMap,
                      int *totalCost)
{
    int level;
    int height = treeHeight(node);

    for (level = 1; level < height; level++) {
        createRoadMapLevel(node, 0, level, currentCity, head, totalRoadMap, totalCost);
    }
}

void deleteTree(struct FamilyTreeNode **node)
{
    if ((node == NULL) || (*node == NULL)) {
        return;
    }

    deleteTree(&((*node)->mother_parents));
    deleteTree(&((*node)->father_parents));
    free(*node);
    *node = NULL;
}
