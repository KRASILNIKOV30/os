#pragma once

#include "../Machine/Machine.h"
#include "EmptyStateRemover/EmptyStateRemover.h"

class DSMConverter
{
public:
    client::Machine ConvertToDSM(const dev::Machine& originMachine);

private:
    NonEmptyTransitions m_notEmptyTransitions = {};
};
