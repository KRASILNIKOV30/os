
#include <iostream>
#include <fstream>
#include <sstream>

#include "DFA.h"
#include "Lexer.h"

using namespace std;

string readFileToStringView(const string& filePath, string& storage)
{
	ifstream file(filePath, ios::in | ios::binary);
	if (!file)
	{
		throw runtime_error("Could not open file: " + filePath);
	}

	string line;
	storage.clear();
	bool firstLine = true;

	while (getline(file, line))
	{
		if (!firstLine)
		{
			storage += '\n';
		}
		storage += line;
		firstLine = false;
	}

	return storage;
}


int main(int argc, char* argv[])
{
	Lexer lexer;

	if (argc < 2)
	{
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}

	string filePath = argv[1];
	string fileContentStorage;

	try
	{
		string fileContent = readFileToStringView(filePath, fileContentStorage);

		cout << "Tokens List:" << endl << endl;
		auto tk = lexer.Lex(fileContent.data());
		for (auto i = tk.begin(); i < tk.end(); i++)
		{
			cout << "[" << i->line << ":" << i->pos << "] '" << i->lexeme << "' : " << getTokenName(i->type) << endl;
		}
	}
	catch (const exception& e)
	{
		cerr << "Error: " << e.what() << endl;
		return 1;
	}

	return EXIT_SUCCESS;
}
