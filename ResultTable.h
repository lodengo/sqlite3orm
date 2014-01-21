#pragma once

#pragma warning(disable:4251)

#include <string>
#include <vector>
using namespace std;

struct ResultTable
{	
	void Fill(char** pazResult, int nRow, int nCol)
	{
		ColNames.clear();
		Results.clear();

		RowCount = nRow;
		ColCount = nCol;

		int index = nCol;
		for (int i = 0; i < nCol; i++)
		{
			ColNames.push_back(pazResult[i]);
		}
		for (int j = 0; j < nRow; j++)
		{
			vector<string> row;
			for (int c = 0; c < nCol; c++)
			{
				string s;
				if (pazResult[index] != NULL)
				{
					s = pazResult[index];
				}
				row.push_back(s);
				index++;
			}
			Results.push_back(row);
		}
	}
	vector<string> ColNames;
	vector< vector<string> > Results;
	int RowCount;
	int ColCount;
};