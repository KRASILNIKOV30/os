#pragma once
#include "Model.h"
#include <optional>

class ViewModel
{
public:
	using ChangeSelectedIdSignal = Signal<void()>;
	using ChangeInputTextSignal = Signal<void(std::string)>;
	using ChangeSelectedIdSlot = ChangeSelectedIdSignal::slot_type;
	using ChangeInputTextSlot = ChangeInputTextSignal::slot_type;

	ViewModel(Model* model)
		: m_model(model)
	{
		m_addTodoSignalConnection = m_model->DoOnAddTodo([&]() {
			DoAddTodo();
		});
		m_deleteTodoSignalConnection = m_model->DoOnDeleteTodo([&]() {
			DoDeleteTodo();
		});
	}

    void AddTodo()
	{
		m_model->AddTodo(m_inputText.GetValue());
        m_inputText = "";
	}

    void DeleteTodo()
    {
        const auto selectedId = m_selectedId.GetValue();
        if (selectedId.has_value())
        {
            m_model->DeleteTodo(selectedId.value());
        }
        m_selectedId = std::nullopt;
    }

    void Input(std::string const& text)
    {
        m_inputText = text;
    }

    void SelectTodo(size_t id)
    {
        m_selectedId = id;
    }

    ScopedConnection DoOnChangeSelectedId(ChangeSelectedIdSlot const& slot)
    {
        return m_selectedId.Connect(slot, false);
    }

    ScopedConnection DoOnChangeInputText(ChangeInputTextSlot const& slot)
    {
        return m_inputText.Connect1(slot, false);
    }

    ScopedConnection DoOnAddTodo(AddTodoSlot const& slot)
    {
        return m_model->DoOnAddTodo(slot);
    }

    ScopedConnection DoOnDeleteTodo(DeleteTodoSlot const& slot)
    {
        return m_model->DoOnDeleteTodo(slot);
    }

    std::optional<size_t> GetSelectedTodoId() const
    {
        return m_selectedId.GetValue();
    }

    void ForEachTodo(std::function<void(Todo const& todo)> callback) const
    {
        m_model->ForEachTodo(callback);
    }

private:
	void DoAddTodo()
	{
		m_addTodoSignal();
	}

	void DoDeleteTodo()
    {
        m_deleteTodoSignal();
    }

private:
	Model* m_model;
	SignallingValue<std::optional<size_t>> m_selectedId;
	SignallingValue<std::string> m_inputText;
	ScopedConnection m_addTodoSignalConnection;
	ScopedConnection m_deleteTodoSignalConnection;
	AddTodoSignal m_addTodoSignal;
	DeleteTodoSignal m_deleteTodoSignal;
};
