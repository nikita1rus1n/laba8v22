#pragma once



// НЕ ДОДЕЛАЛ!!!

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h> 

using namespace std;

class Exception
{
protected:
	int error;
public:
	Exception()
	{
		error = 0;
	}
	Exception(int err)
	{
		error = err;
	}
	~Exception()
	{

	};
	void Print()
	{
		if (this->error == 1)
		{
			cout << "Error! (only int)" << endl;
		}
		if (this->error == 2)
		{
			cout << "Error! (only double)" << endl;
		}if (this->error == 4)
		{
			cout << "Error! " << endl;
		}
	}
};

template<typename T>
class Node
{
public:
	Node<T>* pNext;
	T data;
	Node(T data = T(), Node* pNext = nullptr)
	{
		this->data = data;
		this->pNext = pNext;
	}
};

template<typename T>
class Iterator : public Node<T>
{
	Node<T>* pointer;
	friend class automobile;
public:
	Iterator()
	{
		this->pointer = nullptr;
	}
	Iterator(Node<T>* element)
	{
		this->pointer = element;
	}
	~Iterator()
	{
		this->pointer = nullptr;
	}
	Iterator operator+(int n)
	{
		return (pointer + n);
	}
	Iterator operator++(int n)
	{
		pointer = pointer->pNext;
		return pointer;
	}
	T& operator*()
	{
		return pointer->data;
	}
	bool operator!= (const Iterator& it)
	{
		return pointer != it.pointer;
	}
	T get_data() {
		return pointer->data;
	}
};


template<typename T>
class Stack :public Node<T>
{
public:
	Stack();
	~Stack();
	void pop();
	void clear();
	int GetSize()
	{
		return Size;
	}
	T& operator[](const int index);
	void push(T data);
	friend class File;
	Iterator<T> begin()
	{
		return head;
	}
	Iterator<T> end()
	{
		Node<T>* current = this->head;
		for (int i = 0; i < Size - 1; i++)
		{
			current = current->pNext;
		}
		return current;
	}
	void output(Iterator<T> begin, Iterator<T> end) {
		Iterator<T> it = begin;
		for (it; it != end; it++) {
			cout << *it << endl;
		}
		cout << *end << endl;
	}
private:
	int Size;
	Node<T>* head;
	friend ostream& operator<<(ostream& out, const Node<T>& node);
};

class Drug
{
private:
	string name; // имя
	int shelflife; // срок годности (количество дней)
	int rank; // 1 .. 10  рейтинг продаваемости препарата 
	string country; // страна
	int numofsold; // количество проданых

public:
	Drug(string _name, int _shelflife, int _rank, string _country, int _numofsold)
	{
		name = _name;
		shelflife = _shelflife;
		rank = _rank;
		country = _country;
		numofsold = _numofsold;
	}
	Drug() {
		name = "-";
		shelflife = 0;
		rank = 0;
		country = "-";
		numofsold = 0;
	}
	~Drug() {};
	friend void output(Drug& dr);
	friend class File;
};

class File
{
	string path;
public:
	File()
	{
		this->path = " ";
	}
	File(string _path)
	{
		this->path = _path;
	}
	void ReadText();
	void ReadBin(int n);
	void WriteText();
	void WriteBin(Drug ats);
	void Search();
	void Sort_time(int n, Stack<Drug>& st);
	void Sort_day(int n, Stack<Drug>& st);
	void ReadTextBin();
	int Del(int index, int n, Stack<Drug>& st);
};
template<typename T>
Stack<T>::Stack()
{
	Size = 0;
	head = nullptr;
}

template<typename T>
Stack<T>::~Stack()
{
	clear();
}

template<typename T>
void Stack<T>::pop()
{
	try
	{
		if (Size == 0)
		{
			throw 4;
		}
	}
	catch (int i)
	{
		Exception ex(i);
		ex.Print();
		return;
	}
	Node<T>* temp = head;
	head = head->pNext;
	delete temp;
	Size--;
}

template<typename T>
void Stack<T>::clear()
{
	while (Size)
	{
		pop();
	}
}

template<typename T>
T& Stack<T>::operator[](const int index)
{
	int counter = 0;
	Node<T>* current = this->head;
	while (current != nullptr)
	{
		if (counter == index)
		{
			return current->data;
		}
		current = current->pNext;
		counter++;
	}
}

template<typename T>
void Stack<T>::push(T data)
{
	head = new Node<T>(data, head);
	Size++;
}


template<typename T>
ostream& operator<<(ostream& out, const Node<T>& node)
{
	out << node.data;
	return out;
}

template<typename T>
ostream& operator<<(ostream& out, const Iterator<T>& it)
{
	out << *it;
	return out;
}
void output(Drug& dr) {
	cout << "Наименование препарата: " << dr.name << endl;
	cout << "Срок годности: " << dr.shelflife;
	cout << "Рейтинг продаваемости препарата(от 1 до 10) : " << dr.rank;
	cout << "Страна: "<< dr.country << endl;
	cout << "Кол-во проданных препаратов: " << dr.numofsold << endl;
}

int File::Del(int index, int n, Stack<Drug>& st)
{
	fstream fs;
	fs.open(path, fstream::in | fstream::binary | fstream::out);
	try
	{
		if (!fs.is_open())
		{
			throw 1;
		}
	}
	catch (int i)
	{
		Exception ex(i);
		ex.Print();
		return 0;
	}
	Drug* ms;
	ms = new Drug[n];
	for (int i = 0; i < n; i++)
	{
		fs.read((char*)& ms[i], sizeof(ms[i]));
	}
	fs.close();
	fstream fs1;
	fs1.open(path, fstream::binary | fstream::out | fstream::trunc);
	fs1.close();
	fstream fs2;
	fs2.open(path, fstream::binary | fstream::out);
	for (int i = 0; i < index; i++)
	{
		this->WriteBin(ms[i]);
	}
	for (int i = index + 1; i < n; i++)
	{
		this->WriteBin(ms[i]);
	}
	n = n - 1;
	cout << "Файл после удаления: " << endl;
	this->ReadBin(n);
	fs2.close();
	return n;
}

void File::ReadTextBin()
{
	char* msg;
	msg = new char[100];
	fstream fs;
	fs.open(path, fstream::in | fstream::binary);
	try
	{
		if (!fs.is_open())
		{
			throw 1;
		}
	}
	catch (int i)
	{
		Exception ex(i);
		ex.Print();
		return;
	}
	fs.read(msg, 100);
	int count = 0;
	for (int i = 0; i < 100; i++)
	{
		if (msg[i] >= 97 && msg[i] <= 122 || msg[i] >= 65 && msg[i] <= 90)
		{
			count++;
		}
	}
	for (int i = 0; i < count; i++)
	{
		cout << msg[i];
	}
	cout << endl;
	fs.close();
}

void File::WriteBin(Drug dr)
{
	fstream fs;
	fs.open(path, fstream::out | fstream::binary | fstream::app);
	try
	{
		if (!fs.is_open())
		{
			throw 1;
		}
	}
	catch (int i)
	{
		Exception ex(i);
		ex.Print();
		return;
	}
	fs.write((char*)& dr, sizeof(dr));
	fs.close();
}


int main()
{
	system("chcp 1251>null");
	Stack<Drug> st;
	File file2("fl2.bin");
	
		
	return 0;
}