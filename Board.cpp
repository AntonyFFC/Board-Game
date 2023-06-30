#include "Board.h"
#include <random>
//#include <iostream>

std::tuple<int, int, int> add(std::tuple<int, int, int> a, std::tuple<int, int, int> b)
{
    return { std::get<0>(a) + std::get<0>(b) ,std::get<1>(a) + std::get<1>(b), std::get<2>(a) + std::get<2>(b) };
}

std::tuple<int, int, int> subtract(std::tuple<int, int, int> a, std::tuple<int, int, int> b)
{
    return { std::get<0>(a) - std::get<0>(b) ,std::get<1>(a) - std::get<1>(b), std::get<2>(a) - std::get<2>(b) };
}

int distance(std::tuple<int, int, int> a, std::tuple<int, int, int> b)
{
    std::tuple<int, int, int> vec = subtract(a, b);
    return (abs(std::get<0>(vec)) + abs(std::get<1>(vec)) + abs(std::get<2>(vec))) / 2;
}

bool isTupleInVector(const std::vector<std::tuple<int, int, int>>& vec, const std::tuple<int, int, int>& targetTuple) {
    for (std::tuple<int, int, int > tuple : vec) {
        if (tuple == targetTuple) {
            return true;
        }
    }
    return false;
}

float lerp(int a, int b, float t)
{
    return a + (b - a) * t;
}

std::tuple<float, float, float> cube_lerp(std::tuple<int, int, int> a, std::tuple<int, int, int> b, float t)
{
    return { lerp(std::get<0>(a), std::get<0>(b), t),
        lerp(std::get<1>(a), std::get<1>(b), t),
        lerp(std::get<2>(a), std::get<2>(b), t) };
}

std::tuple<int, int, int> cubeRound(std::tuple<float, float, float> a)
{
    int q = round(std::get<0>(a));
    int r = round(std::get<1>(a));
    int s = round(std::get<2>(a));

    float q_diff = abs(q - (std::get<0>(a)));
    float r_diff = abs(r - (std::get<1>(a)));
    float s_diff = abs(s - (std::get<2>(a)));

    if (q_diff > r_diff && q_diff > s_diff)
        q = -r - s;
    else if (r_diff > s_diff)
        r = -q - s;
    else
        s = -q - r;

    return { q, r, s };
}

std::vector <std::tuple<int, int, int>> Board::linedraw(std::tuple<int, int, int> a, std::tuple<int, int, int> b)
{
    int N = distance(a, b);
    std::vector <std::tuple<int, int, int>> results;
    for (int i = 0; i <= N; i++)
    {
        std::tuple<int, int, int> coords = cubeRound(cube_lerp(a, b, 1.0 / N * i));
        if (coords == a || !hexDict.count(coords)) {
            continue;
        }
        results.push_back(coords);
    }
    return results;
}

Board::Board(int Rows, int Columns, float HSize)
    :numRows(Rows), numCols(Columns), hexSize(HSize)
{
    numHexagons = numRows * numCols;
    vertSpacing = 86.6f;
    horizSpacing = 75.f;
    xOffset = 30.f;
    yOffset = 20.f;
    highlighted.push_back(std::vector<std::tuple<int, int, int>>());
    highlighted.push_back(std::vector<std::tuple<int, int, int>>());

    boardPreperation();
}

Board::~Board()
{
    std::vector<std::tuple<int, int, int>> keysToDelete;
    for (const auto& pair : hexDict) {
        keysToDelete.push_back(pair.first);
    }

    for (const auto& key : keysToDelete) { //this is so that I don't iterate and delete the same thing
        auto iter = hexDict.find(key);
        if (iter != hexDict.end()) {
            delete iter->second;
            hexDict.erase(iter);
        }
    }
}

class DirectionVectors {
public:
    static std::vector<std::tuple<int, int, int>> directionVectors;

    static std::vector<std::tuple<int, int, int>> GetDirectionList() {
        return directionVectors;
    }
};

std::vector<std::tuple<int, int, int>> DirectionVectors::directionVectors = {
    {1, 0, -1}, {1, -1, 0}, {0, -1, 1},
    {-1, 0, 1}, {-1, 1, 0}, {0, 1, -1}
};

