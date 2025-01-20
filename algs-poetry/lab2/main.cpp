#include "Args/ArgsParser.h"
#include "Common/DotFileGenerator.h"
#include "Converter/DSMConverter.h"
#include "GrammarParser/GrammarParser.h"
#include "MachineSaver/MachineSaver.h"
#include <fstream>
#include <iostream>

bool PrepareStreams(std::ifstream& input, std::ofstream& output, const Args& args)
{
	input.open(args.inputFile);

	if (!input.is_open())
	{
		std::cout << "Input file couldn't be opened" << std::endl;
		return false;
	}

	output.open(args.outputFile);

	if (!output.is_open())
	{
		std::cout << "Output file couldn't be opened" << std::endl;
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	auto args = ArgsParser::Parse(argc, argv);
	if (!args)
	{
		std::cout << "Wrong usage. Example: GrammarToDSM left|right input.txt output.csv" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input;
	std::ofstream output;
	if (!PrepareStreams(input, output, *args))
	{
		return EXIT_FAILURE;
	}

	auto machine = DSMConverter::ConvertToDSM(GrammarParser::ParseGrammarToMachine(input, args->grammarSide), args->grammarSide);

	MachineSaver::Save(output, machine);
	GenerateDotFile(machine, "picture.dot");

	return EXIT_SUCCESS;
}
