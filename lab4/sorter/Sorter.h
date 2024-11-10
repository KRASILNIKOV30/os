#include <vector>
#include <istream>

class Sorter
{
public:
	Sorter(std::vector<uint64_t>& elements, unsigned threadsNum);
	void Sort();

private:
	void SortParts();
	void MergeParts();

	std::vector<uint64_t>& m_elements;
	unsigned m_threadsNum;
};