std::vector <std::tuple<int, int, int>> Board::getNeighbours(std::tuple<int, int, int> hexCoordinates)
{
    if (!hexDict.count(hexCoordinates))
        return std::vector<std::tuple<int, int, int>>();

    if (hexNeighboursDict.count(hexCoordinates))
        return hexNeighboursDict[hexCoordinates];

    hexNeighboursDict[hexCoordinates] = std::vector<std::tuple<int, int, int>>();
    for (const auto& direction : DirectionVectors::GetDirectionList()) //this adds the neighbours of the new hex to the dictionary
    {
        if (hexDict.count(add(hexCoordinates, direction)))
        {
            hexNeighboursDict[hexCoordinates].push_back(add(hexCoordinates, direction));
        }
    }

    return hexNeighboursDict[hexCoordinates];
}

std::vector < std::tuple<int, int, int>> Board::getInRange(std::tuple<int, int, int> hexCoordinates, int dist, int minDistance)
{
    std::vector < std::tuple<int, int, int>> hexes;     
    if (dist == 1) // for efficiency I used the neighbours method with 1 distances
    {
        if (minDistance==0)
            hexes.push_back(hexCoordinates);
        std::vector<std::tuple<int, int, int>> neighbours = getNeighbours(hexCoordinates);
        hexes.insert(hexes.end(), neighbours.begin(), neighbours.end());
        return hexes;
    }

    for (int x = -dist; x <= dist; x++)
    {
        for (int y = std::max(-dist, -x - dist); y <= std::min(dist, dist - x); y++)
        {
            int z = -x - y;
            std::tuple<int, int, int> newPos = add(hexCoordinates, { x, y, z });
            if (distance(hexCoordinates, newPos) < minDistance)
                continue;
            hexes.push_back(newPos);
        }
    }
    return hexes;
}

std::vector < std::tuple<int, int, int>> Board::getReachable(std::tuple<int, int, int> start, int movement)
{
    std::vector < std::tuple<int, int, int>> visited;
    std::vector<std::vector < std::tuple<int, int, int>>> fringes;
    fringes.push_back(std::vector< std::tuple<int, int, int>>());
    fringes[0].push_back(start);

    for (int k = 1; k <= movement; k++)
    {
        fringes.push_back(std::vector< std::tuple<int, int, int>>());
        for (std::tuple<int, int, int> coordinates : fringes[k - 1])
        {
            std::vector < std::tuple<int, int, int>> neighbours = getNeighbours(coordinates);
            for (std::tuple<int, int, int> neighbour : neighbours)
            {
                if (!hexDict[neighbour]->isBlocking() && !isTupleInVector(visited, neighbour) && neighbour != start)
                {
                    visited.push_back(neighbour);
                    fringes[k].push_back(neighbour);
                    hexDict[neighbour]->setPawnDist(k);
                }
            }
        }
    }
    return visited;
}

std::vector < std::tuple<int, int, int>> Board::getInView(std::tuple<int, int, int> start, int dist, int minDist)
{
    bool skipOuterLoop = false;
    std::vector <std::tuple<int, int, int>> results;
    std::vector < std::tuple<int, int, int>> inRange = getInRange(start, dist, minDist);
    for (std::tuple<int, int, int> coordinates : inRange)
    {
        std::vector < std::tuple<int, int, int>> inLine = linedraw(start, coordinates);
        for (std::tuple<int, int, int> lineHex : inLine)
        {
            if (hexDict[lineHex]->isBlocking())
            {
                skipOuterLoop = true;
                break;
            }
        }

        if (skipOuterLoop) {
            skipOuterLoop = false;
            continue;
        }

        for (std::tuple<int, int, int> hex : inLine)
        {
            if (!isTupleInVector(results, hex) && isTupleInVector(inRange, hex))
            {
                results.push_back(hex);
            }
        }
    }
    return results;
}

class ObjectCoordinates {
public: 
    static std::vector < std::vector<std::tuple<int, int, int>>> rockCoordinates;
    static std::vector<std::tuple<int, int, int>> deleteCoordinates;
    static std::vector<std::tuple<int, int, int>> startCoordinates;

    static std::vector<std::tuple<int, int, int>> getRockCoordinates() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, rockCoordinates.size() - 1);
        int random_index = dis(gen);
        return rockCoordinates[random_index];
    }

    static std::vector<std::tuple<int, int, int>> getdeleteCoordinates() {
        return deleteCoordinates;
    }

    static std::vector<std::tuple<int, int, int>> getStartCoordinates() {
        return startCoordinates;
    }
};

