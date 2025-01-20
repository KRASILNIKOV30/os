#pragma once

#include <optional>
#include "Args.h"

class ArgsParser
{
public:
    static std::optional<Args> Parse(int argc, char* argv[])
    {
        Args args = {};

        if (argc != 3)
        {
            args.inputFile = "input.csv";
            args.outputFile = "output.csv";
        }
        else
        {
            args.inputFile = argv[1];
            args.outputFile = argv[2];
        }

        return args;
    }
};