#pragma once

#include "../Machine/Machine.h"

class MachineSaver
{
public:
    static void Save(std::ostream& output, const client::Machine& machine);
};
