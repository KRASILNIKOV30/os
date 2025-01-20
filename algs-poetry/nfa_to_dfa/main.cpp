#include "Args/ArgsParser.h"
#include "Converter/DSMConverter.h"
#include "MachineSaver/MachineSaver.h"
#include "TextParser/CSVTextParser.h"
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
		std::cout << "Wrong usage. Example: NotDSMToDSM input.csv output.csv" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input;
	std::ofstream output;
	if (!PrepareStreams(input, output, *args))
	{
		return EXIT_FAILURE;
	}
	auto dsmConverter = new DSMConverter();

	MachineSaver::Save(output, dsmConverter->ConvertToDSM(CSVTextParser::GetMachine(input)));

	return EXIT_SUCCESS;
}
