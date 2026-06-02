#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worldJourney.c"

// -------------------------------------------------------------------------------------------------------
// Primer tota la llogica general per gestionar el Family Tree


// Aquesta estructura em feia falta per retornar una parella de integers en una funcio
struct parents
{
    int mParentsID;
    int fParentsID;
};

struct parents fillNode(int citiesInfoIDX, struct FamilyTreeNode **node)
{
    // Aquesta funcio senzillament emplena un node amb els valors corresponents, perque fa pal fer-ho cada cop :)
    // RETORNA L'INDEX DE MOTHER'S PARENTS I FATHER'S PARENTS
    if (*node == NULL) {
        *node = malloc(sizeof(struct FamilyTreeNode));
    }

    if (*node == NULL) {
        printf("Family Tree node Cannot be NULL\n");
        exit(EXIT_FAILURE);
    }

    strcpy((*node)->motherName, citiesInfo[citiesInfoIDX].mother_name);
    strcpy((*node)->fatherName, citiesInfo[citiesInfoIDX].father_name);
    (*node)->city_id = citiesInfoIDX;

    int mothers_parents_id = citiesInfo[citiesInfoIDX].mother_parents_city_id;
    int fathers_parents_id = citiesInfo[citiesInfoIDX].father_parents_city_id;

    (*node)->mother_parents = NULL;
    (*node)->father_parents = NULL;

    if (mothers_parents_id != -1) {
        (*node)->mother_parents = malloc(sizeof(struct FamilyTreeNode));
    }

    if (fathers_parents_id != -1) {
        (*node)->father_parents = malloc(sizeof(struct FamilyTreeNode));
        }

    struct parents p;
    p.mParentsID = mothers_parents_id;
    p.fParentsID = fathers_parents_id;

    return p;
}


// -------------------------------------------------------------------------------------------------------
// A partir d'aqui el DFS

void dfsStep(struct FamilyTreeNode **current, int nextIdx, struct RoadMap **head, struct RoadMap **totalRoadMap)
{
    // Aquesta funcio senzillamen es el pas de dfs que es truca de forma recursiva
    if ((current == NULL) || (nextIdx == -1)) {
        return;
    }

    struct parents p = fillNode(nextIdx, current);

    if (p.mParentsID != -1) {

        RouteSearch(nextIdx, p.mParentsID, head, totalRoadMap);
        printPartialRoadMap(*head);
        deletePartialRoadMap(head);
        dfsStep(&((*current)->mother_parents), p.mParentsID, head, totalRoadMap);

    }

    if (p.fParentsID != -1) {

        RouteSearch(nextIdx, p.fParentsID, head, totalRoadMap);
        printPartialRoadMap(*head);
        deletePartialRoadMap(head);
        dfsStep(&((*current)->father_parents), p.fParentsID, head, totalRoadMap);
    }
}

void dfsSearch(struct FamilyTreeNode *head, struct RoadMap **roadMapHead, struct RoadMap **totalRoadMap)
{
    // I aquesta es la ultima de dfs que senzillament inicialitza el familytree si no esta inicialitzat, i truca dfsStep
    if (head == NULL) {
        return;
    }

    struct parents p = fillNode(0, &head);

    dfsStep(&(head->mother_parents), p.mParentsID, roadMapHead, totalRoadMap);
    dfsStep(&(head->father_parents), p.fParentsID, roadMapHead, totalRoadMap);
}


// -------------------------------------------------------------------------------------------------------
// I a partir d'aqui el BFS


// Aixo servira de regirstre de les ciutats on hem d'anar per a cada layer del bfs
struct bfsQueue
{
    int city_id;
    struct FamilyTreeNode **correspondingNode;
    struct bfsQueue *next;
}*queue = NULL;

void addToBfsQueue(int city_id, struct FamilyTreeNode **correspondingNode)
{
    if (city_id == -1) {
        return;
    }

    struct bfsQueue *newElement = malloc(sizeof(struct bfsQueue));

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

void removeQueueElement()
{
    if (queue == NULL) {
        return;
    }

    struct bfsQueue *tmp = queue->next;
    free(queue);
    queue = tmp;
}

void bfsSearch(struct FamilyTreeNode **head)
{
    if (head == NULL) {
        return;
    }

    while (queue != NULL) {
        removeQueueElement();
    }

    struct parents p = fillNode(0, head);

    addToBfsQueue(p.mParentsID, &((*head)->mother_parents));
    addToBfsQueue(p.fParentsID, &((*head)->father_parents));

    while (queue != NULL) {
        p = fillNode(queue->city_id, queue->correspondingNode);

        struct FamilyTreeNode *currentNode = *(queue->correspondingNode);
        addToBfsQueue(p.mParentsID, &(currentNode->mother_parents));
        addToBfsQueue(p.fParentsID, &(currentNode->father_parents));

        removeQueueElement();
    }
}


void printTree(struct FamilyTreeNode *node)
{
    if (node == NULL) {
        return;
    }

    printf("%s / %s - %s\n",
           node->motherName,
           node->fatherName,
           citiesInfo[node->city_id].city_name);

    printTree(node->mother_parents);
    printTree(node->father_parents);
}
