#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "../../Machine/Machine.h"

using namespace dev;

using NonEmptyTransitions = std::unordered_map<std::string, std::set<std::string>>;

class EmptyStateRemover
{
public:
    NonEmptyTransitions static GenerateNonEmptyTransitions(const Machine& machine);
};
