/**
 * Main Author: Hunter Terpstra
 * The MainFrame class displays the GUI for the user to interact with
 * The user is able to add, remove, and change the values of lightbulbs
 * The user is also shown the logger information 
*/

#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/listbook.h>
#include "logger.h"
#include <sstream>
using namespace std;
#define PORT 80

/*
* Constructor for main GUI window
  sets up the main page for the user to interact with
*/
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	lightBulbName = "New LightBulb";
	lightBulbNameString = "New LightBulb";

	wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "BULBasoar Smart Light Application", wxPoint(100, 20), wxSize(600, 75), wxALIGN_CENTER);
	
	loggerText = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(400, 450), wxSize(350, 120), wxTE_MULTILINE|wxTE_READONLY);
	wxButton* loggerButton = new wxButton(panel, wxID_ANY, "Refresh Logger", wxPoint(525, 410), wxSize(100, 30));
	loggerButton->Bind(wxEVT_BUTTON, &MainFrame::refreshLogger, this);
	

	staticText->SetBackgroundColour(*wxLIGHT_GREY);
	staticText->SetFont(staticText->GetFont().Scale(2.5));

	tabControl1 = new wxListbook(panel, wxID_ANY, { 10, 100 }, { 690, 300 }); //listbook
	tabControl1->SetMinSize(wxSize(200, -1));

	//page panel in listbook
	wxPanel* panel2 = new wxPanel(tabControl1, wxID_ANY);
	

	//default pages
	tabControl1->AddPage(panel2, "Home Page");
	

	//changes the background color 
	panel->SetBackgroundColour(wxColour(211, 211, 211));
	panel2->SetBackgroundColour(wxColour(255, 255, 255));
	

	//changing name of lightbulb to be added box
	wxTextCtrl* addLB = new wxTextCtrl(panel, wxID_ANY,"", wxPoint(100, 500));
	
	addLB->Bind(wxEVT_TEXT, &MainFrame::OnTextChangedAddLB, this);

	//add and remove lightbulb buttons
	wxButton* buttonAddBulb = new wxButton(panel, wxID_ANY, "Add Bulb", wxPoint(100, 450), wxSize(100, 35));
	wxButton* buttonRemoveBulb = new wxButton(panel, wxID_ANY, "Remove Bulb", wxPoint(300, 450), wxSize(100, 35));

	buttonAddBulb->Bind(wxEVT_BUTTON, &MainFrame::onButtonClickedAdd, this);
	buttonRemoveBulb->Bind(wxEVT_BUTTON, &MainFrame::onButtonClickedRemove, this);

	CreateStatusBar();
}



/*
* Event handler for slider
  Takes in a event (slider being changed) 
  this method adjusts the brightness of the lightbulbs
*/
void MainFrame::onSliderChanged(wxCommandEvent& evt) {
	wxString str = wxString::Format("Slider Value: %d", evt.GetInt());
	wxLogStatus(str);

	wxString currNoteBookPage = tabControl1->GetPageText(tabControl1->GetSelection());
	ostringstream oss;
	
	oss << "PATCH /" << currNoteBookPage << "?value="<< evt.GetInt() << "HTTP/1.1";

	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* xmlFile = strdup(oss.str().c_str());
	

		char buffer[1024] = { 0 };
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			return;
		}
	
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);
	
		// Convert IPv4 and IPv6 addresses from text to binary
		// form
		if (inet_pton(AF_INET, "135.23.236.206", &serv_addr.sin_addr)
			<= 0) {
			printf(
				"\nInvalid address/ Address not supported \n");
			return;
		}
	
		if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr,
					sizeof(serv_addr)))
			< 0) {
			printf("\nConnection Failed \n");
			return;
		}
		send(sock, xmlFile, strlen(xmlFile), 0);
		printf("Http message sent\n");
		valread = read(sock, buffer, 1024);
		printf("%s\n", buffer);
	
		// closing the connected socket
		close(client_fd);


}

