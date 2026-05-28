#include "small.h"
#include <stdio.h>
#include <stdlib.h>

struct RoadMap *head = NULL;

void addToRoadMap(int city_id){
    
    struct RoadMap *newNode = malloc(sizeof(struct RoadMap));
    newNode -> city_id = city_id;
    newNode -> total_cost = 0;
    newNode-> next = NULL;

        if (head == NULL) {
        newNode->total_cost = 0;
        head = newNode;
        return;
        }
    struct RoadMap *current = head;
    int totalCost = 0;

    while ((current -> next) != NULL) {
        totalCost = totalCost + (current -> total_cost);
        current = current -> next;
    }
    newNode -> total_cost = totalCost;
    current -> next = newNode;

}

void printRoadMap(){
    printf ("Total Road Map: \n");

    struct RoadMap *current = head;
    while (current != NULL){
        printf ("%s - ", citiesInfo[current -> city_id].city_name);
    current = current -> next;
    }
}

int RouteSearch(int source, int destination, struct RoadMap *RoadMap){
    if (RoadMap == NULL){
        addToRoadMap(source);
    
    };
int cost = adjacency_matrix[source][destination];
if (cost == 0){
    int min = 0;
    int idx = 0;
    while (!min){
        min = adjacency_matrix[source][idx];
        idx ++;
    }
    
    int idxNextSource = idx - 1;

    for (; idx<NUMBER_CITIES; idx++){
        if ((adjacency_matrix[source][idx] < min)&& (adjacency_matrix[source][idx] != 0)) {
            min = adjacency_matrix[source][idx];
            idxNextSource = idx;
        }
    }
    addToRoadMap(idxNextSource);
    cost = min;

    return (RouteSearch(idxNextSource, destination, RoadMap) + cost);

};
addToRoadMap(destination);
return cost;
}

int main() {
    
    int cost;
    cost = RouteSearch(0, 5, head);
    printRoadMap();

    cost = RouteSearch(3, 6, head);
    printRoadMap();

    cost = RouteSearch(9, 1, head);
    printRoadMap();

    return 0;
}