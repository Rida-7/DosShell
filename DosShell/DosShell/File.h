#pragma once
#include <iostream>

using namespace std;
class File
{
public:
	string name;
	string path;
	time_t printTime;
	string owner;
	bool readOnly;
	string fileType;
	string data;
public:
	File(string n = "NULL", string o = "NULL", string type = "txt")
	{
		name = n;
		owner = o;
		readOnly = false;
		fileType = type;
	}

	void print()
	{
		string att;
		if (readOnly)
		{
			att = "readAble";
		}
		else
		{
			att = "writeAble";
		}
		cout << name << "." << fileType << "\t" << owner << "\t" << att << endl;
	}

	void setContents(const string& contents)
	{
		data = contents;
	}
};