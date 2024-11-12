#include <cstddef>

template<typename T>
class ISet
{
public:
	virtual void Insert(T const& item) = 0;
	virtual T Get(size_t i) const = 0;
	virtual ~ISet() = default;
};
