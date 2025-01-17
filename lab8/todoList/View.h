#pragma once
#include "StoragePath.h"
#include "ViewModel.h"
#include <fstream>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

class View : public wxFrame
{
public:
	View(ViewModel* model)
		: wxFrame(nullptr, wxID_ANY, "Список дел", wxDefaultPosition, wxSize(400, 400))
		, m_model(model)
	{
		m_listBox = new wxListBox(this, wxID_ANY, wxPoint(10, 10), wxSize(250, 200));
		m_inputTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 220), wxSize(250, 30));
		m_addButton = new wxButton(this, wxID_ANY, "Add", wxPoint(270, 220), wxSize(100, 30));
		m_deleteButton = new wxButton(this, wxID_ANY, "Delete", wxPoint(270, 260), wxSize(100, 30));

		m_listBox->Bind(wxEVT_LISTBOX, &View::OnSelectionChanged, this);
		m_inputTextCtrl->Bind(wxEVT_TEXT, &View::OnInput, this);
		m_addButton->Bind(wxEVT_BUTTON, &View::AddTodo, this);
		m_deleteButton->Bind(wxEVT_BUTTON, &View::DeleteTodo, this);
		this->Bind(wxEVT_CLOSE_WINDOW, &View::OnClose, this);

		m_changeTextConnection = m_model->DoOnChangeInputText([&](std::string const& text) {
			m_inputTextCtrl->SetValue(text);
			m_addButton->Enable(!text.empty());
		});
		m_changeSelectedIdConnection = m_model->DoOnChangeSelectedId([&]() {
			UpdateTodoList();
		});
		m_addTodoSignalConnection = m_model->DoOnAddTodo([&]() {
			UpdateTodoList();
		});
		m_deleteTodoSignalConnection = m_model->DoOnDeleteTodo([&]() {
			UpdateTodoList();
		});

		UpdateTodoList();
		m_addButton->Enable(false);
		m_deleteButton->Enable(false);
	}

private:
	void UpdateTodoList()
	{
		m_listBox->Clear();
		const auto& selectedId = m_model->GetSelectedTodoId();
		m_deleteButton->Enable(selectedId.has_value());
		int i = 0;
		m_model->ForEachTodo([&](Todo const& todo) {
			m_listBox->Append(todo.value);
			if (selectedId.has_value() && todo.id == selectedId.value())
			{
				m_listBox->SetSelection(i);
			}
			i++;
		});
	}

	void AddTodo(wxCommandEvent& event)
	{
		m_model->AddTodo();
	}

	void DeleteTodo(wxCommandEvent& event)
	{
		m_model->DeleteTodo();
	}

	void OnInput(wxCommandEvent& event)
	{
		std::string text = m_inputTextCtrl->GetValue().ToStdString();
		m_model->Input(text);
	}

	void OnSelectionChanged(wxCommandEvent& event)
	{
		int selectedIndex = m_listBox->GetSelection();
		if (selectedIndex != wxNOT_FOUND)
		{
			int i = 0;
			m_model->ForEachTodo([&](Todo const& todo) {
				if (selectedIndex == i)
				{
					m_model->SelectTodo(todo.id);
				}
				i++;
			});
		}
	}

	void OnClose(wxCloseEvent& event)
	{
		std::ofstream file(STORAGE_PATH);
		m_model->ForEachTodo([&](Todo const& todo) {
			file << todo.value << std::endl;
		});
		event.Skip();
	}

private:
	ViewModel* m_model;
	wxListBox* m_listBox;
	wxTextCtrl* m_inputTextCtrl;
	wxButton* m_addButton;
	wxButton* m_deleteButton;
	ScopedConnection m_addTodoSignalConnection;
	ScopedConnection m_deleteTodoSignalConnection;
	ScopedConnection m_changeSelectedIdConnection;
	ScopedConnection m_changeTextConnection;
};
