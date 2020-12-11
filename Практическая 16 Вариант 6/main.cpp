#include <iostream>
#include <set>
#include <vector>
using namespace std;

class Item // Класс предмета
{
private:
	uint32_t weight; // Вес предмета
	uint32_t price; // Стоимость предмета
public:
	Item() // Конструктор по-умолчанию обнуляющий значения веса и стоимости
	{
		this->weight = 0;
		this->price = 0;
	}
	Item(uint32_t weight, uint32_t price) // Конструктор устанавливающий значения веса и стоимости
	{
		this->weight = weight;
		this->price = price;
	}
	uint32_t getWeight() { return this->weight; } // Геттер для веса
	void setWeight(uint32_t weight) { this->weight = weight; } // Сеттер для веса
	uint32_t getPrice() { return this->price; } // Геттер для стоимости
	void setPrice(uint32_t price) { this->price = price; } // Сеттер для стоимости
	friend ostream& operator <<(ostream& os, const Item& item) // Перегруженный оператор << 
	{
		os << "{ вес = " << item.weight << ", стоимость = " << item.price << " }";
		return os;
	}
};
class Backpack // Класс рюкзака
{
private:
	uint32_t maxWeight; // Вместимость рюкзака
	uint32_t bestPrice; // Наибольшая возможная стоимость
	vector<Item> bestItems; // Предметы с наибольшей стоимостью, вес которых в сумме не превышает вместимость рюкзака
	uint32_t calcWeigth(vector<Item> items) const // Общий вес набора предметов
	{
		uint32_t sumW = 0;
		for(Item item : items)
			sumW += item.getWeight();
		return sumW;
	}
	uint32_t calcPrice(vector<Item> items) const // Общая стоимость набора предметов
	{
		uint32_t sumPrice = 0;
		for (Item item : items)
			sumPrice += item.getPrice();
		return sumPrice;
	}
	void checkSet(vector<Item> items) // Проверка, на "лучший" набор предметов
	{
		if (bestItems.size() == NULL)
		{
			if (calcWeigth(items) <= maxWeight)
			{
				bestItems = items;
				bestPrice = calcPrice(items);
			}
		}
		else
		{
			if (calcWeigth(items) <= maxWeight && calcPrice(items) > bestPrice)
			{
				bestItems = items;
				bestPrice = calcPrice(items);
			}
		}
	}
public:
	Backpack(uint32_t maxWeight) // Конструктор устанавливающий вместимость рюкзака
	{ 
		this->maxWeight = maxWeight;
		this->bestPrice = 0;
	}
	uint32_t getMaxWeight() { return this->maxWeight; } // Геттер для вместимости рюкзара
	void setMaxWeight(uint32_t maxWeight) { this->maxWeight = maxWeight; } // Сеттер для вместимости рюкзака
	void makeSets(vector<Item> items, vector<Item>curItems = vector<Item>(0)) // Перебор наборов предметов методом ветвей и границ
	{
		if (curItems.size() > 0)
			checkSet(curItems);
		for (uint32_t i = 0; i < items.size(); i++)
		{
			if (calcWeigth(curItems) + items[i].getWeight() <= this->maxWeight)
			{
				curItems.push_back(items[i]);
				items.erase(items.begin() + i);
				makeSets(items, curItems);
				items.insert(items.begin() + i, curItems.back());
				curItems.pop_back();
			}
		}
	}
	vector<Item> getBestSet() // Геттер для набора "лучших" предметов
	{
		return bestItems;
	}
	friend ostream& operator <<(ostream& os, const Backpack& backpack) // Перегруженный оператор << 
	{
		os << "Вместимость рюкзака: " << backpack.maxWeight << endl;
		os << "Полученный вес: " << backpack.calcWeigth(backpack.bestItems) << endl;
		os << "Полученная стоимость: " << backpack.calcPrice(backpack.bestItems) << endl;
		os << "Количество предметов: " << backpack.bestItems.size() << endl;
		os << "Предметы: " << endl;
		for (uint32_t i = 0; i < backpack.bestItems.size(); i++)
			os << "\t" <<backpack.bestItems[i] << endl;
		return os;
	}
};
int main()
{
	setlocale(LC_ALL, "Russian");
	uint32_t maxWeight, count;
	vector<Item> items;
	cout << "Введите вместимость рюкзака: ";
	cin >> maxWeight;
	Backpack backpack(maxWeight);
	cout << "Введите количество предметов: ";
	cin >> count;
	cout << "Введите веса и стоимости предметов: \n\n";
	for (uint32_t i = 0; i < count; i++)
	{
		uint32_t weight, price;
		cout << "Предмет №" << i + 1 << ":\n";
		cout << "\t Вес: ";
		cin >> weight;
		cout << "\t Стоимость: ";
		cin >> price;
		items.push_back(Item(weight, price));
	}
	backpack.makeSets(items);
	cout << "--------------------------------------\n";
	cout << "Результат: \n";
	cout << backpack;
	return 0;
}