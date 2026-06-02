#include <stdio.h>
#include <stdlib.h>

#if defined(LARGE_CASE)
#include "large.h"
#elif defined(MEDIUM_CASE)
#include "medium.h"
#else
#include "small.h"
#endif

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

int getPreviousCity(struct RoadMap *head)
{
    struct RoadMap *tmp = head;

    if ((tmp == NULL) || (tmp->next == NULL)) {
        return -1;
    }

    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }

    return tmp->city_id;
}

void addToRoadMap(struct RoadMap **head, struct RoadMap **totalRoadMap, int city_id, int hop_cost)
{
    struct RoadMap *newNode = malloc(sizeof(struct RoadMap));

    if (newNode == NULL) {
        printf("Road map node cannot be NULL\n");
        exit(EXIT_FAILURE);
    }

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

    struct RoadMap *lastTotalElement = getLastElement(*totalRoadMap);

    if ((lastTotalElement == NULL) || (lastTotalElement->city_id != city_id)) {
        struct RoadMap *newTotalNode = malloc(sizeof(struct RoadMap));

        if (newTotalNode == NULL) {
            printf("Road map node cannot be NULL\n");
            exit(EXIT_FAILURE);
        }

        newTotalNode->city_id = city_id;
        newTotalNode->total_cost = newNode->total_cost;
        newTotalNode->next = NULL;

        if (*totalRoadMap == NULL) {
            *totalRoadMap = newTotalNode;
        } else {
            lastTotalElement->next = newTotalNode;
        }
    }
}

void printRoadMap(struct RoadMap *head)
{
    struct RoadMap *current = head;
    int totalCost = 0;

    while (current != NULL) {
        printf("%s", citiesInfo[current->city_id].city_name);
        totalCost = current->total_cost;

        if (current->next != NULL) {
            printf("-");
        }

        current = current->next;
    }

    printf(" %d\n", totalCost);
}

void printPartialRoadMap(struct RoadMap *head)
{
    printRoadMap(head);
}

void printTotalRoadMap(struct RoadMap *totalRoadMap, int totalCost)
{
    struct RoadMap *tmp = totalRoadMap;

    printf("Total Road Map:\n");

    while (tmp != NULL) {
        printf("%s", citiesInfo[tmp->city_id].city_name);

        if (tmp->next != NULL) {
            printf("-");
        }

        tmp = tmp->next;
    }

    printf("\nTotal cost: %d\n", totalCost);
}

void deleteAllRoadMap(struct RoadMap **head)
{
    struct RoadMap *current = *head;

    while (current != NULL) {
        struct RoadMap *tmp = current->next;
        free(current);
        current = tmp;
    }

    *head = NULL;
}

void deletePartialRoadMap(struct RoadMap **head)
{
    deleteAllRoadMap(head);
}

void deleteTotalRoadMap(struct RoadMap **totalRoadMap)
{
    deleteAllRoadMap(totalRoadMap);
}

int findLowestCostNeighbor(int source, int last_visited)
{
    int idx;
    int minCost = 0;
    int nextCity = -1;

    for (idx = 0; idx < NUMBER_CITIES; idx++) {
        int cost = adjacency_matrix[source][idx];

        if ((cost != 0) && (idx != last_visited) && ((minCost == 0) || (cost < minCost))) {
            minCost = cost;
            nextCity = idx;
        }
    }

    return nextCity;
}

int RouteSearch(int source, int destination, struct RoadMap **head, struct RoadMap **totalRoadMap)
{
    int cost;
    int last_visited;
    int idxNextSource;

    if ((source < 0) || (source >= NUMBER_CITIES) || (destination < 0) || (destination >= NUMBER_CITIES)) {
        return 0;
    }

    if (*head == NULL) {
        addToRoadMap(head, totalRoadMap, source, 0);
    }

    if (source == destination) {
        return 0;
    }

    cost = adjacency_matrix[source][destination];

    if (cost != 0) {
        addToRoadMap(head, totalRoadMap, destination, cost);
        return cost;
    }

    last_visited = getPreviousCity(*head);
    idxNextSource = findLowestCostNeighbor(source, last_visited);

    if (idxNextSource == -1) {
        return 0;
    }

    cost = adjacency_matrix[source][idxNextSource];
    addToRoadMap(head, totalRoadMap, idxNextSource, cost);

    return RouteSearch(idxNextSource, destination, head, totalRoadMap) + cost;
}
