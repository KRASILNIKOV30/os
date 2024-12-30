#include "StudentRepo.h"
#include <stdexcept>

void StudentRepo::Insert(Student student)
{
	auto id = student.id;
    if (m_studentsMap.contains(id))
    {
        throw std::invalid_argument("student already exist");
    }

    try
    {
        auto it = m_studentsMap.insert({ id, student });
        m_namesMap[std::string_view(it.first->second.name)].insert(id);
    }
    catch(std::exception const& e)
    {
        Remove(id);
        throw e;
    }
}

void StudentRepo::Remove(unsigned id)
{
	auto it = m_studentsMap.find(id);
    if (it == m_studentsMap.end())
    {
        throw std::invalid_argument("student does not exist");
    }

    auto& name = it->second.name;
    auto namesIt = m_namesMap.find(name);
    if (namesIt != m_namesMap.end())
    {
        namesIt->second.erase(id);
    }

    m_studentsMap.erase(it);
}

const Student* StudentRepo::Get(unsigned id)
{
    auto it = m_studentsMap.find(id);
    if (it == m_studentsMap.end())
    {
        return nullptr;
    }
	return &(it->second);
}

void StudentRepo::FindAllWithName(std::string_view name, std::function<bool(const Student*)> callback)
{
    auto names = m_namesMap.find(name);
    if (names == m_namesMap.end())
    {
        return;
    }

    for (auto& id : names->second)
    {
        auto it = m_studentsMap.find(id);
        if (it != m_studentsMap.end())
        {
            if (!callback(&(it->second)))
            {
                return;
            }
        }
    }
}
