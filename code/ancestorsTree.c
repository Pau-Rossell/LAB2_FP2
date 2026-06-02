#include "./small.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worldJourney.c"

struct FamilyTreeNode *familyTreeDFS = NULL; // Family Tree que plenarem amb DFS
struct FamilyTreeNode *familyTreeBFS = NULL; // Family Tree que plenarem amb BFS

// Aquesta estructura em feia falta per retornar una parella de integers en una funcio
struct parents
{
    int mParentsID;
    int fParentsID;
};

struct parents fillNode(int citiesInfoIDX, struct FamilyTreeNode *node)
{
    // Aquesta funcio senzillament emplena un node amb els valors corresponents, perque fa pal fer-ho cada cop :)
    // RETORNA L'INDEX DE MOTHER'S PARENTS I FATHER'S PARENTS
    if (node == NULL) {
        node = malloc(sizeof(struct FamilyTreeNode));
    }

    strcpy(node->motherName, citiesInfo[citiesInfoIDX].mother_name);
    strcpy(node->fatherName, citiesInfo[citiesInfoIDX].father_name);
    node->city_id = citiesInfoIDX;
    node->mother_parents = malloc(sizeof(struct FamilyTreeNode));
    node->father_parents = malloc(sizeof(struct FamilyTreeNode));

    int mothers_parents_id = citiesInfo[citiesInfoIDX].mother_parents_city_id;
    int fathers_parents_id = citiesInfo[citiesInfoIDX].father_parents_city_id;

    struct parents p;
    p.mParentsID = mothers_parents_id;
    p.fParentsID = fathers_parents_id;

    return p;
}

void dfsStep(struct FamilyTreeNode *current, int nextIdx)
{
    // Aquesta funcio senzillamen es el pas de dfs que es truca de forma recursiva
    struct parents p = fillNode(nextIdx, current);

    if (!(p.mParentsID == -1 && p.fParentsID == -1)) {
        dfsStep(current->mother_parents, p.mParentsID);
        dfsStep(current->father_parents, p.fParentsID);
    }
}

void dfsSearch(struct FamilyTreeNode *head)
{
    // I aquesta es la ultima de dfs que senzillament inicialitza el familytree si no esta inicialitzat, i truca dfsStep
    if (head == NULL) {
        return
    }

    struct parents p = fillNode(0, head);

    dfsStep(head->mother_parents, p.mParentsID);
    dfsStep(head->father_parents, p.fParentsID);
}


void bfsStep(struct FamilyTreeNode *mother, struct FamilyTreeNode *father, int motherParentsID, int fatherParentsID){
    
}

void bfsSearch(struct FamilyTreeNode *head){
    if (head == NULL) {
        return
    }

    struct parents p = fllNode(0, head);

    bfsStep(head->mother_parents, head->father_parents, p.mParentsID, p.fParentsID);


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

int main(void)
{
    familyTreeDFS = malloc(sizeof(struct FamilyTreeNode));
    dfsSearch(familyTreeDFS);
    printf("DFS family tree:\n");
    printTree(familyTreeDFS);

    familyTreeBFS = malloc(sizeof(struct FamilyTreeNode));
    printf("\nBFS family tree:\n");
    printTree(familyTreeBFS);

    return 0;
}
