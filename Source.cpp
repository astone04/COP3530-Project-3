#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <chrono>
#include <map>

using namespace std;

string fileLocation = "publishers.csv";
string sortOrder, sortBy;

struct Data
{
	string genre;
	string soldBy;
	string amazonRevenue;
	string authorRevenue;
	string grossSales; // revenue
	double addedSales;
	string publisherRevenue;
	string unitsSold; // popularity
	double addedUnits;
	string publisherName;
	string publisherType;
	string averageRating;
	string salePrice;
	string salesRank;
	string totalReview;

	// Overload <= operator to compare Data object
	bool operator<=(const Data& d)
	{
		Data data;
		if (sortBy == "revenue")
		{
			if (addedSales <= d.addedSales)
				return true;
			else
				return false;
		}
		else
		{
			if (addedUnits <= d.addedUnits)
				return true;
			else
				return false;
		}

	}
};

// Head structure reads data from csv and stores a copy
struct Head
{
	vector<Data> list;

	vector<Data> copy()
	{
		return this->list;
	}
};



// Initialize functions
void readCsv(vector<vector<string>>& cells);
void initialize(Head& head);

// Quicksort
void quickSort(vector<Data>& v, int low, int high);
int partition(vector<Data>& v, int start, int end);

// Mergesort
void mergeSort(vector<Data>& v, int left, int right);
void merge(vector<Data>& v, int left, int mid, int right);

bool compare(Data x, Data y);
void print(vector<Data> results);

int main() {
	int input = 0;
	int parameterChoice = 0;
	int sortChoice = 0;

	bool cont = true;
	bool ascending = true;

	Head head;
	vector<Data> v;

	initialize(head);

	while (cont)
	{
		cout << "Press 1 for Quicksort." << endl;
		cout << "Press 2 for Merge sort." << endl;
		cout << "Press 3 for unordered data." << endl;
		cout << "Input choice: ";
		cin >> input;

		cout << "Press 1 to sort by revenue." << endl;
		cout << "Press 2 to sort by popularity." << endl;
		cout << "Input choice: ";
		cin >> parameterChoice;

		cout << "Press 1 for ascending order." << endl;
		cout << "Press 2 for descending order." << endl;
		cin >> sortChoice;

		switch (sortChoice)
		{
		case 1:
			sortOrder = "ascending";
			break;
		case 2:
			sortOrder = "descending";
			break;
		default:
			break;
		}

		switch (parameterChoice)
		{
		case 1:
			sortBy = "revenue";
			break;
		case 2:
			sortBy = "popularity";
			break;
		default:
			break;
		}

		switch (input)
		{
		case 1:
		{
			v = head.copy();

			// test quicksort

			auto start = chrono::high_resolution_clock::now();
			quickSort(v, 0, v.size() - 1);
			auto end = chrono::high_resolution_clock::now();

			print(v);

			cout << endl << "Quicksort finished in " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns." << endl;
		}
		break;
		case 2:
		{
			v = head.copy();
			vector<Data> temp;

			// test merge sort
			auto start = chrono::high_resolution_clock::now();
			mergeSort(v, 0, v.size() - 1);
			auto end = chrono::high_resolution_clock::now();

			print(v);

			cout << endl << "Merge sort finished in " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns." << endl;
		}
		break;
		case 3:
			print(head.list);
			break;
		default:
			break;
		}
	}
	return 0;
}

void readCsv(vector<vector<string>>& cells)
{
	cout << "Reading data..." << endl << endl;

	vector<string> row;
	string line, word;

	fstream file(fileLocation, ios::in);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			row.clear();
			stringstream str(line);

			while (getline(str, word, ','))
			{
				row.push_back(word);
			}

			cells.push_back(row);
		}
	}
	else
	{
		cout << "Cannot open file.";
	}
}

