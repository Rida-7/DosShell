#pragma once
#include "Directory.h"
#include <fstream>

class tree
{
public:
	Directory* root;
	Directory* current;

public:
	tree()
	{
		current = root = new Directory("V");
	}

	void insert(Directory* d)
	{
		current->subDirectories.push_back(d);
	}

	void setPath(Directory* d)
	{
		string path = current->name;
		Directory* parent = new Directory();
		while (parent != nullptr)
		{
			path = parent->name + "/" + path;
			parent = parent ->parent;
		}
		d->path = path;
	}

	void print(string prompt = "\\>")
	{
		if (current->name == "V")
		{
			cout << "V:" << prompt;
		}
		else
		{
			cout << "V:" << current->path.substr(2, current->path.length()) << "\\" << current->name << prompt;
		}
	}

	bool directoryExists(string n)
	{
		for (auto i = current->subDirectories.begin(); i != current->subDirectories.end(); i++)
		{
			if ((*i)->name == n)
			{
				return true;
			}
		}
		return false;
	}

	File* findFile(string n)
	{
		for (auto i = current->Files.begin(); i != current->Files.end(); i++)
		{
			if ((*i)->name == n)
			{
				return *i;
			}
		}
		return nullptr;
	}

	Directory* findDirectory(string n)
	{
		for (auto i = current->subDirectories.begin(); i != current->subDirectories.end(); i++)
		{
			if ((*i)->name == n)
			{
				return *i;
			}
		}
		return nullptr;
	}

	void printDirectoryStructure(Directory* current, int depth = 0)
	{
		for (int i = 0; i < depth; i++)
		{
			cout << "   "; // Adjust the spacing for better visualization
		}

		cout << current->name << "\\" << endl;

		for (auto subDir : current->subDirectories)
		{
			printDirectoryStructure(subDir, depth + 1);
		}
	}

	void saveTreeToFile(const string& fileName)
	{
		ofstream outputFile(fileName);

		if (!outputFile.is_open())
		{
			cout << "Error: Unable to open file " << fileName << " for writing." << endl;
			return;
		}

		// Save the tree structure to the file
		saveDirectoryToFile(root, outputFile);

		outputFile.close();
		cout << "Tree saved to " << fileName << " successfully." << endl;
	}

	void saveDirectoryToFile(Directory* currentDir, ofstream& file, const string& parentPath = "")
	{
		string currentPath = parentPath + "\\" + currentDir->name;
		file << currentPath << "\\" << std::endl;

		for (auto subDir : currentDir->subDirectories)
		{
			saveDirectoryToFile(subDir, file, currentPath);
		}

		for (auto files : currentDir->Files)
		{
			file << currentPath + "\\" + files->name << std::endl;
		}
	}

	


};