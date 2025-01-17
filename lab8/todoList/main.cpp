#include "View.h"
#include <wx/snglinst.h>
#include <memory>

std::vector<std::string> LoadTodos()
{
	if (!fs::exists(STORAGE_PATH))
	{
		return {};
	}
	std::ifstream file(STORAGE_PATH);
	std::string line;
	std::vector<std::string> result;
	while (std::getline(file, line))
	{
		result.push_back(line);
	}
	return result;
}

class TodoApp : public wxApp
{
public:
	virtual bool OnInit() override
	{
		m_checker = std::make_unique<wxSingleInstanceChecker>("MyUniqueApp");
		if (m_checker->IsAnotherRunning())
		{
			wxMessageBox("Another program is running", "Error", wxICON_ERROR);
			return false;
		}
	
        const auto todos = LoadTodos();
        m_model = std::make_unique<Model>(todos);
        m_viewModel = std::make_unique<ViewModel>(m_model.get());
        auto* view = new View(m_viewModel.get());
		view->Show(true);

		return true;
	}

private:
	std::unique_ptr<wxSingleInstanceChecker> m_checker;
    std::unique_ptr<Model> m_model;
    std::unique_ptr<ViewModel> m_viewModel;
};

wxIMPLEMENT_APP(TodoApp);