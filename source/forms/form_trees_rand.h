///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormTreeRand' on 2026-09-09 20:00:31
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormTreeRand' on 2026-09-09 20:00:31

#include <filesystem>
#include <memory>
#include "../wx_other.h"

class Terrain;
class FSarchive;
class SpellTreeRandomizerRules;
class SpellTreeRandomizerTerrain;



///////////////////////////////////////////////////////////////////////////////
/// Class FormEdit
///////////////////////////////////////////////////////////////////////////////
class FormTreeRand : public wxFrame
{
private:
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);

	void OnSelectTerrain(wxCommandEvent& event);
	void OnRuleGroupItemSelect(wxCommandEvent& event);
	void OnRulePupupOpen(wxMouseEvent& event);
	void OnRulePupup(wxCommandEvent& event);
	void OnSrcTreeClick(wxCommandEvent& event);
	void OnRandTreeClick(wxCommandEvent& event);
	void OnSelectSprite(wxCommandEvent& event);
	void OnCanvasRepaint(wxPaintEvent& event);
	void OnProbChange(wxPropertyGridEvent& event);
	void OnSelectProbSprite(wxPropertyGridEvent& event);
	void OnEndRuleLabelEdit(wxListEvent& event);
	void OnTreePupupOpen(wxMouseEvent& event);
	void OnTreePupup(wxCommandEvent& event);
	void OnProbabPupupOpen(wxMouseEvent& event);
	void OnProbabPupup(wxCommandEvent& event);

	wxString OnGetRuleGroupItem(long item_id);

	std::vector<std::shared_ptr<Terrain>> m_terrains;
	std::vector<std::shared_ptr<FSarchive>> m_terrain_fs;
	std::shared_ptr<wxBitmap> m_bmp;
	SpellTreeRandomizerRules *m_rules;
	SpellTreeRandomizerTerrain *m_rules_group;

	enum class PopupActions{
		ADD_RULE = 0,
		REM_RULE,
		RENAME_RULE,
		CLEAR_PROB,
		EQUAL_PROB,
		RAND_PROB,
		FIX_PROB,
		CLR_ALL,
		SEL_ALL,
		SEL_TOOL = 1000,
		CLR_TOOL = 2000
	};

protected:
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormTreeRand' on 2026-09-09 20:00:31
	enum
	{
		wxID_FORM_TREE_RAND = 6000,
		wxID_SBAR,
		wxID_MM_LOAD_PRESET,
		wxID_MM_SAVE_PRESET,
		wxID_MM_EXIT,
		wxID_CH_TERR_CLASS,
		wxID_LIST_RULES,
		wxID_LBOX_SRC_TREES,
		wxID_LBOX_TREES,
		wxID_PG_PROB,
		wxID_SPR_CANVAS,
		wxID_PG_CONFIG,
	};
	
	wxStatusBar* sbar;
	wxMenuBar* m_menubar2;
	wxMenu* m_menu6;
	wxStaticText* m_staticText46;
	wxChoice* chTerrClass;
	wxStaticText* m_staticText67;
	wxListCtrlVirtual* listRules;
	wxStaticText* m_staticText49;
	wxCheckListBox* lboxSourceTrees;
	wxStaticText* m_staticText44;
	wxCheckListBox* lboxTrees;
	wxStaticText* m_staticText45;
	wxPropertyGrid* pgProbab;
	wxStaticLine* m_staticline18;
	wxPanel* canvas;
	wxStaticLine* m_staticline20;
	wxStaticText* m_staticText48;
	wxPropertyGrid* pgConfig;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormTreeRand' on 2026-09-09 20:00:31

public:

	std::string m_last_error;

	FormTreeRand(wxWindow* parent,wxWindowID id = wxID_FORM_TREE_RAND,const wxString& title = _("Trees randomizer rules"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(936,643),long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL);
	~FormTreeRand();

	int SetTerrains(std::vector<std::shared_ptr<FSarchive>> terrains, SpellTreeRandomizerRules *rules);

};

