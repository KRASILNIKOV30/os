#pragma once

#include "../Machine/Machine.h"
#include "Args.h"
#include <optional>

class ArgsParser
{
public:
	static std::optional<Args> Parse(const int argc, char* argv[])
	{
		Args args = {};
		const std::string MEALY_MOORE_COMMAND = "mealy-to-moore";
		const std::string MOORE_MEALY_COMMAND = "moore-to-mealy";
		const std::string MINIMIZE_MEALY_COMMAND = "minimize-mealy";
		const std::string MINIMIZE_MOORE_COMMAND = "minimize-moore";

		if (argc != 4)
		{
			return std::nullopt;
		}

		if (argv[1] == MEALY_MOORE_COMMAND)
		{
			args.commandType = MealyToMoore;
		}
		else if (argv[1] == MOORE_MEALY_COMMAND)
		{
			args.commandType = MooreToMealy;
		}
		else if (argv[1] == MINIMIZE_MEALY_COMMAND)
		{
			args.commandType = MinimizeMealy;
		}
		else if (argv[1] == MINIMIZE_MOORE_COMMAND)
		{
			args.commandType = MinimizeMoore;
		}
		else
		{
			return std::nullopt;
		}

		args.inputFile = argv[2];
		args.outputFile = argv[3];
		return args;
	}
};