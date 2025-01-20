#pragma once

#include <optional>
#include "Args.h"

class ArgsParser
{
public:
    static std::optional<Args> Parse(int argc, char* argv[])
    {
        Args args = {};
        const std::string LEFT_GRAMMAR_TYPE_COMMAND = "left";
        const std::string RIGHT_GRAMMAR_TYPE_COMMAND = "right";

        if (argc != 4)
        {
            return std::nullopt;
        }

        if (argv[1] == LEFT_GRAMMAR_TYPE_COMMAND)
        {
            args.grammarSide = GrammarSide::LEFT;
        }
        else if (argv[1] == RIGHT_GRAMMAR_TYPE_COMMAND)
        {
            args.grammarSide = GrammarSide::RIGHT;
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