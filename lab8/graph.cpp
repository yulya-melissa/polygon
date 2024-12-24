#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include <math.h>
#include <utility> 
#include <sstream>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

struct Node {
   int node_id; // Данные узла 
   double lon, lat;
   std::vector<std::pair<Node*, double>> neibs; // Вектор указателей на смежные узлы

   Node(int value) : node_id(value) {} // Конструктор узла (py: __init__())
};

struct Graph {
   std::vector<Node*> nodes; // Вектор указателей на узлы графа
   double last_daixtra_weight; // Для вывода расстояния пути по алгоритмам
   unordered_map<string, Node*> cash_nodes; // Словарь для быстрого получения ключа по координатам
   

    ~Graph() {
        for (auto& node : nodes) {
            delete node;
        }
    }

    int add_cash_node(Node* addr){
        stringstream s;
        string cash_value;
        s << addr->lon << addr->lat;
        s >> cash_value; // Преобразуем координатьы в строку
        cash_nodes[cash_value] = addr; // Добавляем указатель по координам
        return 1;
   }

    struct compareWays{
        // Структура для функции сравнения пар в алгоритмах дейкстра и А*
        bool operator()(const pair<Node*, double>& p1, const pair<Node*, double>& p2){
            return p1.second > p2.second;
        }
    };

    Node* find_closest_node(double lon, double lat) {
    // Функция поиска ближайшего к координатам узла в графе 
    double min_distance = 99999;
    Node* node_founded = nullptr;

    for (auto node : nodes) {
        // Проходимся по всем вершинам и находим минимальное расстояние от данной
        double distance = pow(node->lat - lat, 2) + pow(node->lon - lon, 2);
        if (distance < min_distance) {
            min_distance = distance;
            node_founded = node;
        }
    }
    return node_founded;
   }

    Node* find_node(double lon, double lat) {
        stringstream s;
        string cash_value;
        s << lon << lat;
        s >> cash_value; // Переводим значение координат в строку

        unordered_map<string, Node*>::const_iterator got = cash_nodes.find(cash_value);
        // Создаем объект, который будет хранить данные о том, существует ли такое значение узла

        if (got == cash_nodes.end()) {
            return nullptr; // Если узел не найден возвращаем пустой указатель
        } else {
            return got->second; //Если найден возвращаем указатель по ключу
        }
        
    }

    int read_graph_from_file(string link) {

    std::ifstream fin(link); // Открываем файл
    if (!fin.is_open()) {
        std::cerr << "File doesnt open!" << std::endl; // Проверка кореектного открытия файла
    }
    string str;

    while (std::getline(fin, str)) {
        
        // Считываем файл построчно
        stringstream ss(str); 
        char cap;  // Переменная для считывания знаков
        double lon, lat;
        ss >> lon >> cap >> lat >> cap; // Записываем переменные через stringstream

        Node* find_n = find_node(lon, lat); // Находим координаты узла
        Node* new_node = nullptr;  // Указатель на узел в текущей строчке

        if (find_n == nullptr) {
            new_node = new Node(nodes.size());
            new_node->lon  = lon;
            new_node->lat  = lat;
            nodes.push_back(new_node);
            add_cash_node(new_node);
        } // Если узел не нашли, создаем новый
        else {
            new_node = find_n;
        } // Иначе запоминаем указатель на узел

        while(true){
            // В цикле считываем и добовдляем соседей, с которым связан текущий узел
            char cap;
            double lon1, lat1, weight;
            ss >> lon1 >> cap >> lat1 >> cap >> weight >> cap; 
            if (ss.eof()) break; 

            Node* find_n = find_node(lon1, lat1);
            Node* new1_node = nullptr; // Указатель на рассматриваемый соседний узел

            if (find_n == nullptr) {
                new1_node = new Node(nodes.size());
                new1_node->lon  = lon1;
                new1_node->lat  = lat1;
                nodes.push_back(new1_node);
                add_cash_node(new1_node);
            } // Если узел не нашли, создаем новый
            else {
                new1_node = find_n;
            }
            new_node->neibs.push_back(make_pair(new1_node, weight)); // Добавляем этот узел в список соседей текущего узла
            new1_node->neibs.push_back(make_pair(new_node, weight)); // Добавляем текущий узел в список соседей этого узла
            // Необходимо для того, чтобы граф бл неориентированный
        }
    }
    fin.close();
    return 1;
   }

    int print_graph() {
        // Метод для отображения графа небольших размеров
        // Проходимся псоелдовательно по всем узлам в графе и выводим информацию о них
        for (const auto& node : nodes) {
            std::cout << "Node " << node->node_id << " Coord: " << node->lon << ", " << node->lat << " " << "Neibs:\n";
            for (const auto& neib : node->neibs) {
                std::cout << "  - Node " << neib.first->node_id << " Coord: " << neib.first->lon 
                          << ", " << neib.first->lat << " with weight: " 
                          << neib.second << "\n";
            }
        }
        return 0;
    }

