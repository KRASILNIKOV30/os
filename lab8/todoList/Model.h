#pragma once
#include "SignallingValue.h"
#include <functional>
#include <string>
#include <map>

struct Todo
{
	size_t id;
	std::string value;
};

using AddTodoSignal = Signal<void()>;
using DeleteTodoSignal = Signal<void()>;
using AddTodoSlot = AddTodoSignal::slot_type;
using DeleteTodoSlot = DeleteTodoSignal::slot_type;

class Model
{
public:
	Model(std::vector<std::string> const& todos)
	{
		for (const auto& todo : todos)
		{
			AddTodo(todo);
		}
	}

	void AddTodo(std::string const& text)
	{
		Todo todo{
			.id = m_id++,
			.value = text
		};
		m_todos.emplace(todo.id, todo);
		m_addTodoSignal();
	}

	void DeleteTodo(size_t id)
	{
		m_todos.erase(id);
		m_deleteTodoSignal();
	}

	ScopedConnection DoOnAddTodo(AddTodoSlot const& slot)
	{
		return m_addTodoSignal.connect(slot);
	}

	ScopedConnection DoOnDeleteTodo(DeleteTodoSlot const& slot)
	{
		return m_deleteTodoSignal.connect(slot);
	}

	void ForEachTodo(std::function<void(Todo const& todo)> callback) const
	{
		for (auto const& [id, todo] : m_todos)
		{
			callback(todo);
		}
	}

private:
	using Todos = std::map<size_t, Todo>;
	Todos m_todos{};
	AddTodoSignal m_addTodoSignal;
	DeleteTodoSignal m_deleteTodoSignal;
	size_t m_id = 0;
};
