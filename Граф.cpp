#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <unordered_set>

// TODO: переписать на умные указатели. Разбить на файлы
struct Edge;
struct Vertex;
struct Graf;

void find_all_paths_helper(Vertex*, Vertex*, std::vector<Vertex*>&, std::unordered_set<Vertex*>&, int);
void find_all_paths(Graf& graph, std::string start, std::string end);
void do_correct_city_name(std::string* city_name);

struct Vertex
{
    std::string city_name;
    std::vector <Edge> edges;
    Vertex(std::string city_name)
    {
        if (!city_name.empty()) // добавить проверку на пробелы
            this->city_name = city_name;
    };

    bool operator==(const Vertex& other) const
    {
        return this->city_name == other.city_name;
    }
};

struct Edge
{
    int path_length;
    Vertex* start_city, * destination_city;
    Edge(int path_length, Vertex* start_city, Vertex* destination_city)
    {
        // добавить исключения
        if (start_city != nullptr)
            this->start_city = start_city;

        if (destination_city != nullptr)
            this->destination_city = destination_city;

        if (path_length >= 0)
            this->path_length = path_length;

        this->start_city->edges.push_back(*this);
    }
};

struct Graf
{
    std::list<Vertex> cities;

    void add_city(std::string city_name)
    {
        Vertex city(city_name);
        cities.push_back(city);
    }

    void add_road(int path_length, std::string start_city_name, std::string dest_city_name)
    {
        Vertex* start_city = nullptr, * destination_city = nullptr;

        start_city = find_city(start_city_name);
        destination_city = find_city(dest_city_name);

        if (start_city != nullptr && destination_city != nullptr)
        {
            Edge road1(path_length, start_city, destination_city);
            Edge road2(path_length, destination_city, start_city);
        }
        else
        {
            // TODO: добавить исключение
        }
    }

    Vertex* find_city(std::string city)
    {
        for (auto& i : cities)
        {
            if (i.city_name == city)
                return &i;
        }
        return nullptr;
    }

    void print_сityes()
    {
        for (auto& city : cities)
        {
            std::cout << " " << city.city_name << " ";
        }
        std::cout << std::endl;
    }

    void print_roads()
    {
        for (auto& city : cities)
        {
            std::cout << city.city_name << " roads:" <<std::endl;
            for (auto& road : city.edges)
            {
                std::cout << road.start_city->city_name << " -> ";
                std::cout << road.destination_city->city_name << " ";
                std::cout << road.path_length << std::endl;
            }
        }
    }

    void delete_road(std::string first_city, std::string second_city)
    {
        Vertex* start_city, * dest_city;
        start_city = find_city(first_city);
        dest_city = find_city(second_city);

        if (start_city != nullptr && dest_city != nullptr)
        {
            for (int i = 0; i < start_city->edges.size(); i++)
            {
                if (start_city->edges[i].destination_city == dest_city)
                {
                    start_city->edges.erase(start_city->edges.begin() + i);
                }
            }

            for (int i = 0; i < dest_city->edges.size(); i++)
            {
                if (dest_city->edges[i].destination_city == start_city)
                {
                    dest_city->edges.erase(dest_city->edges.begin() + i);
                }
            }
        }
    }

    void delete_city(std::string city_name)
    {
        Vertex* city = find_city(city_name);
        if (city != nullptr)
        {
            for (auto& road : city->edges)
            {
                delete_road(road.start_city->city_name, road.destination_city->city_name);
            }

            auto it = std::find(cities.begin(), cities.end(), *city); // находим элемент со значением 3
         //   if (it != cities.end()) {
                cities.erase(it); // удаляем элемент из списка
           // }
        }
    }


};

