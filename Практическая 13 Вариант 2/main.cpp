//В виде одного файла
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
class Graph//Класс графа
{
private:
    class Node//Класс узла
    {
    private:
        uint32_t number;//Номер узла
        vector<pair<Node*, uint32_t>> links;//Связи узла
        static bool checkLink(Node* node1, Node* node2)//Проверка на связь между узлами
        {
            auto it = find_if(begin(node1->links), end(node1->links), [node2](pair<Node*, uint32_t> p) { return p.first == node2; });//Поиск связи
            if (it == end(node1->links)) return false;
            else return true;
        }
    public:
        Node(uint32_t number) { this->number = number; }
        void showLinks()//Вывод связей узла
        {
            for (pair<Node*, uint32_t> p : links)
                cout << "\t#" << p.first->getNumber() << "\tWeight:" << p.second << endl;
        }
        static bool addLink(Node* node1, Node* node2, uint32_t weight)//Добавление связи узлу
        {
            if (checkLink(node1, node2)) return false;
            else
            {
                node1->links.push_back(make_pair(node2, weight));
                node2->links.push_back(make_pair(node1, weight));
                return true;
            }
        }
        static bool removeLink(Node* node1, Node* node2)//Удаление связи у узла
        {
            if (checkLink(node1, node2))
            {
                node1->links.erase(remove_if(begin(node1->links), end(node1->links), [node2](pair<Node*, uint32_t> p) { return p.first == node2; }));//Удаление связь node1 к node2
                node2->links.erase(remove_if(begin(node2->links), end(node2->links), [node1](pair<Node*, uint32_t> p) { return p.first == node1; }));//Удаление связь node2 к node1
                return true;
            }
            else return false;
        }
        static int getWeightOfPair(Node* node1, Node* node2)//Вес связи между узлами
        {
            auto link = find_if(begin(node1->links), end(node1->links), [node2](pair<Node*, uint32_t> p) { return p.first == node2; });//Поиск связи между node1 и node2
            if (link == end(node1->links)) return -1;
            else return (*link).second;//Возвращаем вес
        }
        uint32_t getNumber() { return number; }//Геттер для номера узла
    };
    uint32_t nextNodeNumber = 1;//Номер следующего узла
    vector<Node*> nodes;//Вектор узлов
public:
    void addNode() { nodes.push_back(new Node(nextNodeNumber++)); }//Добавление узла
    void addNode(uint32_t count)//Добавление определенного количества узлов
    {
        for (uint32_t i = 0; i < count; i++) addNode();
    }
    bool removeBackNode()//Удаление узла с конца
    {
        if (nodes.empty()) return false;
        else
        {
            Node* node = nodes.back();//Получение узла
            for (uint32_t i = 0; i < nodes.size() - 1; i++) Node::removeLink(node, nodes[i]);//Удаление всех связей с ним
            nodes.pop_back();//Удаление узла из графа
            delete node;//Очищение памяти
            nextNodeNumber--;
            return true;
        }
    }
    bool addLink(uint32_t numberOfNode1, uint32_t numberOfNode2, uint32_t weight)//Добавление связи узлам
    {
        auto node1 = find_if(begin(nodes), end(nodes), [numberOfNode1](Node* node) {return node->getNumber() == numberOfNode1; });//Поиск узла node1
        auto node2 = find_if(begin(nodes), end(nodes), [numberOfNode2](Node* node) {return node->getNumber() == numberOfNode2; });//Поиск узла node2
        if (node1 == end(nodes) || node2 == end(nodes) || weight < 0) return false;
        else return Node::addLink((*node1), (*node2), weight);
    }
    bool removeLink(uint32_t numberOfNode1, uint32_t numberOfNode2)//Удаление связи между узлами
    {
        auto node1 = find_if(begin(nodes), end(nodes), [numberOfNode1](Node* node) {return node->getNumber() == numberOfNode1; });//Поиск узла node1
        auto node2 = find_if(begin(nodes), end(nodes), [numberOfNode2](Node* node) {return node->getNumber() == numberOfNode2; });//Поиск узла node2
        if (node1 == end(nodes) || node2 == end(nodes)) return false;
        else return Node::removeLink((*node1), (*node2));
    }
    void show()//Вывод графа
    {
        for (Node* node : nodes)
        {
            cout << "Node#" << node->getNumber() << endl;
            node->showLinks();
            cout << endl;
        }
    }
    void clear()//Очищение графа(Удаление всех узлов из графа)
    {
        for (Node* node : nodes) delete node;//Очищение памяти
        nodes.clear();//Удаление всех узлов
        nextNodeNumber = 1;
    }
    int getWeightOfPair(uint32_t numberOfNode1, uint32_t numberOfNode2)//Вес связи между узлами
    {
        auto node1 = find_if(begin(nodes), end(nodes), [numberOfNode1](Node* node) {return node->getNumber() == numberOfNode1; });//Поиск узла node1
        auto node2 = find_if(begin(nodes), end(nodes), [numberOfNode2](Node* node) {return node->getNumber() == numberOfNode2; });//Поиск узла node2
        if (node1 == end(nodes) || node2 == end(nodes)) return false;
        else return Node::getWeightOfPair((*node1), (*node2));
    }
    uint32_t getSize() { return nodes.size(); }//Размер графа(Количество узлов)
};
class SpanningTree//Класс остовного дерева минимального веса
{
private:
    Graph graph;//Граф остовного дерева
    pair<uint32_t, uint32_t>findSmallestPair(vector<uint32_t> usedNodeNumbers, Graph& graph)//Нахождение связи минимального веса между "использованным" узлом и "неиспользованным"
    {
        int minWeight = -1;//Минимальный вес
        pair<uint32_t, uint32_t> smallestPair = make_pair(0,0);//Узлы со связью с минимальным весом
        for (uint32_t nodeNumber : usedNodeNumbers)
        {
            for (uint32_t i = 1; i <= graph.getSize(); i++)
            {
                if (find(begin(usedNodeNumbers), end(usedNodeNumbers), i) == end(usedNodeNumbers))
                {
                    int currentWeight = graph.getWeightOfPair(nodeNumber, i);
                    if ((currentWeight < minWeight || minWeight == -1) && currentWeight != -1)
                    {
                        minWeight = currentWeight;
                        smallestPair = make_pair(nodeNumber, i);
                    }
                }
            }
        }
        return smallestPair;
    }
public:
    void create(Graph &graph)//Создание остовного дерева минимального веса(Алгоритм Прима)
    {
        this->graph.clear();//Очищение графа
        if (graph.getSize() != 0)
        {
            this->graph.addNode(graph.getSize());//Добавление узлов к остовному дереву
            vector<uint32_t> usedNodeNumbers = { 1 };//"Использованные" узлы
            while (usedNodeNumbers.size() != graph.getSize())//Цикл пока количество узлов в графе не сравняется с количеством "использованных узлов"
            {
                pair<uint32_t,uint32_t> smallestPair = findSmallestPair(usedNodeNumbers, graph);//Нахождение пары узлов с минимальным весом связи
                usedNodeNumbers.push_back(smallestPair.second);//Добавление нового узла в "использованные" узлы
                uint32_t weight = graph.getWeightOfPair(smallestPair.first, smallestPair.second);//Получение веса связи между найденными узлами
                this->graph.addLink(smallestPair.first, smallestPair.second, weight);//Добавление связи к остовному дереву
            }
        }
    }
    void show() { this->graph.show(); }//Вывод остовного дерева
};
int main()
{
    Graph graph;
    SpanningTree st;
    uint32_t numOfNodes, counter = 0;
    string input;
    cout << "Number of nodes: ";
    cin >> numOfNodes;
    graph.addNode(numOfNodes);
    cout << "Add links:\nformat: \"Number_of_first_node Number_of_second_node Weight_of_link\"\nEnter 0 for the end of the input\n";
    cin.ignore();
    while (true)
    {
        cout << "Link#" << counter << ": ";
        getline(cin, input);
        if (input == "0") break;
        istringstream iss(input);
        vector<string> results(istream_iterator<string>{iss},istream_iterator<string>());
        graph.addLink(stoul(results[0]), stoul(results[1]), stoul(results[2]));
        counter++;
    }
    cout << "-----------------------------------------------\n";
    cout << "Graph:\n";
    graph.show();
    cout << "-----------------------------------------------\n";
    cout << "SpanningTree:\n";
    st.create(graph);
    st.show();
    cout << "-----------------------------------------------\n";
    return 0;
}
/*
----------------------------------------------------
#14.2.1
1) Программный вывод:
Graph graph;
graph.addNode(5);
graph.addLink(1, 2, 1);
graph.addLink(1, 3, 2);
graph.addLink(1, 5, 10);
graph.addLink(2, 4, 3);
graph.addLink(2, 5, 6);
graph.addLink(3, 5, 7);
graph.addLink(3, 4, 4);
graph.addLink(4, 5, 11);
SpanningTree st;
st.create(graph);
st.show();
2) Вывод через клавиатуру(Скопировать и нажать enter)
5
1 2 1
1 3 2
1 5 10
2 4 3
2 5 6
3 5 7
3 4 4
4 5 11
0
----------------------------------------------------
#14.2.2
1) Программный вывод:
Graph graph;
graph.addNode(6);
graph.addLink(1, 2, 7);
graph.addLink(1, 4, 2);
graph.addLink(1, 6, 4);
graph.addLink(2, 4, 2);
graph.addLink(2, 3, 1);
graph.addLink(2, 3, 1);
graph.addLink(3, 4, 2);
graph.addLink(3, 5, 3);
graph.addLink(4, 5, 6);
graph.addLink(4, 6, 1);
graph.addLink(5, 6, 8);
SpanningTree st;
st.create(graph);
st.show();
2) Вывод через клавиатуру(Скопировать и нажать enter)
6
1 2 7
1 4 2
1 6 4
2 4 2
2 3 1
2 3 1
3 4 2
3 5 3
4 5 6
4 6 1
5 6 8
0
----------------------------------------------------
#14.2.3
1) Программный вывод:
Graph graph;
graph.addNode(7);
graph.addLink(1, 2, 20);
graph.addLink(1, 6, 23);
graph.addLink(1, 7, 1);
graph.addLink(2, 3, 15);
graph.addLink(2, 7, 4);
graph.addLink(3, 4, 3);
graph.addLink(3, 7, 9);
graph.addLink(4, 5, 17);
graph.addLink(4, 7, 16);
graph.addLink(5, 6, 28);
graph.addLink(5, 7, 25);
graph.addLink(6, 7, 36);
SpanningTree st;
st.create(graph);
st.show();
2) Вывод через клавиатуру(Скопировать и нажать enter)
7
1 2 20
1 6 23
1 7 1
2 3 15
2 7 4
3 4 3
3 7 9
4 5 17
4 7 16
5 6 28
5 7 25
6 7 36
0
----------------------------------------------------
#14.2.4
1) Программный вывод:
Graph graph;
graph.addNode(6);
graph.addLink(1, 2, 2);
graph.addLink(1, 3, 6);
graph.addLink(1, 4, 4);
graph.addLink(2, 4, 7);
graph.addLink(2, 5, 8);
graph.addLink(3, 4, 8);
graph.addLink(3, 6, 5);
graph.addLink(4, 5, 2);
graph.addLink(4, 6, 8);
graph.addLink(5, 6, 4);
SpanningTree st;
st.create(graph);
st.show();
2) Вывод через клавиатуру(Скопировать и нажать enter)
6
1 2 2
1 3 6
1 4 4
2 4 7
2 5 8
3 4 8
3 6 5
4 5 2
4 6 8
5 6 4
0
----------------------------------------------------
*/