std::vector<std::vector<std::tuple<int, int, int>>> ObjectCoordinates::rockCoordinates = {
    {{1, -1, 0}, {1, 0, -1}, {-1, -1, 2}, {0, -2, 2}, {2, -3, 1}, {3, -3, 0}, {3, 0, -3}, {2, 1, -3}, {0, 2, -2}, {-1, 2, -1}, {-2, -3, 5}, {4, -6, 2}, {4, 2, -6}, {-2, 5, -3}},
    {{-2, 0, 2}, {-1, -2, 3}, {0, -2, 2}, {1, -4, 3}, {2, -4, 2}, {2, -3, 1}, {3, -3, 0}, {-1, 2, -1}, {0, 2, -2}, {0, 3, -3}, {1, 3, -4}, {2, 1, -3}, {3, 1, -4}, {4, -1, -3}},
    {{-2, -1, 3}, {-1, -1, 2}, {-2, 3, -1}, {-1, 2, -1}, {0, 1, -1}, {1, 2, -3}, {1, 3, -4}, {3, 0, -3}, {4, 0, -4}, {2, -2, 0}, {3, -3, 0}, {4, -4, 0}, {1, -3, 2}, {1, -4, 3}},
    {{-4, -1, 5}, {-2, -3, 5}, {-4, 5, -1}, {-2, 5, -3}, {-1, 3, -2}, {1, 3, -4}, {3, 1, -4}, {4, 2, -6}, {6, 0, -6}, {-1, -2, 3}, {1, -4, 3}, {3, -4, 1}, {4, -6, 2}, {6, -6, 0}},
    {{-1, 0, 1}, {-1, 1, 0}, {-1, -2, 3}, {0, -2, 2}, {-1, 3, -2}, {0, 2, -2}, {1, 3, -4}, {2, 1, -3}, {3, 1, -4}, {3, -1, -2}, {3, -2, -1}, {2, -3, 1}, {3, -4, 1}, {1, -4, 3}},
    {{-1, -1, 2}, {-1, -2, 3}, {-1, 2, -1}, {-1, 3, -2}, {-2, -3, 5}, {-2, 5, -3}, {1, 3, -4}, {1, -4, 3}, {3, -4, 1}, {3, -3, 0}, {4, -6, 2}, {3, 0, -3}, {3, 1, -4}, {4, 2, -6}},
    {{-2, -1, 3}, {-2, 2, 0}, {-1, 2, -1}, {-1, 1, 0}, {1, 2, -3}, {1, 3, -4}, {2, 2, -4}, {0, -1, 1}, {1, -1, 0}, {1, -2, 1}, {3, -4, 1}, {3, -2, -1}, {3, -1, -2}, {4, -2, -2}},
    {{-2, -3, 5}, {0, -3, 3}, {0, -2, 2}, {2, -4, 2}, {2, -3, 1}, {1, -1, 0}, {1, 0, -1}, {0, 2, -2}, {0, 3, -3}, {2, 1, -3}, {2, 2, -4}, {4, 2, -6}, {-2, 5, -3}, {4, -6, 2}},
    {{1, 3, -4}, {1, 2, -3}, {-1, 1, 0}, {3, -1, -2}, {4, -2, -2}, {3, -2, -1}, {2, -2, 0}, {-2, 1, 1}, {-1, 0, 1}, {0, -1, 1}, {0, -3, 3}, {-1, -3, 4}, {2, -4, 2}, {3, -5, 2}},
    {{-2, 0, 2}, {-1, 0, 1}, {-1, 1, 0}, {-1, 3, -2}, {1, 1, -2}, {2, 1, -3}, {3, 1, -4}, {4, -1, -3}, {3, -1, -2}, {3, -2, -1}, {3, -4, 1}, {1, -2, 1}, {0, -2, 2}, {-1, -2, 3}}
};
std::vector<std::tuple<int, int, int>> ObjectCoordinates::deleteCoordinates = {
    {-8, -2, 10}, {-8, -1, 9}, {-8, 0, 8}, {-8, 7, 1}, {-8, 1, 7}, {-8, 8, 0}, 
    {-8, 9, -1}, {-8, 10, -2}, {-7, 0, 7}, {-7, -1, 8}, {-7, -2, 9}, {-7, 7, 0}, 
    {-7, 8, -1}, {-7, 9, -2}, {-7, 10, -3}, {-6, 7, -1}, {-6, 8, -2}, {-6, 9, -3}, 
    {-5, 7, -2}, {-5, 8, -3}, {-5, 9, -4}, {-4, 7, -3}, {-4, 8, -4}, {-3, 7, -4}, 
    {-3, 8, -5}, {-2, 7, -5}, {-1, 7, -6}, {-6, -1, 7}, {-6, -2, 8}, {-6, -3, 9}, 
    {-5, -2, 7}, {-5, -3, 8}, {-4, -3, 7}, {-4, -4, 8}, {-3, -4, 7}, {-2, -5, 7}, 
    {10, -11, 1}, {10, -10, 0}, {10, -9, -1}, {10, -8, -2}, {10, 1, -11}, 
    {10, 0, -10}, {10, -1, -9}, {10, -2, -8}, {9, -1, -8}, {8, 0, -8}, {7, 1, -8}, 
    {6, 2, -8}, {5, 3, -8}, {4, 4, -8}, {5, 4, -9}, {6, 3, -9}, {7, 2, -9}, {7, 3, -10}, 
    {8, 2, -10}, {8, 1, -9}, {9, 0, -9}, {9, 1, -10}, {9, 2, -11}, {9, -8, -1}, {8, -8, 0}, 
    {7, -8, 1}, {6, -8, 2}, {5, -8, 3}, {4, -8, 4}, {6, -9, 3}, {7, -9, 2}, {8, -10, 2}, 
    {8, -9, 1}, {9, -10, 1}, {9, -9, 0}, {3, 5, -8}, {1, 6, -7}
};
std::vector<std::tuple<int, int, int>> ObjectCoordinates::startCoordinates = {
    {10, -7, -3}, {9, -6, -3}, {9, -5, -4}, {9, -4, -5}, {9, -3, -6}, {10, -3, -7}, 
    {10, -4, -6}, {10, -5, -5}, {10, -6, -4}, {-8, 2, 6}, {-7, 2, 5}, {-7, 3, 4}, 
    {-7, 4, 3}, {-7, 5, 2}, {-8, 6, 2}, {-8, 5, 3}, {-8, 4, 4}, {-8, 3, 5}
};