int main()
{
    setlocale(LC_ALL, "");
    Graf railways;
  
    while (true)
    {

        int path_length, a;
        std::string city, first_city, second_city;

        std::cout << "|------------------- МЕНЮ -------------------|\n";
        std::cout << "|1 - Добавить город                          |\n";
        std::cout << "|2 - Добавить путь                           |\n";
        std::cout << "|3 - Вывести все города                      |\n";
        std::cout << "|4 - Вывести все возможные маршруты          |\n";
        std::cout << "|5 - Вывести все возможные маршруты из А в В |\n";
        std::cout << "|6 - Удалить город                           |\n";
        std::cout << "|7 - Удалить дорогу                          |\n";
        std::cout << "|--------------------------------------------|\n";
        std::cin >> a;
        switch (a)
        {
        case 1:
            std::cout << "Введите название города:";
            std::cin >> city;
            do_correct_city_name(&city);
            railways.add_city(city);
            break;
        case 2:
            railways.print_сityes();
            std::cout << "Выберите 1 город:";
            std::cin >> first_city;
            do_correct_city_name(&first_city);
            std::cout << "Выберите 2 город:";
            std::cin >> second_city;
            do_correct_city_name(&second_city);
            std::cout << "Введите длину маршрута:";
            std::cin >> path_length;
            railways.add_road(path_length,first_city,second_city);
            break;
        case 3:
            railways.print_сityes();
            break;
        case 4:
            railways.print_roads();
            break;
        case 5:
            railways.print_сityes();
            std::cout << "Выберите 1 город:";
            std::cin >> first_city;
            do_correct_city_name(&first_city);
            std::cout << "Выберите 2 город:";
            std::cin >> second_city;
            do_correct_city_name(&second_city);
            find_all_paths(railways, first_city, second_city);
            break;
        case 6:
            railways.print_сityes();
            std::cout << "Выберите город:";
            std::cin >> first_city;
            do_correct_city_name(&first_city);
            railways.delete_city(first_city);
            break;
        case 7:
            railways.print_roads();
            std::cout << "Введите города между которыми дорога:" << std::endl;
            std::cout << "город 1:";
            std::cin >> first_city;
            do_correct_city_name(&first_city);
            std::cout << "город 2:";
            std::cin >> second_city;
            do_correct_city_name(&second_city);
            railways.delete_road(first_city, second_city);
            break;
        default:
            break;
        }
    }
}



void do_correct_city_name(std::string* city_name)
{
    (*city_name)[0] = toupper((*city_name)[0]);
    for (int i = 1; i < city_name->length(); i++)
    {
        (*city_name)[i] = tolower((*city_name)[i]);
    }
}

void find_all_paths_helper(Vertex* current, Vertex* destination, std::vector<Vertex*>& path, std::unordered_set<Vertex*>& visited, int length)
{
    path.push_back(current);
    visited.insert(current);

    if (current == destination)
    {
        for (auto v : path)
        {
            std::cout << v->city_name << " -> ";
        }
        std::cout << length;
        std::cout << std::endl;
    }
    else
    {
        for (auto& edge : current->edges)
        {
            if (edge.start_city == current && visited.find(edge.destination_city) == visited.end())
            {
                length += edge.path_length;
                find_all_paths_helper(edge.destination_city, destination, path, visited, length);
                length = 0;
            }
            else if (edge.destination_city == current && visited.find(edge.start_city) == visited.end())
            {
                length += edge.path_length;
                find_all_paths_helper(edge.start_city, destination, path, visited, length);
                length = 0;
            }
        }
    }

    path.pop_back();
    visited.erase(current);
}

void find_all_paths(Graf& graph, std::string start, std::string end)
{
    Vertex* startVertex = nullptr;
    Vertex* endVertex = nullptr;

    for (auto& vertex : graph.cities)
    {
        if (vertex.city_name == start)
        {
            startVertex = &vertex;
        }
        if (vertex.city_name == end)
        {
            endVertex = &vertex;
        }
    }

    if (startVertex && endVertex)
    {
        int length = 0;
        std::unordered_set<Vertex*> visited;
        std::vector<Vertex*> path;
        find_all_paths_helper(startVertex, endVertex, path, visited, length);
    }
    else
    {
        std::cout << "Start or end city not found in the graph" << std::endl;
    }
}
