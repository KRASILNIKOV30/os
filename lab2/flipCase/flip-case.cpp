#include <vector>
#include <string>
#include <iostream>

struct Args
{
    std::vector<std::string> files;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Invalid argument count\n"
                  << "Usage: expand_template.exe <inputFile.txt> <outputFile.txt> [<param> <value> [<param> <value> ...]]\n";
        return std::nullopt;
    }
    Args args;
    args.inputFilePath = argv[1];
    args.outputFilePath = argv[2];
    TemplateParams params;
    for (size_t i = 3; i <= argc - 2; i += 2)
    {
        params.emplace(argv[i], argv[i + 1]);
    }
    args.params = params;

    return args;
}

int main(int argc, char* argv[])
{
    auto args = ParseArgs(argc, argv);
    if (!args)
    {
        return EXIT_FAILURE;
    }

    if (!CopyAndExpandWithParams(args->inputFilePath, args->outputFilePath, args->params))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
