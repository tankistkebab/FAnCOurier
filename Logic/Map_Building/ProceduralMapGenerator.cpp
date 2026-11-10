#include "../../Headers/Map_Building/ProceduralMapGenerator.h"

ProceduralMapGenerator::ProceduralMapGenerator(int clients, int charging_stations, int randomness)
{
    this->clients = clients;
    this->charging_stations = charging_stations;
    this->randomness = randomness;

    //Seed pt rng
    random_device rd;
    rng.seed(rd());
}

void ProceduralMapGenerator::buildMap(Map &map, int startX, int startY)
{

    int i, nx, ny, directie, randomVectorIndex;
    int destX, destY, wallX, wallY;

    stack<POINT> stiva;

    //mic mare caviat cu care mi-am batut mult capul: deoarece grid e de tipul vector<vector>, memoria e orientata
    //pe randuri, asa ca accesarea se face invers decat te-ai astepta
    //x = verticala, y = orizotala, grid[x][y] e corect
    map.grid[startX][startY] = CELLTYPE::ROAD;
    stiva.push({startX, startY});

    //vectori de directii (sus jos stanga dreapta)
    int dx[] = {-2, 2 , 0 , 0}; // verticala
    int dy[] = {0, 0, -2, 2};   // orizontala

    while (!stiva.empty())
    {
        POINT curent = stiva.top();
        vector<int> vecini;

        //iteram prin vecinii vecinii, verificam conditiile si adaugam in vector
        for (i = 0; i < 4; i++)
        {
            nx = curent.x + dx[i];
            ny = curent.y + dy[i];

            if ((nx > 0) && (nx < map.rows - 1) && (ny > 0) && (ny < map.cols - 1))
            {
                if (map.grid[nx][ny] == CELLTYPE::WALL)
                {
                    vecini.push_back(i);
                }
            }
        }

        if (!vecini.empty())
        {
            //Alegem vecin random
            uniform_int_distribution<int> distribution(0, vecini.size() - 1);

            randomVectorIndex = distribution(rng);
            directie = vecini[randomVectorIndex];

            //Cal coord vecinului
            destX = curent.x + dx[directie];
            destY = curent.y + dy[directie];

            //Calc coord zidului dintre mine si vecin
            wallX = curent.x + (dx[directie] / 2);
            wallY = curent.y + (dy[directie] / 2);

            //Facem drumuri
            map.grid[destX][destY] = CELLTYPE::ROAD;
            map.grid[wallX][wallY] = CELLTYPE::ROAD;

            //Adaugam la stack
            stiva.push({destX, destY});
        }
        else
        {
            //nu mai avem vecini valizi, deci ne intoarcem
            stiva.pop();
        }
    }
    //Nota: Ar fi fost mai eficient sa folosesc o recursivitate adevarata, dar tehnic de implementat, varianta mea
    //este mai usoara de citit si gandit. Exista mult loc de improvement.
}

void ProceduralMapGenerator::addRandomRoads(Map &map)
{
    int i, j;
    uniform_int_distribution<int> dist(0, 100);
    //Iteram prin harta

    for (i = 1; i < map.rows - 1; i++)
    {
        for (j = 1; j < map.cols - 1; j++)
        {
            //Daca e zid, punem la intamplare un drum
            if (map.grid[i][j] == CELLTYPE::WALL)
            {
                if (dist(rng) < randomness)
                {
                    map.grid[i][j] = CELLTYPE::ROAD;
                }
            }
        }
    }
}

void ProceduralMapGenerator::placeObjects(Map &map, POINT start)
{
    int i, index;

    vector<POINT> freeSpots;

    index = 0;

    //Salvam doar drumurile accesibile din punctul de start al labirintului (flood fill).
    //addRandomRoads poate crea celule de drum izolate; daca am pune un client sau o statie acolo,
    //harta ar pica integrityCheck.
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    vector<vector<bool>> visited(map.rows, vector<bool>(map.cols, false));
    queue<POINT> q;
    q.push(start);
    visited[start.x][start.y] = true;

    while (!q.empty())
    {
        POINT current = q.front();
        q.pop();
        freeSpots.push_back(current);

        for (i = 0; i < 4; i++)
        {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            if (nx > 0 && nx < map.rows - 1 && ny > 0 && ny < map.cols - 1 &&
                !visited[nx][ny] && map.grid[nx][ny] == CELLTYPE::ROAD)
            {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    //Folosim shuffle pentru a amesta lista
    //Initial incercasem sa aleg pozitii la intamplpare folosind random, dar facand asta e greu sa tin cont de ce am ales
    //si ce pozitii au mai ramas
    shuffle(freeSpots.begin(), freeSpots.end(), rng);

    // Verificam daca mai e loc
    if (freeSpots.size() < static_cast<size_t>(1 + charging_stations + clients))
    {
        throw 2;
    }

    //Punem baza pe primul loc liber;
    POINT basePos = freeSpots[index++];
    map.grid[basePos.x][basePos.y] = CELLTYPE::BASE;
    map.baza = basePos;

    //Punem statiile
    for (i = 0; i < charging_stations; i++)
    {
        POINT p = freeSpots[index++];
        map.grid[p.x][p.y] = CELLTYPE::CHARGING_STATION;
        map.chargingStationsLocations.push_back(p);
    }

    //Punem clientii
    for (i = 0; i < clients; i++)
    {
        POINT p = freeSpots[index++];
        map.grid[p.x][p.y] = CELLTYPE::CLIENT;
        map.clientsLocations.push_back(p);
    }
}

void ProceduralMapGenerator::generateMap(Map &map) // Maze generation + erosion + some randomness
{
    /*
     * Principiul este simplu:
     * Umple gridul de ziduri
     * Alegem la intamplare un punct
     * cautam toti vecinii valizi (sunt in grid si sunt ziduri) la 2 punctde de noi (sus jos stnaga dreapta)
     * Alegem unul la intamplare, il facem drum pe el si punctul dintre curent si ales
     * Repetam procesul pana nu mai avem vecini valizi
     * Dupa facem backtracking si continuam procesul
     * La final, spargem niste ziduri la intamplare pentru varietate
     */
    cout << "DEBUG: Incepem generarea hartii procedurale" << endl;

    int startX, startY;

    map.grid.clear();
    map.chargingStationsLocations.clear();
    map.clientsLocations.clear();

    //diomensionam gridul
    map.grid.resize(map.rows, vector<CELLTYPE>(map.cols, CELLTYPE::WALL));

    //Alegem un pct random de plecare in grid, nu pe margine
    uniform_int_distribution<int> rowDistribution(1, map.rows - 2);
    uniform_int_distribution<int> colDistribution(1, map.cols - 2);

    startX = rowDistribution(rng);
    startY = colDistribution(rng);

    //Am ales sa sparg procesul de generare in 3 funtii penrtu a nu tine o gramada de variabile de care nu mai am nevoie
    //incarcate in memorie
    cout << "Incepem sa construim harta" << endl;
    buildMap(map, startX, startY);

    cout << "Adaugam drumuri la intamplare" << endl;
    addRandomRoads(map);

    cout << "Punem punctele de interes" << endl;
    placeObjects(map, {startX, startY});

    cout << "Verificam daca harta e valida " << endl;
    if (!map.integrityCheck())
    {
        throw 3;
    }
    //Nota: Teoretic cel putin, la cum generez harta, ea ar trebui sa fie mereu valida.
    //But better safe than sorry

    cout << "Displaying map: " << endl;
    map.displayMap();



    cout << "DEBUG: Am terminat generarea hartii procedurale" << endl;
}

