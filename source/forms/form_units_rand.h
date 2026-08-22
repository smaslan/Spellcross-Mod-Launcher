///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormUnitRand' on 2026-08-22 09:02:19
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/checklst.h>
#include <wx/grid.h>
#include <wx/panel.h>
#include <wx/notebook.h>

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormUnitRand' on 2026-08-22 09:02:19

#include <filesystem>
//#include "../spell_randomizer.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class FormSaveEdit
///////////////////////////////////////////////////////////////////////////////

class FSarchive;
class SpellUnits;
class UnitRandomizerSetup;
class UnitRandomizerGlobRule;

class FormUnitRand : public wxFrame
{
private:
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnChangeUnitClass(wxCommandEvent& event);
	void OnChangeFilter(wxCommandEvent& event);
	void OnUnitsPupupOpen(wxMouseEvent& event);
	void OnUnitsPupup(wxCommandEvent& event);
	void OnSelectUnit(wxCommandEvent& event);
	void OnProbabPupupOpen(wxMouseEvent& event);
	void OnSrcUnitsPupupOpen(wxMouseEvent& event);
	void OnProbabPupup(wxCommandEvent& event);
	void OnPGoptionsChange(wxPropertyGridEvent& event);	
	void OnPGprobChange(wxPropertyGridEvent& event);
	void OnUpdateSrcUnits(wxCommandEvent& event);

	void LoadOptions();
	void LoadUnitList(wxCheckListBox* lbox);
	void UpdateWList();
	void FixProbs(int ref_id);

	std::unique_ptr<FSarchive> m_common_fs;
	std::unique_ptr<SpellUnits> m_units;
	std::vector<double> m_probab;

	enum class UnitsPopup : int{
		CLEAR,
		SET,
		SET_ARMOR,
		SET_LIGHT,
		SET_AIR,
		CLEAR_ARMOR,
		CLEAR_LIGHT,
		CLEAR_AIR,
		TOGGLE_ARMOR,
		TOGGLE_LIGHT,
		TOGGLE_AIR,
		SET_OS,
		SET_ALLIANCE,
		CLEAR_OS,
		CLEAR_ALLIANCE
	};

	enum class ProbabPopup : int {
		CLEAR,
		UNIFORM,
		RANDOM
	};

	UnitRandomizerSetup &m_randomizer;
	UnitRandomizerGlobRule *m_rand_rule;

protected:
	
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormUnitRand' on 2026-08-22 09:02:19
	enum
	{
		wxID_FORM_UNIT_RAND = 6000,
		wxID_SBAR,
		wxID_MM_LOAD_PRESET,
		wxID_MM_SAVE_PRESET,
		wxID_MM_EXIT,
		wxID_CH_UNIT_CLASS,
		wxID_LBOX_SRC_UNITS,
		wxID_LBOX_UNITS,
		wxID_CB_FILTER_SAVE,
		wxID_CH_FILTER,
		wxID_PG_PROB,
		wxID_PG_CONFIG,
	};
	
	wxStatusBar* sbar;
	wxMenuBar* m_menubar2;
	wxMenu* m_menu6;
	wxStaticText* m_staticText46;
	wxChoice* chUnitClass;
	wxStaticText* m_staticText49;
	wxCheckListBox* lboxSourceUnits;
	wxStaticText* m_staticText44;
	wxCheckListBox* lboxUnits;
	wxCheckBox* cbLimitSave;
	wxStaticText* m_staticText47;
	wxChoice* chFilter;
	wxStaticText* m_staticText45;
	wxPropertyGrid* pgProbab;
	wxStaticLine* m_staticline18;
	wxStaticText* m_staticText48;
	wxPropertyGrid* pgConfig;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormUnitRand' on 2026-08-22 09:02:19

public:

	std::string m_last_error;

	FormUnitRand(wxWindow* parent,UnitRandomizerSetup& randomizer,wxWindowID id = wxID_FORM_UNIT_RAND,const wxString& title = _("Units randomizer rules"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(825,643),long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL);
	~FormUnitRand();
	
	int SetCommon(std::filesystem::path common_path);

};

