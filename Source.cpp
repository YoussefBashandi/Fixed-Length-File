#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <strstream>

using namespace std;

const char DeleteFlag = '*';

class books {
public:
	char ISBN[6];
	char title[21];
	char authorName[21];
	char price[4];
	char year[5];
	char numOfPages[5];
public:
	void AddBook(fstream& stream, books& b) {
		int flag;
		stream.seekg(0, ios::beg);
		stream.read((char*)&flag, sizeof(flag));
		if (flag == -1) {
			stream.seekp(0, ios::end);
			cout << "Enter the ISBN: " << endl;
			cin >> b.ISBN;
			cout << "Enter the Title: " << endl;
			cin >> b.title;
			cout << "Enter the Name: " << endl;
			cin >> b.authorName;
			cout << "Enter the Price: " << endl;
			cin >> b.price;
			cout << "Enter the Year: " << endl;
			cin >> b.year;
			cout << "Enter the Number of pages: " << endl;
			cin >> b.numOfPages;
			stream.write((char*)&b, sizeof(b));
		}
		else {
			stream.seekp(4 + flag * sizeof(b), ios::beg);
			cout << "Enter the ISBN: " << endl;
			cin >> b.ISBN;
			cout << "Enter the Title: " << endl;
			cin >> b.title;
			cout << "Enter the Name: " << endl;
			cin >> b.authorName;
			cout << "Enter the Price: " << endl;
			cin >> b.price;
			cout << "Enter the Year: " << endl;
			cin >> b.year;
			cout << "Enter the Number of pages: " << endl;
			cin >> b.numOfPages;
			stream.write((char*)&b, sizeof(b));
			stream.seekg(0, ios::beg);
			int header = -1;
			stream.write((char*)&header, sizeof(header));
		}
		
	}

	int deleteBook(fstream& stream, books& b) {
		int counter = 0;
		books Book;
		cout << "Enter the ISBN you want to delete: " << endl;
		cin >> b.ISBN;
		while (!stream.eof())
		{
			stream.seekg(4 + counter * sizeof(Book), ios::beg);
			stream.read((char*)&Book, sizeof(Book));
			if (strcmp(Book.ISBN, b.ISBN) == 0)
			{
				stream.seekg(4 + counter * sizeof(Book), ios::beg);
				stream.write((char*)&DeleteFlag, sizeof(DeleteFlag));
				stream.seekg(0, ios::beg);
				stream.write((char*)&counter, sizeof(counter));
				break;
			}
			else
			{
				counter++;
			}
		}

		if (stream.eof())
		{
			cout << "The ISBN you entered isn't exist." << endl;
			system("pause");
			return -1;
		}

	}
	int printBook(fstream& stream, books& b) {
		int counter = 0;
		books Book;
		cout << "Enter title " << endl;
		cin >> b.title;
		while (!stream.eof())
		{
			stream.seekg(4 + counter * sizeof(Book), ios::beg);
			stream.read((char*)&Book, sizeof(Book));
			if (strcmp(Book.title, b.title) == 0)
			{
				cout << "Found: " << Book.ISBN << " " << Book.title << " " << Book.authorName << " " << Book.price << " " << Book.year << " " << Book.numOfPages << endl;
				break;
			}
			else
			{
				counter++;
			}
		}
		if (stream.eof())
		{
			cout << "Not Found." << endl;
			return -1;
		}
	}
	void readAll(fstream& stream) {
		int counter = 0;
		books b;
		while (!stream.eof()) {
			stream.seekg(4 + counter * sizeof(b));
			stream.read((char*)&b, sizeof(b));
			if (b.ISBN[0] == '*') {
				counter++;
				continue;
			}
			cout << b.ISBN << "  ";
			cout << b.title << "  ";
			cout << b.authorName << "  ";
			cout << b.price << "  ";
			cout << b.year << "  ";
			cout << b.numOfPages << endl;
			counter++;
			if (stream.eof()) {
				break;
			}
		}
		stream.clear();
	}
	int UpdateBook(fstream& stream, books& b) {
		int counter = 0;
		books Book;
		while (!stream.eof())
		{
			stream.seekg(4 + counter * sizeof(Book), ios::beg);
			stream.read((char*)&Book, sizeof(Book));
			if (strcmp(Book.ISBN, b.ISBN) == 0)
			{
				cout << "Enter the new data: " << endl;
				cout << "Enter the ISBN: " << endl;
				cin >> Book.ISBN;
				cout << "Enter the Title: " << endl;
				cin >> Book.title;
				cout << "Enter the Name: " << endl;
				cin >> Book.authorName;
				cout << "Enter the Price: " << endl;
				cin >> Book.price;
				cout << "Enter the Year: " << endl;
				cin >> Book.year;
				cout << "Enter the Number of pages: " << endl;
				cin >> Book.numOfPages;
				stream.seekg(4 + counter * sizeof(Book), ios::beg);
				stream.write((char*)&Book, sizeof(Book));
				break;
			}
			else
			{
				counter++;
			}
			if (stream.eof())
			{
				cout << "The ISBN you entered is not found." << endl;
				return -1;
			}
		}
	}
	void compactFile(fstream& file) {
		fstream myFile;
		myFile.open("temp.txt", ios::out);
		books b;
		int counter = 0;
		while (!file.eof()) {
			file.seekg(4 + counter * sizeof(b), ios::beg);
			char check;
			file.read((char*)&check, sizeof(char));
			cout << check<<endl;
			//file.seekg(-1, ios::cur);
			if (check == '*') {
				counter++;
				continue;
			}
			file.read((char*)&b, sizeof(b));
			myFile.seekp(0, ios::end);
			int sizee = myFile.tellp();
			if (sizee <= 0) {
				int header = -1;
				myFile.seekp(0, ios::beg);
				myFile.write((char*)&header, sizeof(int));
			}
			myFile.seekp(0, ios::end);
			cout << b.ISBN << "  " << b.title << "  " << b.authorName << "  " << b.price << "  " << b.year << "  " << b.numOfPages << endl;
			myFile.write((char*)&b, sizeof(b));
			counter++;
			if (file.eof()) {
				break;
			}
		}
		file.close();
		myFile.close();
		remove("book.txt");
		rename("temp.txt", "book.txt");
	}
};

int main()
{
	fstream file;
	file.open("book.txt", ios::out | ios::in);
	books b;
	file.seekp(0, ios::end);
	int pos = file.tellp();
	if (pos <= 0)
	{
		file.seekp(0, ios::beg);
		int header = -1;
		file.write((char*)&header, sizeof(int));
	}
	b.AddBook(file,b);
	b.AddBook(file, b);
	b.deleteBook(file, b);
	//b.readAll(file);
	//books c;
	//b.printBook(file, b);
	//b.AddBook(file,b);
	system("pause");
	return 0;
}