    vector<Node*> bfs(Node* start, Node* end) {
        // Метод поска пути между вершинами по принципу bfs
        std::queue<Node*> q; // Используем очередь для BFS
        q.push(start);

        std::vector<bool> visited(nodes.size(), false);
        visited[start->node_id] = true; // Отмечаем начальный как посещенный

        std::unordered_map<Node*, Node*> previous; // Вектор для сохрания предыдуго пути
        std::vector<Node*> path;

        while (!q.empty()) {
            Node* current = q.front(); // Берем узел из начала очереди
            q.pop();

            if (current == end) {
                // Если нашли нужный узел
                double s_weight;
                Node* curr = end;
                while (curr != start){
                    // Воссоздаем путь до вершины в обратном порядке
                    path.push_back(curr);
                    for (auto neib : curr->neibs) {
                        // Для подсчета веса(длины) пути
                        if (neib.first == previous[curr]) {
                            s_weight += neib.second;
                            break;
                        }
                    }
                    curr = previous[curr];
                }
                last_daixtra_weight = s_weight;
                path.push_back(start);
                return path;
            };
            for (auto neighbor : current->neibs) {
                if (!visited[neighbor.first->node_id]) {
                    // Если этого узла еще не было
                    q.push(neighbor.first); // Добавляем соседа в очередь
                    visited[neighbor.first->node_id] = true; // Отмечаем соседа как посещенный
                    previous[neighbor.first] = current; // Записывем как мы дошли до соседа
                }
            }
        }
        return {};
    }

    vector<Node*> dfs(Node* start, Node* end) {

        std::stack<Node*> s; // Стек для обхода
        s.push(start);

        std::vector<bool> visited(nodes.size(), false);
        visited[start->node_id] = true; // Отмечаем начальный узел как посещенный

        std::unordered_map<Node*, Node*> previous; 
        std::vector<Node*> path;

        while (!s.empty()) {
            Node* current = s.top(); // Берем узел из начала очереди
            s.pop();

            if (current == end) {
                // Если нашли нужный узел
                Node* curr = end;
                double s_weight;
                while (curr != start){
                    // Воссоздаем путь до вершины в обратном порядке
                    path.push_back(curr);
                    for (auto neib : curr->neibs) {
                        if (neib.first == previous[curr]) {
                            // Подсчитываем вес(длину) пути
                            s_weight += neib.second;
                            break;
                        }
                    }
                    curr = previous[curr];
                }
                path.push_back(start);
                last_daixtra_weight = s_weight;
                return path;
            };

            for (auto neighbor : current->neibs) {
                if (!visited[neighbor.first->node_id]) {
                    // Если этого узла еще не было
                    s.push(neighbor.first); // Добавляем соседа в очередь
                    visited[neighbor.first->node_id] = true; // Отмечаем соседа как посещенный
                    previous[neighbor.first] = current;  // Запысываем из какого узла мы попали в этот
                }
            }
        }
        return {};
    }

    vector<Node*> daixtra(Node* start, Node* end) {

       unordered_map<int, double> distances;  // Хранилище для минимальных расстояний
       for (Node* node : nodes) distances[node->node_id] = node->node_id == start->node_id ? 0 : -1;
       // Заполняем вектор -1 везде, кроме начального узла

       unordered_map<Node*, Node*> previous;  // Хранилище для предыдущих узлов на пути
       priority_queue<pair<Node*, double>, vector<pair<Node*, double>>, compareWays> pq; // Приоритетная очередь минимальных расстояний
       pq.push(make_pair(start, 0));  // Добавляем начальный элемент в очередь
       vector<Node*> path; // Вектор пути от стартовой вершины до финиша 

       while (!pq.empty()) {
           pair<Node*, double> current = pq.top(); // Берем пару из очереди
           pq.pop();

           if (current.first == end) {
            // Если достигли конечной вершины
            Node* curr = end;
            while (curr != start){
                // Воссоздаем путь до вершины в обратном порядке
                path.push_back(curr);
                curr = previous[curr];
            } // Записывваем все ребра, по которым прошлись в путь
            path.push_back(start);
            last_daixtra_weight = distances[end->node_id]; // Для вывода веса(длины) пути
            return path; 

           } 

           for (auto neib : current.first->neibs) { 
               // Проходимся по соседям текущей вершины
               double newDistance = distances[current.first->node_id] + neib.second; // Вычисляем новое расстояние

               double neigborDistance = distances[neib.first->node_id]; // Записываем текущее расстояние

               if (neigborDistance == -1 || newDistance < neigborDistance) {
                // Если текущее расстояние больше нового 
                distances[neib.first->node_id] = newDistance;
                previous[neib.first] = current.first;
                pq.push(make_pair(neib.first, newDistance)); 
                // Обновить distance, путь в previous и добавить в очередь neighbor
               }
            }
       }

       return {}; // Возвращаем пустой путь, если путь не найден
   }

    double dist(Node* node1, Node* node2) {
        return sqrt(pow(node1->lat - node2->lat , 2) + pow(node1->lon - node2->lon, 2));
    }