/*
* event handler for the add page button
  Takes in a button event click as a parameter
  adds a lightbulb and updates the GUI to show new lightbulb
*/
void MainFrame::onButtonClickedAdd(wxCommandEvent& evt) {
	
	wxLogStatus("Button Clicked Add");

	//creates a new page with brightness sliders that the user can interact with
	wxPanel* panel = new wxPanel(tabControl1, wxID_ANY);
	wxSlider* slider = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxPoint(150, 100), wxSize(300, -1));
	slider->Bind(wxEVT_SLIDER, &MainFrame::onSliderChanged, this);
	tabControl1->AddPage(panel, lightBulbName);
	wxStaticText* brightnessText = new wxStaticText(panel, wxID_ANY, "Brightness", wxPoint(205, 150), wxSize(300, 75));
	brightnessText->SetFont(brightnessText->GetFont().Scale(2.5));
	
	panel->SetBackgroundColour(wxColour(255, 255, 255));
	logger::instance().write("Central","Lightbulb added"); //updates the logger

	
	lightbulbs.push_back(lightBulbNameString);
	

	wxString currNoteBookPage = tabControl1->GetPageText(tabControl1->GetSelection());
	

	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;

	ostringstream oss;
	oss << "PUT /" << lightBulbNameString << " HTTP/1.1\n"
	"\n"
	"<?xml version=”1.0”?>"
	"<Node>"
	"<value>0.0</value>"
	"<threshold>1.0</threshold>"
	"<input>"
	"<name>node_one</name>"
	"<weight>0.5</weight>"
	"</input>"
	"<input>"
	"<name>node_two</name>"
	"<weight>0.5</weight>"
	"</input>"
	"<output>"
	"<name>node_three</name>"
	"<weight>0.5</weight>"
	"</output>"
	"</Node>";

	char* xmlFile = strdup(oss.str().c_str());
	
	cout << xmlFile << endl;
		char buffer[1024] = { 0 };
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			return;
		}
	
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);
	
		// Convert IPv4 and IPv6 addresses from text to binary
		// form
		if (inet_pton(AF_INET, "135.23.236.206", &serv_addr.sin_addr)
			<= 0) {
			printf(
				"\nInvalid address/ Address not supported \n");
			return;
		}
	
		if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr,
					sizeof(serv_addr)))
			< 0) {
			printf("\nConnection Failed \n");
			return;
		}
		send(sock, xmlFile, strlen(xmlFile), 0);
		printf("Http message sent\n");
		valread = read(sock, buffer, 1024);
		printf("%s\n", buffer);
	
		// closing the connected socket
		close(client_fd);



}

/*
* event handler for remove page button
  Takes in a button event click as a parameter
  Removes a lightbulb and updates the GUI to show that the lightbulb has been removed
*/
void MainFrame::onButtonClickedRemove(wxCommandEvent& evt) {
	wxLogStatus("Button Clicked Remove");

	wxString currNoteBookPage = tabControl1->GetPageText(tabControl1->GetSelection());
	ostringstream oss;
	
	oss << "DELETE /" << currNoteBookPage << " HTTP/1.1";

	int sel = tabControl1->GetSelection();
	if (sel != wxNOT_FOUND) {
		tabControl1->DeletePage(sel); //deletes current page selected
	}
	logger::instance().write("Central","Lightbulb removed");

	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	
	char* xmlFile = strdup(oss.str().c_str());
	

		char buffer[1024] = { 0 };
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			return;
		}
	
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);
	
		// Convert IPv4 and IPv6 addresses from text to binary
		// form
		if (inet_pton(AF_INET, "135.23.236.206", &serv_addr.sin_addr)
			<= 0) {
			printf(
				"\nInvalid address/ Address not supported \n");
			return;
		}
	
		if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr,
					sizeof(serv_addr)))
			< 0) {
			printf("\nConnection Failed \n");
			return;
		}
		send(sock, xmlFile, strlen(xmlFile), 0);
		printf("Http message sent\n");
		valread = read(sock, buffer, 1024);
		printf("%s\n", buffer);
	
		// closing the connected socket
		close(client_fd);

}


/**
 * Event Handler for refreshing the logger
 * Takes in a button click event as a parameter
 * Updates the logger textbox with new logger information that may have been added since last refresh
*/
void MainFrame::refreshLogger(wxCommandEvent& evt) {
	std::vector<log_message> tst = logger::instance().read_all();
	loggerText->Clear();
	
	for (int i=0; i< tst.size(); i++){
		loggerText->AppendText(tst.at(i).get_callerName() + ": ");
		loggerText->AppendText(tst.at(i).get_message());
		loggerText->AppendText("\n");
	}

}

/**
 * Event Handler for the editable text box 
 * takes in a change text event as a parameter
 * Changes the name of the lightbulb that is going to be added
*/
void MainFrame::OnTextChangedAddLB(wxCommandEvent& evt) {
	lightBulbName = evt.GetString(); //changes name of page to be inserted (lightbulb added)
	lightBulbNameString = lightBulbName.ToAscii();
	

}