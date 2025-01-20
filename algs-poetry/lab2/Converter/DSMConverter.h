#pragma once

#include "../Machine/Machine.h"
#include "../Common/GrammarSide.h"

class DSMConverter
{
public:
    static client::Machine ConvertToDSM(const dev::Machine& originMachine, GrammarSide grammarSide);
};