void initialize(Head& head)
{
	vector<Data> initializedList;
	vector<Data> addedValues;
	vector<vector<string>> content;

	map<string, Data> values;

	// read data from csv
	readCsv(content);

	// Map data into array
	for (int i = 1; i < content.size(); i++)
	{
		Data unit;

		unit.genre = content[i][0];
		unit.soldBy = content[i][1];
		unit.amazonRevenue = content[i][2];
		unit.authorRevenue = content[i][3];
		unit.grossSales = content[i][4]; // used to sort by revenue
		unit.addedSales = stod(unit.grossSales);
		unit.publisherRevenue = content[i][5];
		unit.unitsSold = content[i][6]; // used to sort by popularity
		unit.addedUnits = stod(unit.unitsSold);
		unit.publisherName = content[i][7];
		unit.publisherType = content[i][8];
		unit.averageRating = content[i][9];
		unit.salePrice = content[i][10];
		unit.salesRank = content[i][11];
		unit.totalReview = content[i][12];

		initializedList.push_back(unit);

		// Add sales and units sold for matching genres
		if (values.find(unit.genre) == values.end()) // genre doesn't exist in map
		{
			values.insert({ unit.genre, unit });
		}
		else
		{
			values.at(unit.genre).addedSales += unit.addedSales;
			values.at(unit.genre).addedUnits += unit.addedUnits;
		}
	}

	// compile list of matching genres
	for (const auto& it : values)
	{
		addedValues.push_back(it.second);
	}

	// Place list into head
	head.list = addedValues;
}



// Quicksort
void quickSort(vector<Data>& v, int low, int high)
{
	if (low < high)
	{
		int pivot = partition(v, low, high);

		quickSort(v, low, pivot - 1);
		quickSort(v, pivot + 1, high);
	}
}
int partition(vector<Data>& v, int start, int end)
{
	Data temp;

	// select pivot
	Data pivot = v[end];

	// store index of greatest element
	int index = start - 1;

	// traverse array and compare each element with pivot
	for (int i = start; i < end; i++)
	{
		if (compare(v[i], pivot))
		{
			// if element smaller than pivot is found, swap it with index
			index++;

			// Swap
			temp = v[index];
			v[index] = v[i];
			v[i] = temp;
		}
	}

	// swap pivot with greater element at index
	temp = v[index + 1];
	v[index + 1] = v[end];
	v[end] = temp;

	return index + 1;
}

// Mergesort
void mergeSort(vector<Data>& v, int left, int right)
{
	if (left < right)
	{
		int mid = (left + (right - 1)) / 2;

		mergeSort(v, left, mid);
		mergeSort(v, mid + 1, right);

		// merge arrays after sorting
		merge(v, left, mid, right);
	}
}
void merge(vector<Data>& v, int left, int mid, int right)
{
	int x = mid - left + 1;
	int y = right - mid;

	vector<Data> X(x);
	vector<Data> Y(y);

	for (int i = 0; i < x; i++)
	{
		X[i] = v[left + i];
	}

	for (int i = 0; i < y; i++)
	{
		Y[i] = v[mid + 1 + i];
	}

	// store index of array and subarrays
	int i = 0, j = 0, k = left;

	// Traverse X and Y, picking the larger elements and place them in the correct position
	while (i < x && j < y)
	{
		if (compare(X[i], Y[j]))
		{
			v[k] = X[i];
			i++;
		}
		else
		{
			v[k] = Y[j];
			j++;
		}
		k++;
	}

	// store remaining elements from either X or Y
	while (i < x)
	{
		v[k] = X[i];
		i++;
		k++;
	}

	while (j < y)
	{
		v[k] = Y[j];
		j++;
		k++;
	}
}

bool compare(Data x, Data y)
{
	double a = 0.0;
	double b = 0.0;

	if (sortBy == "revenue")
	{
		a = x.addedSales;
		b = y.addedSales;
	}

	else if (sortBy == "popularity")
	{
		a = x.addedUnits;
		b = y.addedUnits;
	}

	if (sortOrder == "ascending")
	{
		return a < b;
	}
	else
	{
		return a > b;
	}
}

void print(vector<Data> results)
{
	cout << "\n\n";

	// iterate through genres and print
	for (auto unit : results)
	{
		cout << "Genre: " << unit.genre << endl;
		cout << "Revenue: " << unit.addedSales << endl;
		cout << "Popularity: " << unit.addedUnits << endl << endl;
	}
}

