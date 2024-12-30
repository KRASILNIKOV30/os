#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include "studentrepo.h"
#include <functional>
#include <cstring>

struct Student
{
    unsigned id;
    std::string name;
    std::string branch;
    unsigned enteringYear;
};

struct String
{
    const char* chars;
    size_t length;
};

std::string ToString(SRString const& str)
{
    return std::string(str.chars, str.length);
}

bool ToSrStudent(Student const& s, SRStudent& srs)
{
    return SRMakeStudent(s.id, s.name.c_str(), s.branch.c_str(), s.enteringYear, srs);
}

Student ToStudent(SRStudent const& s)
{
    return Student{
        .id = s.id,
        .name = ToString(s.name),
        .branch = ToString(s.branch),
        .enteringYear = s.enteringYear,
    };
}

class SRStringWrapper
{
public:
    SRStringWrapper(std::string const& str)
    {
        SRMakeString(str.c_str(), m_str);
    }

    SRString& Get()
    {
        return m_str;
    }

    ~SRStringWrapper()
    {
        SRFreeString(&m_str);
    }

private:
    SRString m_str;
};

class StudentRepo 
{
public: 
    bool Insert(Student const& student)
    {
        SRStudent newStudent;
        if (!ToSrStudent(student, newStudent))
        {
            throw std::runtime_error("insertion failed");
        }
        return SRAddStudent(newStudent);
    }

    bool Remove(unsigned id)
    {
        return SRRemoveStudent(id);
    }

    const Student* Get(unsigned id)
    {
        auto student = SRGetStudent(id);
        if (student == nullptr)
        {
            return nullptr;
        }

        return new Student{
            .id = student->id,
            .name = ToString(student->name),
            .branch = ToString(student->branch),
            .enteringYear = student->enteringYear,
        };
    }

    void FindAllWithName(std::string const& name, std::function<bool(const Student*)> callback)
    {
        struct Context
        {
            static bool Callback(const SRStudent* student, void* self)
            {
                Context* ctx = static_cast<Context*>(self);
                auto s = ToStudent(*student);
                return ctx->callback(&s);
            }
            std::function<bool(const Student*)> callback;
        };
        Context c{std::move(callback)};
        SRStringWrapper srName(name);
        SRForEachStudentWithName(srName.Get(), &Context::Callback, &c);
    }

private:
    using StudentsMap = std::unordered_map<unsigned, Student>;
    using NamesMap = std::unordered_map<std::string_view, std::unordered_set<unsigned>>;
    
    StudentsMap m_studentsMap{};
    NamesMap m_namesMap{};
};

std::ostream& operator<<(std::ostream& out, const Student& s)
{
    return out << "{ id: " << s.id << ", name: " << s.name
        << ", branch: " << s.branch << ", enteringYear: " << s.enteringYear << "}";
}