#pragma once

#include <string>

enum CommandType
{
	MealyToMoore,
	MooreToMealy,
	MinimizeMealy,
	MinimizeMoore
};

struct Args
{
    CommandType commandType;
    std::string inputFile;
    std::string outputFile;
};