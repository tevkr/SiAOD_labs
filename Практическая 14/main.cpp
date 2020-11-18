#include <iostream>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
class Node//Класс узла
{
public:
    char Alphabet;//Символ
    Node* pNext1, *pNext2;//Указатели на двух потомков
    uint16_t Occurrences;//Появления
    float Probability;//Вероятность
    string code;//Код символа
    Node(char Alphabet, uint16_t Occurrences, float Probability)
    {
        this->Alphabet = Alphabet;
        this->Occurrences = Occurrences;
        this->Probability = Probability;
        pNext1 = nullptr;
        pNext2 = nullptr;
    }
    Node(Node* pNext1, Node* pNext2, uint16_t Occurrences, float Probability)
    {
        this->Alphabet = '\0';
        this->Occurrences = Occurrences;
        this->Probability = Probability;
        this->pNext1 = pNext1;
        this->pNext2 = pNext2;
    }
    ~Node()
    {
        delete pNext1;
        delete pNext2;
    }
};
class Table//Класс таблицы
{
private:
    vector<Node*> table;
public:
    void fill(string fio)//Заполнение таблицы
    {
        transform(begin(fio), end(fio), begin(fio), [](unsigned char c) { return std::tolower(c); });//Б->б
        uint16_t length = fio.length();//Длина строки
        for (char ch : fio)
        {
            auto it = find_if(begin(table), end(table), [ch](Node* row)//Нахождение итератора по символу
                {
                    return row->Alphabet == ch;
                });
            if (it == end(table))
                table.push_back(new Node(ch, 1, 1 / (float)length));//Если символа нет, добавляем
            else
            {
                (*it)->Occurrences++;
                (*it)->Probability = (*it)->Occurrences / (float)length;
            }
        }
    }
    void show()//Вывод таблицы
    {
        cout << "Алфавит\t\tКол-во вх.\tВероятн.\n";
        for (Node* row : table)
            cout << "\"" << row->Alphabet << "\"" << "\t\t" << row->Occurrences << "\t\t" << row->Probability << endl;
    }
    void sortTable()//Сортировка таблицы по убыванию
    {
        sort(begin(table), end(table), [](Node* row1, Node* row2) { return row1->Probability > row2->Probability; });
    }
    vector<Node*> getTable()
    {
        return this->table;
    }
};
class HuffmanTree//Класс дерева Хаффмана
{
private:
    Node* root;//Указатель на корневой узел
    string str;//Полученная строка
    vector<Node*> table;//Таблица с символами
    string tab(const uint16_t depth)//Отступы для дерева
    {
        string str = "";
        for (uint32_t i = 0; i < depth; i++)
            str += "  ";
        return str;
    }
    void recursionHuffmanTree(const Node* node, const uint16_t depth, string code)//Рекурсивный вывод дерева
    {
        cout << tab(depth) << "[" << node->Occurrences << "|" << node->Probability << "]:code[" << code << "]";
        if (node->pNext1 == nullptr)
        {
            cout << "\"" << node->Alphabet << "\"" << endl;
            return;
        }
        else
        {
            cout << endl;
            recursionHuffmanTree(node->pNext1, depth + 1, "0");
            recursionHuffmanTree(node->pNext2, depth + 1, "1");
        }
    }
    void recursionsetCodes(Node* node, const char ch,string path)//Рекурсивный вывод кодов символов
    {
        if (node->pNext1 == nullptr)
        {
            if (node->Alphabet == ch) node->code = path;
            return;
        }
        else
        {
            recursionsetCodes(node->pNext1, ch, path + "0");
            recursionsetCodes(node->pNext2, ch, path + "1");
        }
    }
public:
    HuffmanTree(Table t, string str)
    {
        this->str = str;
        table = t.getTable();
        while (table.size() != 1)
        {
            //сортируем свободные узлы
            sort(begin(table), end(table), [](Node* node1, Node* node2) { return node1->Occurrences > node2->Occurrences; });
            //Находим 2 наименьших и удаляем
            Node* node1 = table[table.size() - 1];
            table.pop_back();
            Node* node2 = table[table.size() - 1];
            table.pop_back();
            //Создаем новый узел с указателями на удаленные из свободных узлов
            Node* newNode = new Node
            (
                node2,
                node1,
                node1->Occurrences + node2->Occurrences,
                node1->Probability + node2->Probability
            );
            //новый созданный обавляем в свободные
            table.push_back(newNode);
        }
        root = table[0];
        table = t.getTable();
    }
    void showHuffmanTree()
    {
        cout << "Дерево кодирования Хаффмана:\n";
        recursionHuffmanTree(root, 0, "root");
    }
    void setCodes()//Установка кодов для символов
    {
        for (Node* node : table)
            recursionsetCodes(root, node->Alphabet, "");
    }
    string getCode(const char ch)//Получение кода символа
    {
        for (Node* node : table)
        {
            if (ch == node->Alphabet) return node->code;
        }
    }
    void showCodes()//Вывод всех кодов
    {
        cout << "Коды символов:\n";
        for (Node* node : table)
            cout << "\"" << node->Alphabet << "\" : " << node->code << endl;
    }
    void showFIO()//Шифрование полученного сообщения
    {
        for (char ch : str)
            cout << getCode(ch);
        cout << endl;
    }
};
void asciiComp(const string str, HuffmanTree & const ht)//Сравнение с аски
{
    cout << "Коэффициент сжатия относительно использования коди-ровки ASCII (8 бит/символ):\n";
    cout << "L[ascii] = " << 8 * str.length() << endl;
    uint32_t k = 0;
    for (char ch : str)
        k += ht.getCode(ch).length();
    cout << "L[huff] = " << k << endl;
    cout << "Следовательно, коэффициент сжатия будет равен\n";
    cout << "K = L[ascii] / L[huff] = " << 8 * str.length() / (double)k << endl;
}
void unicideComp(const string str, HuffmanTree& const ht)//Сравнение с юникодом
{
    cout << "Коэффициент сжатия относительно равномерного кода (5 бит/символ):\n";
    cout << "L[uni] = " << 5 * str.length() << endl;
    uint32_t k = 0;
    for (char ch : str)
        k += ht.getCode(ch).length();
    cout << "L[huff] = " << k << endl;
    cout << "Следовательно, коэффициент сжатия будет равен\n";
    cout << "K = L[uni] / L[huff] = " << 5 * str.length() / (double)k << endl;
}
void averageCodeLength_dispersion(Table & const table)//ср длина и дисперсия
{
    double l = 0, b = 0;
    for (Node* node : table.getTable())
        l += node->Probability * node->code.length();
    cout << "Для полученного кода средняя длина будет равна:\n";
    cout << "Lср = " << l << endl;
    for (Node* node : table.getTable())
        b += node->Probability * pow((node->code.length() - l),2);
    cout << "Для полученного кода дисперсия будет равна:\n";
    cout << "б = " << b << endl;;
}
int main()
{
    setlocale(LC_ALL, "Russian"); SetConsoleCP(1251); SetConsoleOutputCP(1251);
    string fio;
    cout << "Введите ФИО: ";
    getline(cin, fio);
    Table table;
    table.fill(fio);
    cout << "Таблица частот:\n";
    table.show();
    table.sortTable();
    cout << "Отсортированная таблица частот:\n";
    table.show();
    HuffmanTree ht(table, fio);
    ht.showHuffmanTree();
    ht.setCodes();
    ht.showCodes();
    cout << "Исходная строка:\n";
    ht.showFIO();
    asciiComp(fio, ht);
    unicideComp(fio, ht);
    averageCodeLength_dispersion(table);
    system("pause");
    return 0;
}
