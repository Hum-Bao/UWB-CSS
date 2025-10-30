// 
//  Simple program which tests implemented sorts
//

#include <iostream>
#include <vector>
#include "sorts.h"
using namespace std;

void LocalSort(vector<int>& item_vector, int first, int last);
void DisplayVector(vector<int>& item_vector);
bool vectorCompare(vector<int>& v1, vector<int>& v2, int start, int finish);

int main()
{
	int score = 0;
	int miss = 0;
	int points = 0;
	vector<int>* p_arr;
	vector<int>* p_correct_arr;
	vector<int> array_to_sort = { 9, 22,1 ,27 ,4, 22, 5, 7, 8, 30, 10, 21,14, 15, 16, 
		17, 18, 19, 20, 29, 23, 3, 6, 28, 5, 24, 26, 11, 12, 13 };
	vector<int> sorted_array(array_to_sort);
	LocalSort(sorted_array, 0, sorted_array.size() - 1);

	//Insertion
	points = 2;
	p_arr = new vector<int>(array_to_sort);
	InsertionSort(*p_arr, 0, (*p_arr).size() - 1);
	if (vectorCompare(*p_arr, sorted_array, 0, (*p_arr).size() - 1))
	{
		score += points;
		cout << "SCORE: " << score << endl;
	}
	else
	{
		miss += points;
		cout << "Insertion Failed: -" << points << endl;
		DisplayVector(*p_arr);
		DisplayVector(sorted_array);
		cout << endl;
	}
	delete p_arr;

}

void LocalSort(vector<int>& item_vector, int first, int last)
{
	for (int i = first; i < last + 1; i++)
	{
		int next_item = item_vector[i];
		int count = i;
		while ((count > first) && (item_vector[count - 1]) > next_item)
		{
			item_vector[count] = item_vector[count - 1];

			count--;
		}
		item_vector[count] = next_item;
	}
}

void DisplayVector(vector<int>& item_vector)
{
	for (int i = 0; i < item_vector.size(); i++)
	{
		cout << item_vector[i] << " ";
	}
	cout << endl;
}

bool vectorCompare(vector<int>& v1, vector<int>& v2, int start, int finish)
{
	if (v1.size() != v2.size())
	{
		return false;
	}
	if ((finish < start) || (finish >= v1.size()))
	{
		return false;
	}
	bool arr_equal = true;
	for (int i = start; i <= finish; i++)
	{
		if (v1[i] != v2[i])
		{
			arr_equal = false;
			break;
		}
	}
	return arr_equal;
}
