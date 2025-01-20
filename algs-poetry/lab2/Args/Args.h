#pragma once

#include <string>
#include "../Common/GrammarSide.h"

struct Args
{
    GrammarSide grammarSide;
    std::string inputFile;
    std::string outputFile;
};