#include <stdio.h>
#include <stdlib.h>
#include "small.h"

struct RoadMap *getLastElement(struct RoadMap *head)
{
    struct RoadMap *tmp = head;

    if (head == NULL) {
        return NULL;
    }

    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    return tmp;
}

void addToRoadMap(struct RoadMap **head, struct RoadMap **totalRoadMap, int city_id, int hop_cost)
{
    struct RoadMap *newNode = malloc(sizeof(struct RoadMap));
    newNode->city_id = city_id;
    newNode->next = NULL;

    if (*head == NULL) {
        newNode->total_cost = 0;
        *head = newNode;
    } else {
        struct RoadMap *lastElement = getLastElement(*head);
        newNode->total_cost = lastElement->total_cost + hop_cost;
        lastElement->next = newNode;
    }

    //Afegeixo lo mateix x el total roadmap
    struct RoadMap *newTotalNode = malloc(sizeof(struct RoadMap));
    newTotalNode->city_id = city_id;
    newTotalNode->total_cost = newNode->total_cost;
    newTotalNode->next = NULL;

    if (*totalRoadMap == NULL) {
        *totalRoadMap = newTotalNode;
    } else {
        struct RoadMap *lastTotalElement = getLastElement(*totalRoadMap);
        lastTotalElement->next = newTotalNode;
    }
}

void printPartialRoadMap(struct RoadMap *head)
{
    struct RoadMap *current = head;

    while (current != NULL) {
        if (current->next == NULL) {
            printf("%s", citiesInfo[current->city_id].city_name);
            printf("  %d", current->total_cost);
        } else {
            printf("%s - ", citiesInfo[current->city_id].city_name);
        }
        current = current->next;
    }

    printf("\n");
}

void printTotalRoadMap(struct RoadMap *totalRoadMap, int totalCost)
{
    printf("\n\nTotal Road Map: \n");

    struct RoadMap *tmp = totalRoadMap;
    int currentCityID = -1;
    while (tmp != NULL) {
        if (tmp->next == NULL) {   // Aqui he afegit aquesta variable currentCityID que serveix perque no s'ens imprimeixi la mateixa ciutat dos cops seguits :)
            printf("%s", citiesInfo[tmp->city_id].city_name);
        } else if (tmp->city_id != currentCityID) {
            printf("%s - ", citiesInfo[tmp->city_id].city_name);
            currentCityID = tmp->city_id;
        }
        tmp = tmp->next;
    }

    printf("\n\nTotal Cost: %d\n", totalCost);
}

void deletePartialRoadMap(struct RoadMap **head)
{
    struct RoadMap *current = *head;

    while (current != NULL) {
        struct RoadMap *tmp = current->next;
        free(current);
        current = tmp;
    }

    *head = NULL;
}

void deleteTotalRoadMap(struct RoadMap **totalRoadMap)
{
    struct RoadMap *current = *totalRoadMap;

    while (current != NULL) {
        struct RoadMap *tmp = current->next;
        free(current);
        current = tmp;
    }

    *totalRoadMap = NULL;
}

int RouteSearch(int source, int destination, struct RoadMap **head, struct RoadMap **totalRoadMap)
{
    int last_visited = NUMBER_CITIES; //Mirem d'on venim per no entrar en bucle

    if (*head == NULL) { // Aixo nomes quan no hi ha res a RoadMap (l'inicialitzem)
        addToRoadMap(head, totalRoadMap, source, 0);
        last_visited = -1;
    };

    int cost = adjacency_matrix[source][destination];

    if (cost != 0) { // Aixo es quan hi ha ruta directa de source a destination
        addToRoadMap(head, totalRoadMap, destination, cost);
        return cost;
    }

    // A partir d'aqui ens trobem en el punt en que hem de fer escala (a la ciutat mes barata)
    if ((last_visited != -1) && (*head != NULL) && ((*head)->next != NULL)) {
        struct RoadMap *tmp = *head;

        while (tmp->next->next != NULL) { // AIXO NO FUNCIONA QUAN NOMES HI HA UN ITEM A ROADMAP
            tmp = tmp->next;
        }

        last_visited = tmp->city_id;
    }

    int min = 0; //
    int idx = 0;

    while ((idx < NUMBER_CITIES) && (!min)) {
        if (idx != last_visited) {
            min = adjacency_matrix[source][idx];
        }
        idx++;
    }

    int idxNextSource = idx - 1;

    for (; idx < NUMBER_CITIES; idx++) {
        if ((adjacency_matrix[source][idx] < min) && (adjacency_matrix[source][idx] != 0) && (idx != last_visited)) {
            min = adjacency_matrix[source][idx];
            idxNextSource = idx;
        }
    }

    cost = min;
    addToRoadMap(head, totalRoadMap, idxNextSource, cost);

    return RouteSearch(idxNextSource, destination, head, totalRoadMap) + cost;
}