void Board::drawBoard(sf::RenderTarget& target, bool isShift)
{
    for (auto& pair : hexDict) {
        pair.second->draw(target, isShift);
    }
}

void Board::deleteHexagons()
{
    for (const auto& coordinates : ObjectCoordinates::getdeleteCoordinates())
    {
        hexDict.erase(coordinates);
    }
}

void Board::setStart()
{
    for (const auto& coordinates : ObjectCoordinates::getStartCoordinates())
    {
        hexDict[coordinates]->setStart(true);
    }
}

void Board::setRocks()
{
    for (const auto& coordinates : ObjectCoordinates::getRockCoordinates())
    {
        hexDict[coordinates] = new Rock(coordinates);
    }
}

void Board::setBases()
{
    std::tuple<int, int, int> base1 = {-4,2,2};
    std::tuple<int, int, int> base2 = { 6,-3,-3};
    
    hexDict[base1]->setBase(true);
    hexDict[base2]->setBase(true);
}

void Board::boardPreperation()
{
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            Grass hexagon;

            float x = j * horizSpacing * hexSize + xOffset;
            float y = i * vertSpacing * hexSize + yOffset + (j % 2) * vertSpacing * hexSize / 2;

            hexagon.setPos(x, y);
            /*std::cout << "x:" << x << " y:" << y << "\n";
            std::cout << "Hx:" << hexagon.x << " Hy:" << hexagon.y << " Hz:" << hexagon.z << "\n";*/

            hexDict[hexagon.getCubeCoords()] = new Grass(hexagon.getCubeCoords());
        }
    }
    deleteHexagons();
    setStart();
    setRocks();
    setBases();
}

void Board::clearHighlight()
{
    for (std::tuple<int, int, int> hex : highlighted[0])
    {
        hexDict[hex]->setHighlight(false, 0);
    }
    highlighted[0].clear();
}

void Board::setWall(std::tuple<int, int, int> coords)
{
    hexDict[coords] = new Wall(coords);
}

void Board::setGrass(std::tuple<int, int, int> coords)
{
    hexDict[coords] = new Grass(coords);
}