#include "small.h"
#include <stdio.h>
#include <stdlib.h>

struct RoadMap *head = NULL;

struct RoadMap *getLastElement(){
    struct RoadMap *tmp = head;
    while (tmp -> next != NULL){
        tmp = tmp -> next;
    }
    return tmp;
}

void addToRoadMap(int city_id, int hop_cost){
    
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

    struct RoadMap *lastElement = getLastElement();
    newNode -> total_cost = lastElement->total_cost + hop_cost;
    lastElement -> next = newNode;

}

void printRoadMap(){
    printf ("Total Road Map: \n");

    struct RoadMap *current = head;
    while (current != NULL){
        if (current -> next == NULL){
        printf ("%s", citiesInfo[current -> city_id].city_name);
        printf("  %d", current -> total_cost);
        }else{
            printf ("%s - ", citiesInfo[current -> city_id].city_name);
        }
    current = current -> next;
    }
    
    printf("\nTotal cost: %d\n", getLastElement()->total_cost);
    printf("\n");
}

int RouteSearch(int source, int destination, struct RoadMap *RoadMap){
    
    int last_visited = NUMBER_CITIES; //Mirem d'on venim per no entrar en bucle

    if (RoadMap == NULL){   // Aixo nomes quan no hi ha res a RoadMap (l'inicialitzem)
        addToRoadMap(source, 0);
        last_visited = -1;
    };

    int cost = adjacency_matrix[source][destination];
    
    if (cost != 0){         // Aixo es quan hi ha ruta directa de source a destination
        addToRoadMap(destination, cost);
        return cost;
    }

    // A partir d'aqui ens trobem en el punt en que hem de fer escala (a la ciutat mes barata)
    if (last_visited != -1){
        struct RoadMap *tmp = head;
        while (tmp->next->next != NULL){ // AIXO NO FUNCIONA QUAN NOMES HI HA UN ITEM A ROADMAP
            tmp = tmp->next;
        }
        last_visited = tmp->city_id;
    }
    
    int min = 0; // 
    int idx = 0;
    while (!min){
        if (idx != last_visited){
            min = adjacency_matrix[source][idx];
        }
    idx ++;
    }

    int idxNextSource = idx - 1;

    for (; idx<NUMBER_CITIES; idx++){
        if ((adjacency_matrix[source][idx] < min) && (adjacency_matrix[source][idx] != 0) && (idx != last_visited)) {
        min = adjacency_matrix[source][idx];
        idxNextSource = idx;
        }
    }
    cost = min;
    addToRoadMap(idxNextSource, cost);

    return RouteSearch(idxNextSource, destination, head) + cost;
};


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