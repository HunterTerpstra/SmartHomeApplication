#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App); 

/**
 * Creates the GUI window and MainFrame to go inside the window
*/
bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("BULBasoar");
	
	mainFrame->SetClientSize(800, 600); //size of the created window
	mainFrame->Center();
	mainFrame->Show();
	return true;
}


