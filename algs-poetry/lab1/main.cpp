#include <iostream>
#include "Args/ArgsParser.h"
#include "TextParser/CSVTextParser.h"
#include "MachineSaver/MachineSaver.h"
#include "Converter/MachineConverter.h"
#include "MachineMinimizator/MachineMinimizator.h"
#include "Common/DotFileGenerator.h"

#include <fstream>

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
		std::cout << "Wrong usage. Example: Converter mealy|moore input.csv output.csv" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input;
	std::ofstream output;
	if (!PrepareStreams(input, output, *args))
	{
		return EXIT_FAILURE;
	}

	Machine machine;
	if (args->commandType == MinimizeMealy)
	{
		machine = CSVTextParser::GetMealy(input);
		machine = MachineMinimizator::MinimizeMealy(machine);
		MachineSaver::SaveMealyMachine(output, machine);
	}
	else if (args->commandType == MinimizeMoore)
	{
		machine = CSVTextParser::GetMoore(input);
		machine = MachineMinimizator::MinimizeMoore(machine);
		MachineSaver::SaveMooreMachine(output, machine);
	}
	else if (args->commandType == MealyToMoore)
	{
		machine = CSVTextParser::GetMealy(input);
		MachineSaver::SaveMealyMachine(output, MachineConverter::GetMooreMachineFromMealy(machine));
	}
	else if (args->commandType == MooreToMealy)
	{
		machine = CSVTextParser::GetMoore(input);
		MachineSaver::SaveMealyMachine(output, MachineConverter::GetMealyMachineFromMoore(machine));
	}
	GenerateDotFile(machine, "picture.dot");

	return EXIT_SUCCESS;
}
// нужно чтобы выводилась информация какие вершины в какие попали
// мили в мура