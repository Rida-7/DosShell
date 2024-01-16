#pragma once
#include <iostream>
#include <list>
#include "File.h"

using namespace std;
class Directory
{
public:
	string name;
	list<Directory*> subDirectories;
	list<File*> Files;
	Directory* parent;
	time_t creationTime;
	string owner;
	string path;
	bool hidden;

public:
	Directory(string n = "NULL", string p = "", Directory* parent_ = nullptr, list<Directory*> SF = list<Directory*>(), list<File*> fs = list<File*>())
	{
		name = n;
		subDirectories = SF;
		Files = fs;
		parent = parent_;
		path = p;
		hidden = false;
	}

	string getPath() 
	{
		if (parent != nullptr) 
		{
			return parent->getPath() + "\\" + name;
		}
		else
		{
			return name;
		}
	}


	void removeFile(string n)
	{
		for (auto i = Files.begin(); i != Files.end(); i++)
		{
			if ((*i)->name == n)
			{
				delete* i;
				i = Files.erase(i);
				break;
			}
			else
			{
				++i;
			}
		}
	}

	void removeDirectory(string n)
	{
		for (auto i = subDirectories.begin(); i != subDirectories.end();)
		{
			if ((*i)->name == n)
			{
				delete *i;  // Delete the object pointed to by i
				i = subDirectories.erase(i);  // Erase the element from the vector
				break;
			}
			else
			{
				++i;
			}
		}
	}


	void emptyDirectory()
	{
		subDirectories.clear();
		Files.clear();
	}

	File* findFile(string n)
	{
		for (auto i = Files.begin(); i != Files.end(); i++)
		{
			if ((*i)->name == n)
			{
				return *i;
			}
			else
			{
				++i;
			}
		}
		return nullptr;
	}

	Directory* findDirectory(string n)
	{
		for (auto i = subDirectories.begin(); i != subDirectories.end(); i++)
		{
			if ((*i)->name == n)
			{
				return *i;
			}
			else
			{
				++i;
			}
		}
		return nullptr;
	}

	void insertFile(File* f)
	{
		Files.push_back(f);
	}

	void insertDirectory(Directory* d)
	{
		subDirectories.push_back(d);
	}

	void print()
	{
		cout << "Directory of " << path << "\\" << name << endl << endl;
		for (auto i = subDirectories.begin(); i != subDirectories.end(); i++)
		{
			cout << "                      <DIR>     " << (*i)->name << endl;
		}
		for (auto i = Files.begin(); i != Files.end(); i++)
		{
			cout << "                                " << (*i)->name << endl;
		}
	}
};