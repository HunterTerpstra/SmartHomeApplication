#pragma once
#include <wx/wx.h>
#include <wx/listbook.h>
#include "logger.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <vector>
/**
 * @author Hunter Terpstra
 * @brief Creates a GUI for the user to interact with
 * The class creates a user interface that a user can add, remove, and change the values of lightbulbs
 * The class also displays the logger information to the user 
 */
class MainFrame : public wxFrame
{


public:
    /**
     * @brief Construct a new Main Frame object
     * creates the main page for the GUI
     * @param title 
     */
    MainFrame(const wxString& title);
    


private:

    /** 
     * @brief Event handler for slider 
     * @param Takes in a event (slider being changed)  
     * this method adjusts the brightness of the lightbulbs
    */
    void onSliderChanged(wxCommandEvent& evt);

    /** 
     * @brief event handler for the add page button 
     * @param Takes in a button event click as a parameter 
     * adds a lightbulb and updates the GUI to show new lightbulb
    */
    void onButtonClickedAdd(wxCommandEvent& evt);

    /** 
     * @brief event handler for remove page button 
     * @param Takes in a button event click as a parameter 
     * Removes a lightbulb and updates the GUI to show that the lightbulb has been removed
    */
    void onButtonClickedRemove(wxCommandEvent& evt);

    /**
     * @brief Event Handler for the editable text box 
     * @param takes in a change text event as a parameter
     * Changes the name of the lightbulb that is going to be added
    */
    void OnTextChangedAddLB(wxCommandEvent& evt);

    /**
     * @brief Event Handler for refreshing the logger
     * @param Takes in a button click event as a parameter
     * Updates the logger textbox with new logger information that may have been added since last refresh
    */
    void refreshLogger(wxCommandEvent& evt); 


    wxListbook* tabControl1; //keeps track of the added lightbulbs
    wxTextCtrl* loggerText; //text displayed at the bottom of the GUI when an action is made
    wxString lightBulbName; //Keeps track of current lightbulb name to be added
    std::string lightBulbNameString; //lightbulbname converted to a usable string from a wxString
    std::vector<std::string> lightbulbs; //vector of strings storing the names of the lightbulbs

    
	


};

