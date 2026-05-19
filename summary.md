# Project 2 — Quick Reference

## 1. Setup
- Include `small.h` for basic case
- Use `#ifdef` for small/medium/large support
- Compile with `gcc -DSMALL -o exec source.c`

## 2. Graph & RouteSearch
- Read adjacency matrix from the header file
- In `RouteSearch(source, destination, roadmap)`:
  - Check for direct connection first → take it
  - Otherwise pick the lowest-cost neighbor
  - Track visited cities to avoid infinite loops

## 3. Road Map (Linked List)
- Implement `addToRoadMap()` → append city + cumulative cost
- Implement `printRoadMap()` → call it **every time** you add a city
- Implement `deleteAllRoadMap()` → free all nodes, called once at end

## 4. Ancestors' Tree — DFS
- Root = your parents
- Recurse down mother's side fully first, then father's side
- Each visit → call `RouteSearch` to travel to that city

## 5. Ancestors' Tree — BFS
- Use a queue to process nodes level by level
- Visit mother's side then father's side at each level
- Each visit → call `RouteSearch` to travel to that city

## 6. Tree Display
- Print each node with indentation showing depth (`->`, `->->`, etc.)
- Show names + city for each node

## 7. Report
- Document your design decisions as you go
- Note bugs encountered and how you solved them