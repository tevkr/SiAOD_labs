#include <iostream>
#include <array>
using namespace std;
class Book
{
private:
	uint_least64_t isbn;
	string author;
	string title;
public:
	Book() {}
	Book(uint_least64_t isbn, string author, string title)
	{
		this->isbn = isbn;
		this->author = author;
		this->title = title;
	}
	uint_least64_t getIsbn() const { return isbn; }
	string getAuthor() const { return author; }
	string getTitle() const { return title; }
	bool isEmpty() { return !isbn; }
};
ostream& operator<<(ostream& os, const Book& book)
{
	os << "ISBN: " << book.getIsbn() << "\nAuthor: " << book.getAuthor() << "\nTitle: " << book.getTitle();
	return os;
}
class Books
{
private:
	static const uint32_t SIZE = 13;
	array<Book, SIZE> books;
	array<bool, SIZE> deleted;
	uint32_t h(uint_least64_t key, uint32_t i) { return (h1(key) + i * h2(key)) % SIZE; }
	uint32_t h1(uint_least64_t key) { return key%SIZE; }
	uint32_t h2(uint_least64_t key) { return (key % (SIZE-2)) + 1; }
public:
	Books()
	{
		Book emtpyBook(0, "unknown", "unknown");
		books.fill(emtpyBook);
		deleted.fill(false);
	}
	void add(Book book)
	{
		for (uint32_t i = 0; i < SIZE; i++)
		{
			uint32_t x = h(book.getIsbn(), i);
			if (books[x].isEmpty() || deleted[x])
			{
				books[x] = book;
				deleted[x] = false;
				return;
			}
		}
	}
	Book find(uint_least64_t key)
	{
		for (uint32_t i = 0; i < SIZE; i++)
		{
			uint32_t x = h(key, i);
			if (!books[x].isEmpty())
			{
				if (books[x].getIsbn() == key && !deleted[x])
					return books[x];
			}
			else
				return Book(0, "unknown", "unknown");
		}
		return Book(0, "unknown", "unknown");
	}
	void del(uint_least64_t key)
	{
		for (uint32_t i = 0; i < SIZE; i++)
		{
			uint32_t x = h(key, i);
			if (!books[x].isEmpty())
			{
				if (books[x].getIsbn() == key)
				{
					deleted[x] = true;
					return;
				}
			}
			else
				return;
		}
	}
	void out()
	{
		for (uint32_t i = 0; i < SIZE; i++)
			if (!books[i].isEmpty() && !deleted[i])
				cout << "Books[" << i << "]:\n" << books[i] << endl << endl;
	}
};
int main() 
{
	Books books;
	books.add(Book(123456789012, "Jennifer Ashley", "Murder in the East End"));
	books.add(Book(111111111111, "Luanne Rice", "Last Day"));
	books.add(Book(222222222222, "James Patterson", "1st Case"));
	books.add(Book(333333333333, "Kendra Elliot", "The Last Sister"));
	cout << "Books list:\n";
	books.out();
	cout << "---------------------------------\n";
	books.del(111111111111);
	cout << "Books list:\n";
	books.out();
	cout << "---------------------------------\n";
	cout << "Find key(222222222222):\n";
	cout << books.find(222222222222) << endl;
	cout << "---------------------------------\n";
	cout << "Find key(111111111111):\n";
	cout << books.find(111111111111) << endl;
	cout << "---------------------------------\n";
	return 0;
}