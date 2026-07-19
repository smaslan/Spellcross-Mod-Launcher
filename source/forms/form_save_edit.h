///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormSaveEdit' on 2026-07-17 16:21:19
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
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/checklst.h>
#include <wx/grid.h>
#include <wx/panel.h>
#include <wx/notebook.h>

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormSaveEdit' on 2026-07-17 16:21:19
#include <wx/tglbtn.h>

#include <filesystem>
#include "../SpellSaves.h"
#include "../wx_other.h"

///////////////////////////////////////////////////////////////////////////


class SavesData : public wxClientData {
public:
	SavesData(std::filesystem::path path) 
		: m_path(path) {}
	std::filesystem::path m_path;
};

///////////////////////////////////////////////////////////////////////////////
/// Class FormSaveEdit
///////////////////////////////////////////////////////////////////////////////
class FormSaveEdit : public wxFrame
{
private:
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnResSelect(wxCommandEvent& event);
	void OnUpgSelect(wxCommandEvent& event);
	void OnUnitSelect(wxCommandEvent& event);
	void OnCommanderSelect(wxCommandEvent& event);
	void OnHierPartSelect(wxCommandEvent& event);
	void OnHierUnitListSelect(wxListEvent& event);
	void OnHierCommanderListSelect(wxListEvent& event);
	void OnHierUnitClick(wxCommandEvent& event);
	void OnHierComClick(wxCommandEvent& event);
	void OnHierComUnitClick(wxCommandEvent& event);
	void OnPaintBigmapCanvas(wxPaintEvent& event);
	void OnBigmapMouse(wxMouseEvent& event);
	void OnTerritorySelect(wxCommandEvent& event);
	void OnSortUnits(wxCommandEvent& event);
	void OnResetUnitNames(wxCommandEvent& event);
	void OnEditProp(wxPropertyGridEvent& event);
	void OnEditPupup(wxCommandEvent& event);
	void OnTerrPropChange(wxPropertyGridEvent& event);
	void OnLevelPropChange(wxPropertyGridEvent& event);
	void OnUnitPropChange(wxPropertyGridEvent& event);
	void OnSyncResearch(wxCommandEvent& event);
	void OnSyncUpgrades(wxCommandEvent& event);

	wxString OnGetUnitItem(long item_id);
	void OnUnitBeginDrag(wxListEvent& event);
	void OnUnitDragMotion(wxMouseEvent& event);
	void OnUnitEndDrag(wxMouseEvent& event);
	void OnUnitDragLost(wxMouseCaptureLostEvent& event);
	std::unique_ptr<wxDragImage> m_drag_unit_img;
	int m_drag_unit;
	
	void UpdateList();
	wxString OnGetHierUnitItem(long item_id);
	wxListItemAttr* OnGetHierUnitItemAttr(long item_id);
	wxString OnGetHierCommanderItem(long item_id);
	wxListItemAttr* OnGetHierCommanderItemAttr(long item_id);
	void FillHierarchy();

	std::filesystem::path m_common_fs_path;
	std::filesystem::path m_save_dir;
	bool m_is_mod;
	SpellSaveBigMap m_bigmap;
	int m_territory_mouse;

protected:
	
	const int wxID_CH_HIERARCH_UNIT_0 = 7000;
	const int wxID_CH_HIERARCH_COM1C_0 = 7100;
	const int wxID_CH_HIERARCH_COM1U_0 = 7150;
	const int wxID_CH_HIERARCH_COM2C_0 = 7200;
	const int wxID_CH_HIERARCH_COM2U_0 = 7250;
	const int wxID_CH_HIERARCH_COM3C_0 = 7300;
	const int wxID_CH_HIERARCH_COM3U_0 = 7350;

	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormSaveEdit' on 2026-07-17 16:21:19
	enum
	{
		wxID_FORM_SAVE_EDIT = 6000,
		wxID_MM_OPEN,
		wxID_MM_SAVE,
		wxID_MM_SAVE_AS,
		wxID_MM_EXIT,
		wxID_SBAR,
		wxID_CH_SAVES,
		wxID_BTN_LOAD_SPELL_SAVE,
		wxID_PAGE_CTRL,
		wxID_PAN_RESEARCH,
		wxID_LBOX_RES,
		wxID_BTN_RES_SYNC,
		wxID_LBL_RES_NAME,
		wxID_GRID_RES,
		wxID_GRID_RAW_RESEARCH,
		wxID_LBOX_UPG,
		wxID_BTN_UPG_SYNC,
		wxID_LBL_UPG_NAME,
		wxID_GRID_UPG,
		wxID_GRID_RAW_UPGRADES,
		wxID_PAN_UNITS,
		wxID_LIST_UNITS,
		wxID_BTN_UNIT_RESET_NAMES,
		wxID_BTN_UNIT_REM_GAPS,
		wxID_BTN_UNIT_SORT_PERM_REIN,
		wxID_BTN_UNIT_SORT_NAMES,
		wxID_BTN_UNIT_SORT_TYPES,
		wxID_GRID_UNITS,
		wxID_BTN_UNIT_RESET_NAME,
		wxID_GRID_UNIT_RAW,
		wxID_LBOX_COMANDERS,
		wxID_GRID_COMANDERS,
		wxID_PAN_HIERARCHY,
		wxID_CH_HEIR_PART,
		wxID_LIST_HIER_UNITS,
		wxID_LIST_HIER_COMMANDERS,
		wxID_PAN_BIGMAP,
		wxID_LBOX_TERRITORY,
		wxID_CANVAS_BIGMAP,
		wxID_GRID_TERR_PROPS,
		wxID_GRID_BIGMAP_RAW,
		wxID_PAN_LEVEL,
		wxID_GRID_LEVEL,
	};
	
