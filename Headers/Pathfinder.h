#ifndef FANCOURIER_PATHFINDER_H
#define FANCOURIER_PATHFINDER_H
#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include "Map_Building/Map.h"
#include "Agent.h"

using namespace std;

/*
 *Algoritmul ales este A*
 *Tehnic vorbin, pentru dimensiunile relativ mici cu care lucram si faptul ca nu avem "trafic" propriu-zis
 *Dijkstra ar fi mai bun (mult mai lent, dar garanteaza shortest path)
 *Cu toate astea, am ales A* din 2 motive:
 *1) Pe viitor, daca planuiesc sa continui proiectul pe cont propriu, vreau sa simulez traficul
 *caz in care sa am A* deja implementat e un bonus
 *2) Se foloseste foarte mult in industrie
 */
struct Node
{
    POINT p;
    int currentCost; //dist de start
    int estimatedCost; //dist estimatat
    int fCost() const { return currentCost + estimatedCost; }

    //operator pt Prioryty queue
    bool operator>(const Node& other) const {
        return fCost() > other.fCost();
    }
};

//algoritm luat dintr-o librarie publica si adaptat pentru infrastructura proiectului meu
class Pathfinder
{
public:

    static vector<POINT> findPath(POINT start, POINT end, const Map* map, MOVEMENTMODE mode)
    {
        int i, tentativeG;
        int nx, ny;

        bool isWalkable;


        //Check ca clientul sa fie accesibil (teoretic nu e necesar, dar better safe than sorry)
        if (mode == MOVEMENTMODE::GROUND && map->grid[end.x][end.y] == CELLTYPE::WALL)
        {
            return {};
        }

        // vectori pt directii (Sus, Jos, Stanga, Dreapta)
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        //structuri A*
        priority_queue<Node, vector<Node>, greater<Node>> openSet;

        //mapare pentru parintit (folosim matrici de vectori pt simplitate si memori safety)
        vector<vector<POINT>> parent(map->rows, vector<POINT>(map->cols, {-1, -1}));
        vector<vector<int>> gScore(map->rows, vector<int>(map->cols, 999999));

        //nod de start
        gScore[start.x][start.y] = 0;
        openSet.push({start, 0, calculateH(start, end)});

        while (!openSet.empty())
        {
            Node current = openSet.top();
            openSet.pop();

            POINT u = current.p;

            //check daca am ajuns la destinatie
            if (u.x == end.x && u.y == end.y)
            {
                return reconstructPath(parent, end);
            }

            // Vecinii
            for (i = 0; i < 4; i++)
            {
                nx = u.x + dx[i];
                ny = u.y + dy[i];

                //check pt limitelel hartii
                if (nx >= 0 && nx < map->rows && ny >= 0 && ny < map->cols) {

                    //abordam diferenta intre drone si restu
                    isWalkable = true;
                    if (mode == MOVEMENTMODE::GROUND)
                    {
                        if (map->grid[nx][ny] == CELLTYPE::WALL)
                        {
                            isWalkable = false;
                        }
                    }

                    if (isWalkable)
                    {
                        tentativeG = gScore[u.x][u.y] + 1; //consideream costul unui pas ca fiind 1

                        if (tentativeG < gScore[nx][ny])
                        {
                            parent[nx][ny] = u;
                            gScore[nx][ny] = tentativeG;
                            openSet.push({ {nx, ny}, tentativeG, calculateH({nx, ny}, end) });
                        }
                    }
                }
            }
        }
        //nu am gasit drum
        return {};
    }

private:
    // Heuristica Manhattan
    static int calculateH(POINT a, POINT b)
    {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }

    static vector<POINT> reconstructPath(const vector<vector<POINT>>& parent, POINT current)
    {
        vector<POINT> path;
        while (current.x != -1)
        {
            path.push_back(current);
            current = parent[current.x][current.y];
        }

        //drumul e invers, deci il intoarce si ignoram startul intr-uncat agentul e deja acolo
        reverse(path.begin(), path.end());
        if (!path.empty())
        {
            path.erase(path.begin()); // Scoatem pozitia curenta
        }
        return path;
    }
};



#endif //FANCOURIER_PATHFINDER_H