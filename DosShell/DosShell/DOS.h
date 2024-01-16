#pragma once
#include<string>
#include "tree.h"
#include <queue>
#include "currentFile.h"
#include <fstream>

class DOS
{
	string lower(string str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			str[i] = tolower(str[i]);
		}
		return str;
	}
	bool prompt = false;

public:
	tree t;
	string user = "Rida";
	priority_queue<File*>pqueue;
	queue<File*> tqueue;
	const string version = "1.0";
	DOS()
	{

	}

	void print()
	{
		cout << "\t\t\t\t\t" << "RIDA BATOOL|2022-CS-118|DSA-UET" << endl << endl;
		t.print();
	}

	void printPriorityQueue()
	{
		cout << "Priority Print Queue:" << endl;

		priority_queue<File*> tempQueue = pqueue;

		while (!tempQueue.empty())
		{
			File* file = tempQueue.top();
			cout << file->name << endl;
			tempQueue.pop();
		}
	}

	void printQueue()
	{
		cout << "Print Queue:" << endl;

		queue<File*> tempQueue1 = tqueue;

		while (!tempQueue1.empty())
		{
			File* file = tempQueue1.front();
			cout << file->name << endl;
			tempQueue1.pop();
		}
	}

	void printQueueWithTimeLeft()
	{
		time_t currentTime = time(nullptr);

		queue<File*> tempQueue1 = tqueue;

		if (tempQueue1.empty())
		{
			cout << "Print Queue is empty." << endl;
			return;
		}

		cout << "Print Queue with Time Left:" << endl;

		while (!tempQueue1.empty())
		{
			File* file = tempQueue1.front();

			int timeLeft = currentTime - file->printTime;

			cout << file->name << " (Time Left: " << timeLeft << " seconds)" << endl;

			tempQueue1.pop();
		}
	}

	void printPriorityQueueWithTimeLeft()
	{
		time_t currentTime = time(nullptr);

		priority_queue<File*> tempQueue = pqueue;

		if (tempQueue.empty())
		{
			cout << "Priority Print Queue is empty." << endl;
			return;
		}

		cout << "Priority Print Queue with Time Left:" << endl;

		while (!tempQueue.empty())
		{
			File* file = tempQueue.top();

			int timeLeft = currentTime - file->printTime;

			cout << file->name << " (Time Left: " << timeLeft << " seconds)" << endl;

			tempQueue.pop();
		}
	}

	void loadTreeFromFile(const string& fileName)
	{
		ifstream inputFile(fileName);

		if (!inputFile.is_open())
		{
			cout << "Error: Unable to open file " << fileName << endl;
			return;
		}

		// Clear existing tree structure
		t.root->emptyDirectory();

		// Read the tree structure from the file
		loadDirectoryFromFile(inputFile, t.root);

		inputFile.close();
		cout << "Tree loaded from " << fileName << " successfully." << endl;
	}

	// Helper function to load a directory and its subdirectories/files recursively from a file
	void loadDirectoryFromFile(ifstream& inputFile, Directory* currentDir)
	{
		string line;
		while (getline(inputFile, line))
		{
			// Identify the depth of the directory/file based on leading spaces
			size_t depth = line.find_first_not_of(' ');

			if (depth == string::npos)
			{
				continue; // Skip empty lines
			}

			// Trim leading spaces
			line = line.substr(depth);

			if (line.back() == '\\') // Directory
			{
				line.pop_back(); // Remove trailing '\'
				Directory* newDir = new Directory(line, currentDir->path + "\\" + currentDir->name, currentDir);
				loadDirectoryFromFile(inputFile, newDir);
				currentDir->subDirectories.push_back(newDir);
			}
			else // File
			{
				File* newFile = new File(line);
				currentDir->Files.push_back(newFile);
			}
		}
	}

	void run()
	{
		print();
		bool exit = false;
		while (!exit)
		{
			exit = input();
		}
	}

	bool input()
	{
		string command;
		getline(cin, command);
		string opr = command.substr(0, command.find(' '));
		opr = lower(opr);

		if (opr == "attrib")
		{
			if (command.find(' ') == string::npos)
			{
				cout << "Invalid command format. Usage: attrib <filename>" << endl;
				return true;
			}

			string name = command.substr(7);
			File* f = t.findFile(name);
			if (f)
			{
				f->print();
			}
			else
			{
				cout << "File '" << name << "' not found!" << endl;
			}
		}
		else if (opr == "cd")
		{
			if (command.find(' ') == string::npos)
			{
				cout << "Invalid command format. Usage: cd <directory>" << endl;
				return true;
			}

			string directory = command.substr(3);
			if (t.directoryExists(directory))
			{
				t.current = t.findDirectory(directory);
			}
			else
			{
				cout << "No such directory exists!" << endl;
			}
		}
		else if (opr == "dir" || opr == "cd.")
		{
			t.current->print();
		}
		else if (opr == "cd..")
		{
			if (t.current->parent != nullptr)
			{
				t.current = t.current->parent;
			}
		}
		else if (opr == "cd\\")
		{
			if (t.current->parent != nullptr)
			{
				t.current = t.root;
			}
		}
		else if (opr == "convert")
		{
			if (command.find(' ') == string::npos)
			{
				cout << "Invalid command format. Usage: convert <sourceExtension> <destinationExtension>" << endl;
				return true;
			}

			command = command.substr(opr.length() + 1);
			string sourceExt = command.substr(0, command.find(' '));
			string destExt = command.substr(sourceExt.length() + 1);

			for (auto i : t.current->Files)
			{
				if (i->fileType == sourceExt)
				{
					i->fileType = destExt;
				}
			}
		}
		else if (opr == "copy")
		{
			if (command.find(' ') == string::npos)
			{
				cout << "Invalid copy command. Usage: copy <sourcePath> <destPath>" << endl;
				return true;
			}
			command = command.substr(opr.length() + 3, command.length());
			int ind = command.find(' ');
			string sourcePath = command.substr(0, command.find(' '));
			string destPath = command.substr(sourcePath.length() + 3, command.length());
			File* fileToCopy;

			Directory* currentDir = t.root;
			string f;
			while (sourcePath.length() > 0)
			{
				f = sourcePath.substr(0, sourcePath.find('\\'));
				sourcePath = sourcePath.substr(f.length() + 1, sourcePath.length());
				if (sourcePath.length() != 0)
				{
					currentDir = currentDir->findDirectory(f);
				}
				if (currentDir == nullptr)
				{
					cout << "Path not found!";
					return true;
				}
			}
			fileToCopy = currentDir->findFile(f);

			currentDir = t.root;
			while (destPath.length() > 0)
			{
				f = destPath.substr(0, destPath.find('\\'));
				destPath = destPath.substr(f.length() + 1, destPath.length());
				currentDir = currentDir->findDirectory(f);
				if (currentDir == nullptr)
				{
					cout << "Path not found!";
					return true;
				}
			}
			currentDir->insertFile(fileToCopy);
			cout << "Copy successful!" << endl;
		}
		else if (opr == "create")
		{
			if (command.find(' ') == string::npos)
			{
				cout << "Invalid command format. Usage: create <filename>" << endl;
				return true;
			}

			string fileName = command.substr(7); // Extract filename from the command
			File* newFile = new File(fileName, user); // Assuming File constructor takes name and owner

			cout << "Enter the contents of the file. Type 'EOF' on a new line to finish:" << endl;

			string line;
			while (getline(cin, line))
			{
				if (line == "EOF")
				{
					break; // End input on 'EOF'
				}
				newFile->data += line + "\n"; // Append the line to file contents
			}

			// Save the contents of the file
			newFile->print(); // Print file information
			t.current->insertFile(newFile);
			cout << "File '" << fileName << "' created and contents saved." << endl;
		}

		else if (opr == "del")
		{
			command = command.substr(4, command.length());
			t.current->removeFile(command);
			cout << "File '" << command << "' deleted!" << endl;
		}
		else if (opr == "exit")
		{
			return true;
		}
		else if (opr == "find")
		{
			string name = command.substr(5, command.length());
			File* f = t.findFile(name);
			if (f != nullptr)
			{
				cout << "File '" << f << "' Exists!" << endl;
			}
			else
			{
				cout << "File Not Found!" << endl;
			}
		}
		else if (opr == "findf")
		{
			if (command.find(' ') == string::npos)
			{
				cout << "Invalid command format. Usage: search <filename> <text>" << endl;
				return true;
			}

			command = command.substr(opr.length() + 1); // Remove the command part
			string fileName = command.substr(0, command.find(' '));
			string searchText = command.substr(fileName.length() + 1);

			File* fileToSearch = t.findFile(fileName);

			if (fileToSearch != nullptr)
			{
				if (fileToSearch->data.find(searchText) != string::npos)
				{
					cout << "Text found in file '" << fileName << "'" << endl;
				}
				else
				{
					cout << "Text not found in file '" << fileName << "'" << endl;
				}
			}
			else
			{
				cout << "File '" << fileName << "' not found!" << endl;
			}
		}
		else if (opr == "findstr")
		{
			if (command.find(' ') == string::npos)
			{
				cout << "Invalid command format. Usage: findstr <text>" << endl;
				return true;
			}

			string searchText = command.substr(opr.length() + 1);

			// Iterate through all files in the current virtual directory
			for (auto file : t.current->Files)
			{
				// Check if the text is present in the file's contents
				if (file->data.find(searchText) != string::npos)
				{
					cout << "Text found in file: " << file->name << endl;
				}
				else if (file->data.find(searchText) == string::npos)
				{
					cout << "Text not found in any File." << endl;
				}
			}
		}
		else if (opr == "format")
		{
			t.root->emptyDirectory();
		}
		else if (opr == "help")
		{
			cout << "attrib\tDisplays file attributes." << endl;
			cout << "cd\tChanges the current directory." << endl;
			cout << "cd.\tPrints working directory." << endl;
			cout << "cd..\tChanges the current directory to its parent diretory." << endl;
			cout << "cd\tChanges the current directory " << endl;
			cout << "convert\tAsks two types and converts extension of all files of one type to another type." << endl;
			cout << "copy\tCopies one file in the current directory to another location (directory)." << endl;
			cout << "create\tCreates a file with the name provided and allows the user to enter contents of the file." << endl;
			cout << "del\tDelete a file whose name is provided in input." << endl;
			cout << "dir\tPrints working directory." << endl;
			cout << "edit\tOpens a file and allows the user to edit and save the contents of the file." << endl;
			cout << "exit\tQuits the program." << endl;
			cout << "find\tSearches for a file in your current virtual directory whose name is provided as input." << endl;
			cout << "findf\tSearches for a text string in the currently open file or the file whose name is provided as input." << endl;
			cout << "findstr\tSearches for strings in all files of your current virtual directory, prints names of files with the string." << endl;
			cout << "format\tFormats the current virtual directory i.e. empties the current directory and all subdirectories." << endl;
			cout << "help\tProvides Help information for all these commands." << endl;
			cout << "loadtree\tLoad a given tree in your tree structure." << endl;
			cout << "mkdir\tMakes a new directory in current directory " << endl;
			cout << "mov\tMoves one file (whose name is provided as input) from one directory to another directory." << endl;
			cout << "pprint\tAdds a text file to the priority based print queue, and displays the current priority queue." << endl;
			cout << "prompt\tChanges the Windows command prompt (for example from V:\&gt; to V$)." << endl;
			cout << "print\tAdds a text file to the print queue, and displays the current queue." << endl;
			cout << "pqueue\tShows current state of the priority based print queue, with time left for each element." << endl;
			cout << "pwd\tPrints working directory." << endl;
			cout << "queue\tShows current state of the print queue, with time left for each element." << endl;
			cout << "rename\tRenames a file whose current and new name is provided as input." << endl;
			cout << "rmdir\tRemoves a directory along with its contents." << endl;
			cout << "save\tSaves the currently open file to disk." << endl;
			cout << "tree\tDisplays the complete directory structure." << endl;
			cout << "ver\tDisplays the version of your program." << endl;


		}
		else if (opr == "mkdir")
		{
			command = command.substr(6, command.length());
			t.insert(new Directory(command, t.current->path + "\\" + t.current->name, t.current));
			cout << "Directory '" << command << "' created!" << endl;

		}
		else if (opr == "mov") {
			command = command.substr(opr.length() + 3, command.length());
			int ind = command.find(' ');
			string sourcepath = command.substr(0, command.find(' '));
			string destpath = command.substr(sourcepath.length() + 3, command.length());
			File* file_to_mov;

			// going all the way down to the source file and saving its pointer
			Directory* currFolder = t.root;
			string f;
			while (sourcepath.length() > 0) {
				f = sourcepath.substr(0, sourcepath.find('\\'));
				sourcepath = sourcepath.substr(f.length() + 1, sourcepath.length());
				if (sourcepath.length() != 0) {
					currFolder = currFolder->findDirectory(f);
				}
				if (currFolder == nullptr) {
					cout << "path was not found";
					return true;
				}
			}
			file_to_mov = currFolder->findFile(f);
			currFolder->removeFile(file_to_mov->name);

			// going all the way down to the destination and moving the saved file there
			currFolder = t.root;
			while (destpath.length() > 0) {
				f = destpath.substr(0, destpath.find('\\'));
				destpath = destpath.substr(f.length() + 1, destpath.length());
				currFolder = currFolder->findDirectory(f);
				if (currFolder == nullptr) {
					cout << "Path was not found";
					return true;
				}
			}
			currFolder->insertFile(file_to_mov);
		}
		else if (opr == "pprint")
		{
			command = command.substr(opr.length() + 1, command.length());
			string fileName = command.substr(0, command.find(' '));
			File* f = t.findFile(fileName);
			if (f != nullptr)
			{
				pqueue.push(f);
				f->printTime = time(nullptr);
				cout << "File '" << fileName << "' added to priority print queue." << endl;
				printPriorityQueue();
			}
			else
			{
				cout << "File Not Found!" << endl;
			}

		}
		else if (opr == "print")
		{
			command = command.substr(opr.length() + 1, command.length());
			string fileName = command.substr(0, command.find(' '));
			File* f = t.findFile(fileName);
			if (f != nullptr)
			{
				tqueue.push(f);
				f->printTime = time(nullptr);
				cout << "File '" << fileName << "' added to print queue." << endl;
				printQueue();
			}
			else
			{
				cout << "File Not Found!" << endl;
			}

		}
		else if (opr == "pqueue")
		{
			printPriorityQueueWithTimeLeft();
		}
		else if (opr == "pwd")
		{
			cout << "Current Working Directory: " << t.current->getPath() << endl;
		}
		else if (opr == "queue")
		{
			printQueueWithTimeLeft();
		}
		else if (opr == "prompt")
		{
			prompt = !prompt;
		}
		else if (opr == "rename")
		{
			command = command.substr(opr.length() + 1, command.length());
			string curName = command.substr(0, command.find(' '));
			string newName = command.substr(curName.length() + 1, command.length());
			File* f = t.current->findFile(curName);
			f->name = newName;
			cout << "File '" << curName << "' renamed to '" << "'" << newName << "'!" << endl;
		}
		else if (opr == "rmdir")
		{
			if (t.current == t.root)
			{
				cout << "Cannot remove the root directory." << endl;
			}
			else
			{
				Directory* current = t.current;
				t.current = t.current->parent;

				cout << "Directory '" << current->name << "' removed!" << endl;
				t.current->removeDirectory(current->name);
			}
		}
		else if (opr == "edit")
		{
			editor d;
			d.run();
			system("cls");
			print();
			system("color 07");
			
		}
		else if (opr == "sav")
		{
			editor d;
			d.run();
			system("cls");
			print();
			system("color 07");
		}
		else if (opr == "cls")
		{
			system("cls");
			print();
		}
		else if (opr == "tree")
		{
			cout << "Directory Tree:" << endl;
			t.printDirectoryStructure(t.root);
		}
		else if (opr == "ver")
		{
			cout << "Version: " << version << endl;
		}
		else if (opr == "savetree")
		{
			t.saveTreeToFile("tree.txt");
		}
		else if (opr == "loadtree")
		{
			loadTreeFromFile("tree.txt");
		}
		else
		{
			cout << "'" << opr << "' is not recognized as any command." << endl;;
		}
		if (opr != "cls")
		{
			if (prompt)
			{
				t.print("$");
			}
			else
			{
				t.print();
			}
		}

		return false;
	}
};