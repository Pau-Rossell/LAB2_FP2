# Project 2 — Group 19 project instructions

## 1. Graph & RouteSearch
- Read adjacency matrix from the header file:
    DATA_STRUCTURE:
    |0|1|...|N|
    |---|---|---|---|
    |1||||
    |...||||
    |N||||
    Symmetric matrix and 0 on the diagonal (no traveling from city to city)
- Implement `RouteSearch(source, destination, roadmap)`:
  - Check for direct connection first → take it
  - Otherwise pick the lowest-cost neighbor
  - Track visited cities to prevent infinite loops

## 2. Road Map (Linked List)
- Implement `addToRoadMap()` → append city + cumulative cost
- Implement `printRoadMap()` → call it every time you add a city
- Implement `deleteAllRoadMap()` → free all nodes, called once at end

## 3. Ancestors' Tree
- With DFS
- Root = your parents
- Recurse down mother's side fully first, then father's side
- Each visit → call `RouteSearch` to travel to that city

- With BFS
- Use a queue to process nodes level by level
- Visit mother's side then father's side at each level
- Each visit → call `RouteSearch` to travel to that city

## 4. Report
- Document your design decisions as you go
- Note bugs encountered and how you solved them