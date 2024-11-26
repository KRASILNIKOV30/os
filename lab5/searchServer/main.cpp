#include "Menu.h"
#include "SearchServer.h"

int main()
{
	SearchServer server;
	Menu menu(std::cin, std::cout, {
		Item("add_file", "adds file to index", [&](auto& input) {
			Path path;
			input >> path;
			server.AddFile(path);
		}),
		Item("add_dir", "adds directory to index", [&](auto& input) {
			Path path;
			input >> path;
			server.AddDir(path);
		}),
		Item("add_dir_recursive", "adds directory to index recursively", [&](auto& input) {
			Path path;
			input >> path;
			server.AddDirRecursive(path);
		}),
		Item("find", "finds files", [&](auto& input) {
			std::string query;
			std::getline(input, query);
			server.Search(query);
		}),
		Item("find_batch", "finds files with many queries", [&](auto& input) {
			Path path;
			input >> path;
			server.SearchBatch(path);
		}),
		Item("remove_file", "removes file from index", [&](auto& input) {
			Path path;
			input >> path;
			server.RemoveFile(path);
		}),
		Item("remove_dir", "adds directory to index", [&](auto& input) {
			Path path;
			input >> path;
			server.RemoveDir(path);
		}),
		Item("remove_dir_recursive", "adds directory to index recursively", [&](auto& input) {
			Path path;
			input >> path;
			server.RemoveDirRecursive(path);
		}),
	});
	menu.AddItem("exit", "", [&](auto& input) { menu.Exit(); });
	menu.AddItem("help", "", [&](auto& input) { menu.ShowInstructions(); });

	menu.Run();

	return EXIT_SUCCESS;
}
