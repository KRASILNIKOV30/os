#include <iostream>
#include "StudentRepo.h"
#include <cassert>

int main()
{
    StudentRepo students;

    students.Insert({.id = 1, .name = "Lev", .branch = "PS", .enteringYear = 2022});
    students.Insert({.id = 2, .name = "Alexey", .branch = "PS", .enteringYear = 2022});
    students.Insert({.id = 3, .name = "Fedor", .branch = "PS", .enteringYear = 2022});
    students.Insert({.id = 4, .name = "Boris", .branch = "PS", .enteringYear = 2023});
    students.Insert({.id = 5, .name = "Vladimir", .branch = "MC", .enteringYear = 2023});
    students.Insert({.id = 6, .name = "Vladimir", .branch = "PS", .enteringYear = 2024});

    const auto s = students.Get(4);
    assert(s != nullptr && s->name == "Boris");

    const auto s2 = students.Get(42);
    assert(s2 == nullptr);

    std::string name = "Vladimir";
    students.FindAllWithName(name, [](const auto* student) {
        std::cout << *student << std::endl;
        return true;
    });

    return EXIT_SUCCESS;
};