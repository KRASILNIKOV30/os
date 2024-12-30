#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <functional>

struct Student
{
    unsigned id;
    std::string name;
    std::string branch;
    unsigned enteringYear;
};

class StudentRepo 
{
public: 
    void Insert(Student student);
    void Remove(unsigned id);
    const Student* Get(unsigned id);
    void FindAllWithName(std::string_view name, std::function<bool(const Student*)> callback);

private:
    using StudentsMap = std::unordered_map<unsigned, Student>;
    using NamesMap = std::unordered_map<std::string_view, std::unordered_set<unsigned>>;
    
    StudentsMap m_studentsMap{};
    NamesMap m_namesMap{};
};


