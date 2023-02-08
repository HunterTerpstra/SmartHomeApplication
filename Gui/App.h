#pragma once
#include <wx/wx.h>

/**
 * @author
 * @brief class for initializing GUI application window
 * creates the GUI window for the user to interact with
 */
class App : public wxApp
{

public:
	/**
	 * @brief initializes the window application
	 * 
	 * @return true on success
	 * @return false on failure
	 */
	bool OnInit();
	

};