    vector<Node*> evristik(Node* start, Node* end) {

       unordered_map<int, double> distances;  // Хранилище для минимальных расстояний
       for (Node* node : nodes) distances[node->node_id] = node->node_id == start->node_id ? 0 : -1;
       // Заполняем вектор -1 везде, кроме начального узла
       unordered_map<int, double> evr_distances;  // Хранилище для минимальных расстояний с эврирстикой
       for (Node* node : nodes) evr_distances[node->node_id] = node->node_id == start->node_id ? 0 : -1;
       // Заполняем вектор -1 везде, кроме начального узла

       unordered_map<Node*, Node*> previous;  // Хранилище для предыдущих узлов на пути
       priority_queue<pair<Node*, double>, vector<pair<Node*, double>>, compareWays> pq; // Приоритетная очередь минимальных расстояний
       pq.push(make_pair(start, 0));
       vector<Node*> path; // Вектор пути до конечной вершины

       while (!pq.empty()) {
           pair<Node*, double> current = pq.top(); // Текущая пара рассматриваемых элементов
           pq.pop();

           if (current.first == end) {
            // Если достигли конечной вершины
            Node* curr = end;
            while (curr != start){
                // Воссоздаем путь до вершины в обратном порядке
                path.push_back(curr);
                curr = previous[curr];
            } // Записывваем все ребра, по которым прошлись в путь
            path.push_back(start);
            last_daixtra_weight = distances[end->node_id];
            return path;

           } // Если достигли конца

           for (auto neib : current.first->neibs) {
               // Проходимся по соседям текущей вершины
               double newDistance = distances[current.first->node_id] + neib.second;

               double neigborDistance = evr_distances[neib.first->node_id];

               double evr = dist(start, neib.first); // Вычисляем эвристику

               if (neigborDistance == -1 || newDistance + evr < neigborDistance) {
                // Если текущее расстояние с эвристикойбольше нового с эвристикой
                distances[neib.first->node_id] = newDistance;
                evr_distances[neib.first->node_id] = newDistance + evr;
                previous[neib.first] = current.first;
                pq.push(make_pair(neib.first, newDistance + evr)); 
                // Обновить distance, evr_distance, путь в previous и добавить в очередь neighbor
               }
           }
       }

       return {}; // Возвращаем пустой путь, если путь не найден
   }
};


int main(){
    Graph g;
    g.read_graph_from_file("spb_graph.txt");
    std::vector<Node*> ans;
    double lon_coord = 30.306542; //30.306542, 60.000436
    double lat_coord = 60.000436;
    //g.print_graph();
    clock_t start1 = clock();
    ans = g.bfs(g.find_closest_node(lon_coord, lat_coord), g.find_closest_node(30.308108, 59.957238));
    clock_t end1 = clock();
    double seconds1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    cout << "bfs: nodes " << ans.size() << ", weight " << g.last_daixtra_weight << ", time " << seconds1 << "\n" ;

    clock_t start2 = clock();
    ans = g.dfs(g.find_closest_node(lon_coord, lat_coord), g.find_closest_node(30.308108, 59.957238));
    clock_t end2 = clock();
    double seconds2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    cout << "dfs: nodes " << ans.size()  << ", weight " << g.last_daixtra_weight << ", time " << seconds2 << "\n";

    clock_t start3 = clock();
    ans = g.daixtra(g.find_closest_node(lon_coord, lat_coord), g.find_closest_node(30.308108, 59.957238));
    clock_t end3 = clock();
    double seconds3 = (double)(end3 - start3) / CLOCKS_PER_SEC;
    cout << "daixtra: nodes " << ans.size() << ", weight " << g.last_daixtra_weight << ", time " << seconds3 << "\n";
    

    clock_t start4 = clock();
    ans = g.evristik(g.find_closest_node(lon_coord, lat_coord), g.find_closest_node(30.308108, 59.957238));
    clock_t end4 = clock();
    double seconds4 = (double)(end4 - start4) / CLOCKS_PER_SEC;
    cout << "evristik: nodes " << ans.size() << ", weight " << g.last_daixtra_weight << ", time " << seconds4 << "\n";


    //g.print_graph();
    //ans = g.bfs(g.find_node(1, 1), g.find_node(6, 5));
    // cout << "bfs " << ans.size() << " weight " << g.last_daixtra_weight << "\n";
    // for(auto k : ans) {
    //     cout << " " << k->node_id;
    // }
    // cout << "\n";
    //ans = g.dfs(g.find_node(1, 1), g.find_node(6, 5));
    // cout << "dfs " << ans.size()  << " weight " << g.last_daixtra_weight << "\n";
    // for(auto k : ans) {
    //     cout << " " << k->node_id;
    // }
    // cout << "\n";
    //ans = g.daixtra(g.find_node(1, 1), g.find_node(6, 5));
    // cout << "daixtra " << ans.size() << " weight " << g.last_daixtra_weight << "\n";
    // for(auto k : ans) {
    //     cout << " " << k->node_id;
    // }
    //cout << g.find_closest_node(5, 5)->node_id;
    //cout << "\n" << g.last_daixtra_weight;
}