	wxMenuBar* m_menubar4;
	wxMenu* mnufile;
	wxStatusBar* sbar;
	wxStaticText* m_staticText38;
	wxChoice* chSaves;
	wxBitmapButton* btnLoadSpellSave;
	wxNotebook* pageCtrl;
	wxPanel* panResearch;
	wxBoxSizer* szrUnitsC;
	wxBoxSizer* szrUnitsB;
	wxBoxSizer* szrUnitsA;
	wxStaticText* m_staticText21;
	wxListBox* listRes;
	wxButton* btnResSync;
	wxStaticText* lblResName;
	wxPropertyGrid* gridResProp;
	wxStaticText* m_staticText39;
	wxGrid* gridRawResearch;
	wxStaticLine* m_staticline11;
	wxStaticText* m_staticText211;
	wxListBox* listUpg;
	wxButton* btnUpgSync;
	wxStaticText* lblUpgName;
	wxPropertyGrid* gridUpgProp;
	wxStaticText* m_staticText40;
	wxGrid* gridRawUpgrades;
	wxPanel* panUnits;
	wxStaticText* m_staticText212;
	wxListCtrlVirtual* listUnits;
	wxButton* btnUnitsResetNames;
	wxStaticLine* m_staticline17;
	wxButton* btnUnitsNoGaps;
	wxButton* btnUnitsSortPermaReinforces;
	wxButton* btnUnitsSortNames;
	wxButton* btnUnitsSortTypes;
	wxStaticText* lblResName1;
	wxPropertyGrid* gridUnitProp;
	wxButton* btnUnitsResetName;
	wxStaticLine* m_staticline111;
	wxStaticText* m_staticText27;
	wxGrid* gridRawUnit;
	wxPanel* panCommanders;
	wxStaticText* m_staticText2121;
	wxListBox* listComanders;
	wxStaticText* lblResName11;
	wxPropertyGrid* gridCommanderProp;
	wxStaticLine* m_staticline1111;
	wxStaticText* m_staticText271;
	wxGrid* gridRawCommander;
	wxPanel* panHierarchy;
	wxStaticText* m_staticText31;
	wxChoice* chHierPart;
	wxStaticLine* m_staticline14;
	wxStaticText* m_staticText32;
	wxListCtrlVirtual* listHierUnits;
	wxStaticText* m_staticText321;
	wxListCtrlVirtual* listHierCommanders;
	wxPanel* panBigmap;
	wxBoxSizer* szrBigmap;
	wxStaticText* m_staticText41;
	wxListBox* listTerritory;
	wxPanel* canvasBigmap;
	wxStaticLine* m_staticline15;
	wxStaticText* m_staticText37;
	wxPropertyGrid* gridTerritoriesProp;
	wxStaticLine* m_staticline16;
	wxStaticText* m_staticText2711;
	wxGrid* gridRawBigmap;
	wxPanel* panLevel;
	wxStaticText* lblResName12;
	wxPropertyGrid* gridLevelProp;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormSaveEdit' on 2026-07-17 16:21:19

public:


	FormSaveEdit(wxWindow* parent,wxWindowID id = wxID_FORM_SAVE_EDIT,const wxString& title = _("Spellcross Save Game Editor"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(1200,750),long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL);
	~FormSaveEdit();

	void SetCommonPath(std::filesystem::path common_fs_path,bool is_mod);
	void SetSaveDir(std::filesystem::path save_dir);

};

