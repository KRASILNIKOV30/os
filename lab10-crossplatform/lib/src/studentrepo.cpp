#include "StudentRepo.h"
#include <iostream>
#include <cstring>
#include "../include/studentrepo.h"

StudentRepo repo;

std::string ToString(SRString str)
{
	return std::string(str.chars, str.length);
}

Student ToStudent(SRStudent s)
{
	return Student{
		.id = s.id,
		.name = ToString(s.name),
		.branch = ToString(s.branch),
		.enteringYear = s.enteringYear,
	};
}

STUDENTREPO_LIBRARY_EXPORT bool SRMakeString(const char* chars, SRString& str)
{
	try
	{
		auto len = strlen(chars);
		auto newChars = new char[len + 1];
		std::copy_n(chars, len, newChars);
		newChars[len] = '\0';
		str.chars = newChars;
		str.length = len;
	}
	catch(const std::exception& e)
	{
		return false;
	}

	return true;
}

STUDENTREPO_LIBRARY_EXPORT void SRFreeString(SRString* str)
{
	delete[] str->chars;
}

STUDENTREPO_LIBRARY_EXPORT bool SRMakeStudent(unsigned id, const char* name, const char* branch, unsigned enteringYear, SRStudent& s)
{
	SRString srname;
	if (!SRMakeString(name, srname))
	{
		return false;
	}
	s.name = srname;

	SRString srbranch;
	if (!SRMakeString(branch, srbranch))
	{
		SRFreeString(&srname);
		return false;
	}
	s.branch = srbranch;
	s.id = id;
	s.enteringYear = enteringYear;
	
	return true;
}

STUDENTREPO_LIBRARY_EXPORT void SRFreeStudent(SRStudent* s)
{
	SRFreeString(&s->name);
	SRFreeString(&s->branch);
}

STUDENTREPO_LIBRARY_EXPORT bool SRAddStudent(SRStudent student)
{
	try
	{
		repo.Insert(ToStudent(student));
		SRFreeStudent(&student);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	
	return true;
}

STUDENTREPO_LIBRARY_EXPORT bool SRRemoveStudent(unsigned id)
{
	try
	{
		repo.Remove(id);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	
	return true;
}

STUDENTREPO_LIBRARY_EXPORT void SRForEachStudentWithName(SRString name, bool (*callback)(const SRStudent*, void*), void* context)
{
	try
	{
		repo.FindAllWithName(ToString(name), std::function([&](const Student* s) {
			SRStudent student;
			SRMakeStudent(s->id, s->name.c_str(), s->branch.c_str(), s->enteringYear, student);
			return callback(&student, context);
		}));
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

STUDENTREPO_LIBRARY_EXPORT const SRStudent* SRGetStudent(unsigned id)
{
	try
	{
		const auto* s = repo.Get(id);
		if (s == nullptr)
		{
			return nullptr;
		}
		auto student = new SRStudent;
		SRMakeStudent(s->id, s->name.c_str(), s->branch.c_str(), s->enteringYear, *student);

		return student;
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}


    