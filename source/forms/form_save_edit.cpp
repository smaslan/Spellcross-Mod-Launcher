///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/rawbmp.h>
#include <wx/dcclient.h>

#include <functional>
#include <math.h>
#include "form_save_edit.h"
#include "../other.h"


///////////////////////////////////////////////////////////////////////////

FormSaveEdit::FormSaveEdit( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormSaveEdit' on 2026-06-04 18:52:31
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	m_menubar4 = new wxMenuBar( 0 );
	mnufile = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem( mnufile, wxID_MM_OPEN, wxString( _("Open savegame") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	mnufile->Append( mmOpen );
	
	wxMenuItem* mmSave;
	mmSave = new wxMenuItem( mnufile, wxID_MM_SAVE, wxString( _("Save savegame") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mnufile->Append( mmSave );
	
	wxMenuItem* mmSaveAs;
	mmSaveAs = new wxMenuItem( mnufile, wxID_MM_SAVE_AS, wxString( _("Save savegame as") ) , wxEmptyString, wxITEM_NORMAL );
	mnufile->Append( mmSaveAs );
	
	mnufile->AppendSeparator();
	
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mnufile, wxID_MM_EXIT, wxString( _("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mnufile->Append( mmExit );
	
	m_menubar4->Append( mnufile, _("File") );
	
	this->SetMenuBar( m_menubar4 );
	
	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SBAR );
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText38 = new wxStaticText( this, wxID_ANY, _("Spellcross save games:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	bSizer43->Add( m_staticText38, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer731;
	bSizer731 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString chSavesChoices;
	chSaves = new wxChoice( this, wxID_CH_SAVES, wxDefaultPosition, wxDefaultSize, chSavesChoices, 0 );
	chSaves->SetSelection( 0 );
	bSizer731->Add( chSaves, 1, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	btnLoadSpellSave = new wxBitmapButton( this, wxID_BTN_LOAD_SPELL_SAVE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );
	bSizer731->Add( btnLoadSpellSave, 0, wxRIGHT|wxLEFT, 5 );
	
	
	bSizer43->Add( bSizer731, 0, wxEXPAND, 5 );
	
	pageCtrl = new wxNotebook( this, wxID_PAGE_CTRL, wxDefaultPosition, wxDefaultSize, 0 );
	panResearch = new wxPanel( pageCtrl, wxID_PAN_RESEARCH, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer46;
	bSizer46 = new wxBoxSizer( wxVERTICAL );
	
	bSizer46->SetMinSize( wxSize( 200,-1 ) );
	m_staticText21 = new wxStaticText( panResearch, wxID_ANY, _("Research list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer46->Add( m_staticText21, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	listRes = new wxListBox( panResearch, wxID_LBOX_RES, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer46->Add( listRes, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer44->Add( bSizer46, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer47;
	bSizer47 = new wxBoxSizer( wxVERTICAL );
	
	bSizer47->SetMinSize( wxSize( 300,-1 ) );
	lblResName = new wxStaticText( panResearch, wxID_LBL_RES_NAME, _("Research:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblResName->Wrap( -1 );
	bSizer47->Add( lblResName, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridResProp = new wxPropertyGrid(panResearch, wxID_GRID_RES, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer47->Add( gridResProp, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer44->Add( bSizer47, 0, wxEXPAND, 5 );
	
	m_staticline11 = new wxStaticLine( panResearch, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer44->Add( m_staticline11, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer461;
	bSizer461 = new wxBoxSizer( wxVERTICAL );
	
	bSizer461->SetMinSize( wxSize( 200,-1 ) );
	m_staticText211 = new wxStaticText( panResearch, wxID_ANY, _("Upgrade list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	bSizer461->Add( m_staticText211, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	listUpg = new wxListBox( panResearch, wxID_LBOX_UPG, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer461->Add( listUpg, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer44->Add( bSizer461, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer471;
	bSizer471 = new wxBoxSizer( wxVERTICAL );
	
	bSizer471->SetMinSize( wxSize( 300,-1 ) );
	lblUpgName = new wxStaticText( panResearch, wxID_LBL_UPG_NAME, _("Research:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblUpgName->Wrap( -1 );
	bSizer471->Add( lblUpgName, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridUpgProp = new wxPropertyGrid(panResearch, wxID_GRID_UPG, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer471->Add( gridUpgProp, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer44->Add( bSizer471, 1, wxEXPAND, 5 );
	
	
	panResearch->SetSizer( bSizer44 );
	panResearch->Layout();
	bSizer44->Fit( panResearch );
	pageCtrl->AddPage( panResearch, _("Research"), false );
	panUnits = new wxPanel( pageCtrl, wxID_PAN_UNITS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer441;
	bSizer441 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer462;
	bSizer462 = new wxBoxSizer( wxVERTICAL );
	
	bSizer462->SetMinSize( wxSize( 200,-1 ) );
	m_staticText212 = new wxStaticText( panUnits, wxID_ANY, _("Units list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText212->Wrap( -1 );
	bSizer462->Add( m_staticText212, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	listUnits = new wxListBox( panUnits, wxID_LBOX_UNITS, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer462->Add( listUnits, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	btnUnitsResetNames = new wxButton( panUnits, wxID_BTN_UNIT_RESET_NAMES, _("Reset names"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer462->Add( btnUnitsResetNames, 0, wxALL|wxEXPAND, 5 );
	
	m_staticline17 = new wxStaticLine( panUnits, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer462->Add( m_staticline17, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	btnUnitsNoGaps = new wxButton( panUnits, wxID_BTN_UNIT_REM_GAPS, _("Remove gaps"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer462->Add( btnUnitsNoGaps, 0, wxEXPAND|wxALL, 5 );
	
	btnUnitsSortPermaReinforces = new wxButton( panUnits, wxID_BTN_UNIT_SORT_PERM_REIN, _("Sort Permanent-Reinforces"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer462->Add( btnUnitsSortPermaReinforces, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	btnUnitsSortNames = new wxButton( panUnits, wxID_BTN_UNIT_SORT_NAMES, _("Sort by Names"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer462->Add( btnUnitsSortNames, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	btnUnitsSortTypes = new wxButton( panUnits, wxID_BTN_UNIT_SORT_TYPES, _("Sort by Types"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer462->Add( btnUnitsSortTypes, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer441->Add( bSizer462, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer472;
	bSizer472 = new wxBoxSizer( wxVERTICAL );
	
	bSizer472->SetMinSize( wxSize( 300,-1 ) );
	lblResName1 = new wxStaticText( panUnits, wxID_LBL_RES_NAME, _("Parameters:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblResName1->Wrap( -1 );
	bSizer472->Add( lblResName1, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridUnitProp = new wxPropertyGrid(panUnits, wxID_GRID_UNITS, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer472->Add( gridUnitProp, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	btnUnitsResetName = new wxButton( panUnits, wxID_BTN_UNIT_RESET_NAME, _("Reset Unit Name"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer472->Add( btnUnitsResetName, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer441->Add( bSizer472, 0, wxEXPAND, 5 );
	
	m_staticline111 = new wxStaticLine( panUnits, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer441->Add( m_staticline111, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer52;
	bSizer52 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText27 = new wxStaticText( panUnits, wxID_ANY, _("Raw data:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	bSizer52->Add( m_staticText27, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridRawUnit = new wxGrid( panUnits, wxID_GRID_UNIT_RAW, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	gridRawUnit->CreateGrid( 5, 16 );
	gridRawUnit->EnableEditing( true );
	gridRawUnit->EnableGridLines( true );
	gridRawUnit->EnableDragGridSize( false );
	gridRawUnit->SetMargins( 0, 0 );
	
	// Columns
	gridRawUnit->SetColSize( 0, 38 );
	gridRawUnit->SetColSize( 1, 45 );
	gridRawUnit->SetColSize( 2, 39 );
	gridRawUnit->SetColSize( 3, 39 );
	gridRawUnit->SetColSize( 4, 32 );
	gridRawUnit->SetColSize( 5, 35 );
	gridRawUnit->SetColSize( 6, 35 );
	gridRawUnit->SetColSize( 7, 39 );
	gridRawUnit->EnableDragColMove( false );
	gridRawUnit->EnableDragColSize( false );
	gridRawUnit->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	
	// Rows
	gridRawUnit->EnableDragRowSize( false );
	gridRawUnit->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );
	
	// Label Appearance
	
	// Cell Defaults
	gridRawUnit->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_TOP );
	bSizer52->Add( gridRawUnit, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer441->Add( bSizer52, 1, wxEXPAND, 5 );
	
	
	panUnits->SetSizer( bSizer441 );
	panUnits->Layout();
	bSizer441->Fit( panUnits );
	pageCtrl->AddPage( panUnits, _("Units"), true );
	panCommanders = new wxPanel( pageCtrl, wxID_PAN_UNITS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4411;
	bSizer4411 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4621;
	bSizer4621 = new wxBoxSizer( wxVERTICAL );
	
	bSizer4621->SetMinSize( wxSize( 200,-1 ) );
	m_staticText2121 = new wxStaticText( panCommanders, wxID_ANY, _("Commanders list:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2121->Wrap( -1 );
	bSizer4621->Add( m_staticText2121, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	listComanders = new wxListBox( panCommanders, wxID_LBOX_COMANDERS, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer4621->Add( listComanders, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer4411->Add( bSizer4621, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4721;
	bSizer4721 = new wxBoxSizer( wxVERTICAL );
	
	bSizer4721->SetMinSize( wxSize( 300,-1 ) );
	lblResName11 = new wxStaticText( panCommanders, wxID_LBL_RES_NAME, _("Parameters:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblResName11->Wrap( -1 );
	bSizer4721->Add( lblResName11, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridCommanderProp = new wxPropertyGrid(panCommanders, wxID_GRID_COMANDERS, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer4721->Add( gridCommanderProp, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer4411->Add( bSizer4721, 0, wxEXPAND, 5 );
	
	m_staticline1111 = new wxStaticLine( panCommanders, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer4411->Add( m_staticline1111, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer521;
	bSizer521 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText271 = new wxStaticText( panCommanders, wxID_ANY, _("Raw data:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText271->Wrap( -1 );
	bSizer521->Add( m_staticText271, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridRawCommander = new wxGrid( panCommanders, wxID_GRID_UNIT_RAW, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	gridRawCommander->CreateGrid( 3, 16 );
	gridRawCommander->EnableEditing( true );
	gridRawCommander->EnableGridLines( true );
	gridRawCommander->EnableDragGridSize( false );
	gridRawCommander->SetMargins( 0, 0 );
	
	// Columns
	gridRawCommander->SetColSize( 0, 38 );
	gridRawCommander->SetColSize( 1, 45 );
	gridRawCommander->SetColSize( 2, 39 );
	gridRawCommander->SetColSize( 3, 39 );
	gridRawCommander->SetColSize( 4, 32 );
	gridRawCommander->SetColSize( 5, 35 );
	gridRawCommander->SetColSize( 6, 35 );
	gridRawCommander->SetColSize( 7, 39 );
	gridRawCommander->EnableDragColMove( false );
	gridRawCommander->EnableDragColSize( false );
	gridRawCommander->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	
	// Rows
	gridRawCommander->EnableDragRowSize( false );
	gridRawCommander->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );
	
	// Label Appearance
	
	// Cell Defaults
	gridRawCommander->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_TOP );
	bSizer521->Add( gridRawCommander, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer4411->Add( bSizer521, 1, wxEXPAND, 5 );
	
	
	panCommanders->SetSizer( bSizer4411 );
	panCommanders->Layout();
	bSizer4411->Fit( panCommanders );
	pageCtrl->AddPage( panCommanders, _("Commanders"), false );
	panHierarchy = new wxPanel( pageCtrl, wxID_PAN_HIERARCHY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* szHierA;
	szHierA = new wxBoxSizer( wxVERTICAL );
	
	m_staticText31 = new wxStaticText( panHierarchy, wxID_ANY, _("Hierarchy part:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	szHierA->Add( m_staticText31, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chHierPartChoices;
	chHierPart = new wxChoice( panHierarchy, wxID_CH_HEIR_PART, wxDefaultPosition, wxDefaultSize, chHierPartChoices, 0 );
	chHierPart->SetSelection( 0 );
	chHierPart->SetMinSize( wxSize( 200,-1 ) );
	
	szHierA->Add( chHierPart, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticline14 = new wxStaticLine( panHierarchy, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	szHierA->Add( m_staticline14, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* szHierB;
	szHierB = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* szHierUnits;
	szHierUnits = new wxBoxSizer( wxVERTICAL );
	
	szHierUnits->SetMinSize( wxSize( 200,-1 ) );
	
	szHierB->Add( szHierUnits, 0, wxEXPAND, 5 );
	
	wxBoxSizer* szHierCom1;
	szHierCom1 = new wxBoxSizer( wxVERTICAL );
	
	szHierCom1->SetMinSize( wxSize( 200,-1 ) );
	
	szHierB->Add( szHierCom1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* szHierCom2;
	szHierCom2 = new wxBoxSizer( wxVERTICAL );
	
	szHierCom2->SetMinSize( wxSize( 200,-1 ) );
	
	szHierB->Add( szHierCom2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* szHierCom3;
	szHierCom3 = new wxBoxSizer( wxVERTICAL );
	
	szHierCom3->SetMinSize( wxSize( 200,-1 ) );
	
	szHierB->Add( szHierCom3, 0, wxEXPAND, 5 );
	
	
	szHierB->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer63;
	bSizer63 = new wxBoxSizer( wxVERTICAL );
	
	bSizer63->SetMinSize( wxSize( 200,-1 ) );
	m_staticText32 = new wxStaticText( panHierarchy, wxID_ANY, _("Unassigned units:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	bSizer63->Add( m_staticText32, 0, wxRIGHT|wxLEFT, 5 );
	
	listHierUnits = new wxListCtrlVirtual(panHierarchy,wxID_LIST_HIER_UNITS,wxDefaultPosition,wxSize(200,-1),wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL|wxALWAYS_SHOW_SB|wxVSCROLL);
	bSizer63->Add( listHierUnits, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	szHierB->Add( bSizer63, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer631;
	bSizer631 = new wxBoxSizer( wxVERTICAL );
	
	bSizer631->SetMinSize( wxSize( 200,-1 ) );
	m_staticText321 = new wxStaticText( panHierarchy, wxID_ANY, _("Unassigned commanders:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText321->Wrap( -1 );
	bSizer631->Add( m_staticText321, 0, wxRIGHT|wxLEFT, 5 );
	
	listHierCommanders = new wxListCtrlVirtual(panHierarchy,wxID_LIST_HIER_COMMANDERS,wxDefaultPosition,wxSize(200,-1),wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL|wxALWAYS_SHOW_SB|wxVSCROLL);
	bSizer631->Add( listHierCommanders, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	szHierB->Add( bSizer631, 0, wxEXPAND, 5 );
	
	
	szHierA->Add( szHierB, 1, wxEXPAND, 5 );
	
	
	szHierA->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	panHierarchy->SetSizer( szHierA );
	panHierarchy->Layout();
	szHierA->Fit( panHierarchy );
	pageCtrl->AddPage( panHierarchy, _("Hierarchy"), false );
	panBigmap = new wxPanel( pageCtrl, wxID_PAN_BIGMAP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44111;
	bSizer44111 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer75;
	bSizer75 = new wxBoxSizer( wxVERTICAL );
	
	bSizer75->SetMinSize( wxSize( 200,-1 ) );
	m_staticText41 = new wxStaticText( panBigmap, wxID_ANY, _("Bigmap territories:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	bSizer75->Add( m_staticText41, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	listTerritory = new wxListBox( panBigmap, wxID_LBOX_TERRITORY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer75->Add( listTerritory, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer44111->Add( bSizer75, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer46211;
	bSizer46211 = new wxBoxSizer( wxVERTICAL );
	
	canvasBigmap = new wxPanel( panBigmap, wxID_CANVAS_BIGMAP, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer46211->Add( canvasBigmap, 1, wxEXPAND | wxALL, 5 );
	
	m_staticline15 = new wxStaticLine( panBigmap, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer46211->Add( m_staticline15, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer73;
	bSizer73 = new wxBoxSizer( wxVERTICAL );
	
	bSizer73->SetMinSize( wxSize( 300,-1 ) );
	m_staticText37 = new wxStaticText( panBigmap, wxID_ANY, _("Territory properties:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	bSizer73->Add( m_staticText37, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridTerritoriesProp = new wxPropertyGrid(panBigmap, wxID_GRID_TERR_PROPS, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer73->Add( gridTerritoriesProp, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer71->Add( bSizer73, 1, wxEXPAND, 5 );
	
	m_staticline16 = new wxStaticLine( panBigmap, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer71->Add( m_staticline16, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer72;
	bSizer72 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText2711 = new wxStaticText( panBigmap, wxID_ANY, _("Raw data:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2711->Wrap( -1 );
	bSizer72->Add( m_staticText2711, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridRawBigmap = new wxGrid( panBigmap, wxID_GRID_BIGMAP_RAW, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	gridRawBigmap->CreateGrid( 4, 16 );
	gridRawBigmap->EnableEditing( true );
	gridRawBigmap->EnableGridLines( true );
	gridRawBigmap->EnableDragGridSize( false );
	gridRawBigmap->SetMargins( 0, 0 );
	
	// Columns
	gridRawBigmap->SetColSize( 0, 38 );
	gridRawBigmap->SetColSize( 1, 45 );
	gridRawBigmap->SetColSize( 2, 39 );
	gridRawBigmap->SetColSize( 3, 39 );
	gridRawBigmap->SetColSize( 4, 32 );
	gridRawBigmap->SetColSize( 5, 35 );
	gridRawBigmap->SetColSize( 6, 35 );
	gridRawBigmap->SetColSize( 7, 39 );
	gridRawBigmap->EnableDragColMove( false );
	gridRawBigmap->EnableDragColSize( false );
	gridRawBigmap->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	
	// Rows
	gridRawBigmap->EnableDragRowSize( false );
	gridRawBigmap->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );
	
	// Label Appearance
	
	// Cell Defaults
	gridRawBigmap->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_TOP );
	bSizer72->Add( gridRawBigmap, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer71->Add( bSizer72, 1, wxEXPAND, 5 );
	
	
	bSizer46211->Add( bSizer71, 1, wxEXPAND, 5 );
	
	
	bSizer44111->Add( bSizer46211, 1, wxEXPAND, 5 );
	
	
	panBigmap->SetSizer( bSizer44111 );
	panBigmap->Layout();
	bSizer44111->Fit( panBigmap );
	pageCtrl->AddPage( panBigmap, _("Big Map"), false );
	panLevel = new wxPanel( pageCtrl, wxID_PAN_LEVEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4412;
	bSizer4412 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4722;
	bSizer4722 = new wxBoxSizer( wxVERTICAL );
	
	lblResName12 = new wxStaticText( panLevel, wxID_LBL_RES_NAME, _("Parameters:"), wxDefaultPosition, wxDefaultSize, 0 );
	lblResName12->Wrap( -1 );
	bSizer4722->Add( lblResName12, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	gridLevelProp = new wxPropertyGrid(panLevel, wxID_GRID_LEVEL, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer4722->Add( gridLevelProp, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer4412->Add( bSizer4722, 1, wxEXPAND, 5 );
	
	
	panLevel->SetSizer( bSizer4412 );
	panLevel->Layout();
	bSizer4412->Fit( panLevel );
	pageCtrl->AddPage( panLevel, _("Level"), false );
	
	bSizer43->Add( pageCtrl, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer43 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormSaveEdit' on 2026-06-04 18:52:31

	mmSave->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmSaveAs->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmOpen->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));
	mmExit->SetBitmaps(LoadSVGiconsBundle("IDR_EXIT"));

	auto op_sz = FromDIP(wxSize(16,16));
	btnLoadSpellSave->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnLoadSpellSave->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormSaveEdit::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormSaveEdit::OnOpen,this,wxID_MM_OPEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormSaveEdit::OnSave,this,wxID_MM_SAVE);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormSaveEdit::OnSave,this,wxID_MM_SAVE_AS);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnOpen,this,wxID_BTN_LOAD_SPELL_SAVE);
	

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSaveEdit::OnResSelect,this,wxID_LBOX_RES);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSaveEdit::OnUpgSelect,this,wxID_LBOX_UPG);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSaveEdit::OnUnitSelect,this,wxID_LBOX_UNITS);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSaveEdit::OnCommanderSelect,this,wxID_LBOX_COMANDERS);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSaveEdit::OnTerritorySelect,this,wxID_LBOX_TERRITORY);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSaveEdit::OnHierPartSelect,this,wxID_CH_HEIR_PART);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnSortUnits,this,wxID_BTN_UNIT_REM_GAPS);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnSortUnits,this,wxID_BTN_UNIT_SORT_PERM_REIN);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnSortUnits,this,wxID_BTN_UNIT_SORT_TYPES);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnSortUnits,this,wxID_BTN_UNIT_SORT_NAMES);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnResetUnitNames,this,wxID_BTN_UNIT_RESET_NAMES);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnResetUnitNames,this,wxID_BTN_UNIT_RESET_NAME);

	//Bind(wxEVT_PG_RIGHT_CLICK,&FormSaveEdit::OnEditProp,this,wxID_GRID_TERR_PROPS);
	Bind(wxEVT_PG_CHANGED,&FormSaveEdit::OnTerrPropChange,this,wxID_GRID_TERR_PROPS);
	Bind(wxEVT_PG_CHANGED,&FormSaveEdit::OnLevelPropChange,this,wxID_GRID_LEVEL);
	Bind(wxEVT_PG_CHANGED,&FormSaveEdit::OnUnitPropChange,this,wxID_GRID_UNITS);
	Bind(wxEVT_PG_CHANGED,&FormSaveEdit::OnUnitPropChange,this,wxID_GRID_COMANDERS);
	
	
	
	
	listHierUnits->SetGetItemTextCb(std::bind(&FormSaveEdit::OnGetHierUnitItem,this,std::placeholders::_1));
	listHierUnits->SetGetItemAttrCb(std::bind(&FormSaveEdit::OnGetHierUnitItemAttr,this,std::placeholders::_1));
	Bind(wxEVT_LIST_ITEM_SELECTED,&FormSaveEdit::OnHierUnitListSelect,this,wxID_LIST_HIER_UNITS);
	Bind(wxEVT_LIST_ITEM_ACTIVATED,&FormSaveEdit::OnHierUnitListSelect,this,wxID_LIST_HIER_UNITS);

	listHierCommanders->SetGetItemTextCb(std::bind(&FormSaveEdit::OnGetHierCommanderItem,this,std::placeholders::_1));
	listHierCommanders->SetGetItemAttrCb(std::bind(&FormSaveEdit::OnGetHierCommanderItemAttr,this,std::placeholders::_1));
	Bind(wxEVT_LIST_ITEM_SELECTED,&FormSaveEdit::OnHierCommanderListSelect,this,wxID_LIST_HIER_COMMANDERS);
	Bind(wxEVT_LIST_ITEM_ACTIVATED,&FormSaveEdit::OnHierCommanderListSelect,this,wxID_LIST_HIER_COMMANDERS);

	
	
	canvasBigmap->SetDoubleBuffered(true);
	canvasBigmap->Bind(wxEVT_PAINT,&FormSaveEdit::OnPaintBigmapCanvas,this,wxID_CANVAS_BIGMAP);
	canvasBigmap->Bind(wxEVT_MOTION,&FormSaveEdit::OnBigmapMouse,this,wxID_CANVAS_BIGMAP);
	canvasBigmap->Bind(wxEVT_LEFT_DOWN,&FormSaveEdit::OnBigmapMouse,this,wxID_CANVAS_BIGMAP);

	// init raw data viewers
	std::vector<wxGrid*> hex = {gridRawUnit, gridRawCommander, gridRawBigmap};
	for(auto grid: hex)
	{
		grid->SetRowLabelSize(40);
		for(int k = 0; k < grid->GetNumberCols(); k++)
		{
			grid->SetColLabelValue(k, string_format("x%Xh",k));
			grid->SetColSize(k, 30);
		}
		for(int k = 0; k < grid->GetNumberRows(); k++)
		{
			grid->SetRowLabelValue(k,string_format("%Xxh",k));
		}
	}		
	
	for(int k = 0; k < 16; k++)
	{
		wxArrayString no_choices;
		auto choice = new wxButton(panHierarchy,wxID_CH_HIERARCH_UNIT_0 + k,"",wxDefaultPosition,wxDefaultSize,0);
		choice->Enable(true);
		choice->Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnHierUnitClick,this,wxID_CH_HIERARCH_UNIT_0 + k);
		szHierUnits->Add(choice,0,wxEXPAND|wxALL,3);
	}
	szHierUnits->Layout();
	std::vector<int> wx_id_com_list = {wxID_CH_HIERARCH_COM1C_0, wxID_CH_HIERARCH_COM2C_0, wxID_CH_HIERARCH_COM3C_0};
	std::vector<int> wx_id_unit_list = {wxID_CH_HIERARCH_COM1U_0, wxID_CH_HIERARCH_COM2U_0, wxID_CH_HIERARCH_COM3U_0};
	std::vector<wxBoxSizer*> hier_size_list = {szHierCom1, szHierCom2, szHierCom3};
	for(int lev = 0; lev < 3; lev++)
	{
		auto sizer = hier_size_list[lev];
		int com_count = 4/(1<<lev);
		for(int k = 0; k < com_count; k++)
		{
			wxArrayString no_choices;
			sizer->Add(0,0,wxEXPAND,3);
			auto choice = new wxButton(panHierarchy,wx_id_com_list[lev] + k,"",wxDefaultPosition,wxDefaultSize,0);
			choice->Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveEdit::OnHierComClick,this,wx_id_com_list[lev] + k);
			sizer->Add(choice,0,wxEXPAND|wxALL,3);
			auto uchoice = new wxToggleButton(panHierarchy,wx_id_unit_list[lev] + k,"",wxDefaultPosition,wxDefaultSize,0);
			uchoice->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,&FormSaveEdit::OnHierComUnitClick,this,wx_id_unit_list[lev] + k);
			sizer->Add(uchoice,0,wxEXPAND|wxALL,3);
			sizer->Add(0,0,wxEXPAND,3);
		}
		sizer->Layout();
	}
	szHierA->Layout();
	szHierB->Layout();	

	chHierPart->Clear();
	chHierPart->Append("Part 1");
	chHierPart->Append("Part 2");
	chHierPart->Select(0);

	chSaves->Clear();

	m_territory_mouse = 0;
}

// destruct
FormSaveEdit::~FormSaveEdit()
{
}

// close window
void FormSaveEdit::OnClose(wxCloseEvent& ev)
{	
	ev.Skip();
}

// on close form
void FormSaveEdit::OnCloseClick(wxCommandEvent& event)
{	
	Close();
}


// set path to common.fs
void FormSaveEdit::SetCommonPath(std::filesystem::path common_fs_path, bool is_mod)
{
	m_is_mod = is_mod;
	m_common_fs_path = common_fs_path;

	if(is_mod)
		this->SetTitle("Spellcross Save Editor (using modded COMMON.FS)");
	else
		this->SetTitle("Spellcross Save Editor (using original COMMON.FS)");
}

// set savegames directory
void FormSaveEdit::SetSaveDir(std::filesystem::path save_dir)
{
	m_save_dir = save_dir;

	SpellSave::Saves saves;
	SpellSave::LoadSaves(m_save_dir,saves,true);
	chSaves->Freeze();
	chSaves->Clear();
	for(auto &save: saves)
	{				
		auto str = string_format("%s: %ls (%ls)",save.dir_name.c_str(), save.name.c_str(),save.dir_path.wstring().c_str());
		chSaves->Append(str,new SavesData(save.dir_path));
	}
	if(chSaves->GetCount())
		chSaves->Select(0);
	chSaves->Thaw();
}



// on open file
void FormSaveEdit::OnOpen(wxCommandEvent& event)
{
	std::filesystem::path path;
	if(event.GetId() == wxID_BTN_LOAD_SPELL_SAVE)
	{
		// from save menu
		auto sel_id = chSaves->GetSelection();
		if(sel_id < 0)
			return;
		auto save = (SavesData*)chSaves->GetClientObject(sel_id);
		path = save->m_path / "big_map.sav";
	}
	else
	{
		auto dir = m_save_dir;
		if(!m_bigmap.m_path.empty())
			dir = m_bigmap.m_path.parent_path();

		// using open dialogue
		wxFileDialog openFileDialog(this,"Open Spellcross savegame",dir.wstring(),"","Spellcross savegame (big_map.sav)|big_map.sav",
			wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if(openFileDialog.ShowModal() == wxID_CANCEL)
			return;
		path = std::filesystem::path(openFileDialog.GetPath().ToStdWstring());
	}
	if(path.empty() || !std::filesystem::exists(path))
		return;

	m_bigmap.Load(path, m_common_fs_path);
	UpdateList();	

	SetStatusText(path.wstring(),0);
}

// save 
void FormSaveEdit::OnSave(wxCommandEvent& event)
{
	if(m_bigmap.m_path.empty())
		return;
	
	// save as?
	std::filesystem::path path = m_bigmap.m_path;
	if(event.GetId() == wxID_MM_SAVE_AS)
	{
		std::wstring dir = path.parent_path().wstring();
		std::wstring name = path.filename().wstring();
		wxFileDialog openFileDialog(this,"Save Spellcross savegame",dir,name,"Spellcross savegame (big_map.sav)|big_map.sav",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if(openFileDialog.ShowModal() == wxID_CANCEL)
			return;
		path = std::filesystem::path(openFileDialog.GetPath().ToStdWstring());
	}
	else
	{
		wxMessageDialog dial(this, string_format("Overwrite Spellcross savegame \"%ls\"?\nMake sure you have backups as this tool is very experimental!",path.wstring().c_str()), "Save Spellcross savegame",wxICON_QUESTION|wxYES_NO|wxYES_DEFAULT);
		if(dial.ShowModal() != wxID_YES)
			return;
	}

	// try save
	if(m_bigmap.Save(path))
	{
		wxMessageBox(string_format("Failed saving Spellcross savegame to \"%ls\"!",path.wstring().c_str()),"Error",wxICON_ERROR);
		return;
	}	
}

// reload lists from current session
void FormSaveEdit::UpdateList()
{
	m_territory_mouse = 0;

	listRes->Freeze();
	listRes->Clear();
	for(auto& res: m_bigmap.research)
		listRes->Append(string_format("#%02d: ",&res - m_bigmap.research.data()) + res.name);
	listRes->Thaw();

	listUpg->Freeze();
	listUpg->Clear();
	for(auto& upg: m_bigmap.upgrade)
		listUpg->Append(string_format("#%02d: ",&upg - m_bigmap.upgrade.data()) + upg.name);
	listUpg->Thaw();

	listUnits->Freeze();
	auto sel_id = listUnits->GetSelection();
	listUnits->Clear();
	for(auto& unit: m_bigmap.units)
		listUnits->Append(string_format("#%02d: ",&unit - m_bigmap.units.data()) + unit.name);
	if(sel_id && sel_id < listUnits->GetCount())
		listUnits->Select(sel_id);
	listUnits->Thaw();

	listComanders->Freeze();
	sel_id = listComanders->GetSelection();
	listComanders->Clear();
	for(auto& com: m_bigmap.commanders)
		listComanders->Append(string_format("#%02d: ",&com - m_bigmap.commanders.data()) + com.full_name());
	if(sel_id && sel_id < listComanders->GetCount())
		listComanders->Select(sel_id);
	listComanders->Thaw();

	
	std::map<int,std::string> terr_enum;
	listTerritory->Freeze();
	listTerritory->Clear();
	for(auto& terr: m_bigmap.bigmap.terr)
	{
		if(terr.valid)
		{
			auto terr_id = &terr - m_bigmap.bigmap.terr.data();
			listTerritory->Append(string_format("Map %d: %s",terr_id,terr.dta_name.c_str()));
			terr_enum.insert({terr_id+1, terr.dta_name});
		}
		else
			listTerritory->Append(string_format("Map %d: <empty slot>",&terr - m_bigmap.bigmap.terr.data()));
	}
	listTerritory->Thaw();


	listHierUnits->ClearAll();
	listHierUnits->AppendColumn("list",wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);
	listHierUnits->SetItemCount(m_bigmap.units.size());
	listHierUnits->SetColumnWidth(0,wxLIST_AUTOSIZE_USEHEADER);
	listHierUnits->Refresh();

	listHierCommanders->ClearAll();
	listHierCommanders->AppendColumn("list",wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);
	listHierCommanders->SetItemCount(m_bigmap.commanders.size());
	listHierCommanders->SetColumnWidth(0,wxLIST_AUTOSIZE_USEHEADER);
	listHierCommanders->Refresh();

	auto& level = m_bigmap.level;
	gridLevelProp->Freeze();
	gridLevelProp->Clear();
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Level"),wxT(""),&m_bigmap.bigmap.level));
	gridLevelProp->Append(new wxEnumPropertyExt(wxT("Final territory"),wxT(""),MapToPGenumChoices(terr_enum),&m_bigmap.bigmap.final_terr));
	
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Money"),wxT(""),&level.money));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Money to research"),wxT(""),&level.money_research));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Round"),wxT(""),&level.round));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("XP"),wxT(""),&level.xp));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Rank"),wxT(""),&level.rank));

	gridLevelProp->Append(new wxIntPropertyExt(wxT("Level kill light"),wxT(""),&level.stat_kill_light));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Level kill armor"),wxT(""),&level.stat_kill_armor));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Level kill air"),wxT(""),&level.stat_kill_air));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Level losses light"),wxT(""),&level.stat_loss_light));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Level losses armor"),wxT(""),&level.stat_loss_armor));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Level losses air"),wxT(""),&level.stat_loss_air));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Level losses commanders"),wxT(""),&level.stat_loss_com));

	gridLevelProp->Append(new wxIntPropertyExt(wxT("Total kill light"),wxT(""),&level.stat_kill_light_tot));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Total kill armor"),wxT(""),&level.stat_kill_armor_tot));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Total kill air"),wxT(""),&level.stat_kill_air_tot));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Total losses light"),wxT(""),&level.stat_loss_light_tot));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Total losses armor"),wxT(""),&level.stat_loss_armor_tot));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Total losses air"),wxT(""),&level.stat_loss_air_tot));
	gridLevelProp->Append(new wxIntPropertyExt(wxT("Total losses commanders"),wxT(""),&level.stat_loss_com_tot));

	gridLevelProp->Thaw();
	gridLevelProp->FitColumns();


	FillHierarchy();
	canvasBigmap->Refresh();
}

// edit level properties
void FormSaveEdit::OnLevelPropChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;

	auto prop = event.GetProperty();
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
	{
		obj->Update(prop);
	}	
}



// on sort units
void FormSaveEdit::OnSortUnits(wxCommandEvent& event)
{
	auto id = event.GetId();
	m_bigmap.SortUnits(id == wxID_BTN_UNIT_REM_GAPS, id == wxID_BTN_UNIT_SORT_PERM_REIN, id == wxID_BTN_UNIT_SORT_TYPES, id == wxID_BTN_UNIT_SORT_NAMES);
	UpdateList();
}
// reset unit names
void FormSaveEdit::OnResetUnitNames(wxCommandEvent& event)
{
	if(event.GetId() == wxID_BTN_UNIT_RESET_NAME)
	{
		auto sel_id = listUnits->GetSelection();
		if(sel_id)
			m_bigmap.ResetUnitName(sel_id);
	}
	else
		m_bigmap.ResetUnitName();
	UpdateList();
	OnUnitSelect(event);
}


// on property edit
void FormSaveEdit::OnEditProp(wxPropertyGridEvent& event)
{
	wxMenu menu;
	menu.Append(0,"Test");
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormSaveEdit::OnEditPupup),NULL,this);
	PopupMenu(&menu);
}
void FormSaveEdit::OnEditPupup(wxCommandEvent& event)
{
}




wxString FormSaveEdit::OnGetHierUnitItem(long item_id)
{
	if(item_id >= m_bigmap.units.size())
		return("");
	auto& unit = m_bigmap.units[item_id];
	auto unit_name = string_format("#%02d: ",item_id) + unit.name;
	return(unit_name);
}
wxListItemAttr* FormSaveEdit::OnGetHierUnitItemAttr(long item_id)
{
	if(item_id >= m_bigmap.units.size())
		return(nullptr);
	auto &unit = m_bigmap.units[item_id];
	if(!unit.is_permanent() || unit.hierarch_pos >= 0)
	{
		static wxListItemAttr disabledAttr(*wxLIGHT_GREY,*wxWHITE,wxNullFont);
		return &disabledAttr;
	}
	return nullptr;
}
// on change hierarchy part selector
void FormSaveEdit::OnHierUnitListSelect(wxListEvent& event)
{
	long item_id = event.GetIndex();
	if(item_id >= m_bigmap.units.size())
		return;
	auto& unit = m_bigmap.units[item_id];

	if(!unit.is_permanent() || unit.hierarch_pos >= 0) {
		// Veto the selection to keep the previous state
		event.Veto();
		listHierUnits->SetItemState(item_id, 0,wxLIST_STATE_SELECTED);
		return;
	}
	event.Skip();
}

wxString FormSaveEdit::OnGetHierCommanderItem(long item_id)
{
	if(item_id >= m_bigmap.commanders.size())
		return("");
	auto& com = m_bigmap.commanders[item_id];
	auto unit_name = string_format("#%02d: ",item_id) + com.full_name();
	return(unit_name);
}
wxListItemAttr* FormSaveEdit::OnGetHierCommanderItemAttr(long item_id)
{
	if(item_id >= m_bigmap.commanders.size())
		return(NULL);
	auto& com = m_bigmap.commanders[item_id];
	if(com.is_empty() || com.is_placed())
	{
		static wxListItemAttr disabledAttr(*wxLIGHT_GREY,*wxWHITE,wxNullFont);
		return &disabledAttr;
	}
	return nullptr;
}
void FormSaveEdit::OnHierCommanderListSelect(wxListEvent& event)
{
	long item_id = event.GetIndex();
	if(item_id >= m_bigmap.commanders.size())
		return;
	auto& com = m_bigmap.commanders[item_id];

	if(com.is_placed()) {
		// Veto the selection to keep the previous state
		event.Veto();
		listHierCommanders->SetItemState(item_id,0,wxLIST_STATE_SELECTED);
		return;
	}
	event.Skip();
}

// hierarchy unit click
void FormSaveEdit::OnHierUnitClick(wxCommandEvent& event)
{
	int hier_id = event.GetId() - wxID_CH_HIERARCH_UNIT_0;
	if(chHierPart->GetSelection())
		hier_id += 16;
	if(hier_id < 0 || hier_id >= 32)
		return;

	// check if some commander-unit button is active
	int hier_level = -1;
	int hier_pos = -1;
	wxToggleButton *com_unit_btn = NULL;
	std::vector<int> wx_id_unit_list ={wxID_CH_HIERARCH_COM1U_0, wxID_CH_HIERARCH_COM2U_0, wxID_CH_HIERARCH_COM3U_0};
	for(int lev = 0; lev < 3; lev++)
	{
		int com_cnt = 4/(1<<lev);
		for(int k = 0; k < com_cnt; k++)
		{
			auto btn = (wxToggleButton*)panHierarchy->FindItem(wx_id_unit_list[lev] + k);
			if(!btn)
				continue;
			if(btn->GetValue())
			{
				hier_level = lev;
				hier_pos = k;
				com_unit_btn = btn;
				break;
			}
		}
	}
	
	if(hier_level >= 0 && hier_pos >= 0)
	{
		// --- this is commander unit assignement:
		int com_count_part = 4/(1<<hier_level);
		if(chHierPart->GetSelection())
			hier_pos += com_count_part;
		for(auto& com: m_bigmap.commanders)
			if(com.is_placed() && com.command_level() == hier_level && com.command_pos() == hier_pos)
			{
				// found commander
				for(auto& unit: m_bigmap.units)
					if(unit.hierarch_pos == hier_id)
					{
						// found target unit: assign
						com.unit_id = &unit - m_bigmap.units.data();
						com_unit_btn->SetValue(false);
						break;
					}
				break;
			}
	}
	else
	{
		// --- this is unit assignement to the hierarchy slot:
		
		// first remove old unit
		for(auto &unit: m_bigmap.units)
			if(unit.hierarch_pos == hier_id)
			{
				unit.hierarch_pos = -1;
				// also remove commander(s)
				auto unit_id = &unit - m_bigmap.units.data();
				for(auto &com: m_bigmap.commanders)
					if(com.unit_id == unit_id)
						com.unit_id = -1;
			}
	
		// assign new unit?
		auto unit_sel_id = listHierUnits->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
		if(unit_sel_id >= 0 && unit_sel_id < m_bigmap.units.size())
		{
			auto &unit = m_bigmap.units[unit_sel_id];
			unit.hierarch_pos = hier_id;
			listHierUnits->SetItemState(unit_sel_id,0,wxLIST_STATE_SELECTED);
		}
	}
		
	FillHierarchy();
}

// hierarchy commander click
void FormSaveEdit::OnHierComClick(wxCommandEvent& event)
{
	auto ev_id = event.GetId();
	int lev = -1;
	int com_slot = -1;
	if(ev_id >= wxID_CH_HIERARCH_COM1C_0 && ev_id < wxID_CH_HIERARCH_COM2C_0)
	{
		lev = 0;
		com_slot = ev_id - wxID_CH_HIERARCH_COM1C_0;
	}
	else if(ev_id >= wxID_CH_HIERARCH_COM2C_0 && ev_id < wxID_CH_HIERARCH_COM3C_0)
	{
		lev = 1;
		com_slot = ev_id - wxID_CH_HIERARCH_COM2C_0;
	}
	else if(ev_id >= wxID_CH_HIERARCH_COM3C_0 && ev_id < wxID_CH_HIERARCH_COM3C_0 + 2)
	{
		lev = 2;
		com_slot = ev_id - wxID_CH_HIERARCH_COM3C_0;
	}
	if(lev < 0)
		return;
	int com_count_part = 4/(1<<lev);
	int com_slot_abs = com_slot;
	if(chHierPart->GetSelection())
		com_slot_abs += com_count_part;

	// first remove commander from heirarchy position
	for(auto& com: m_bigmap.commanders)
		if(com.command_level() == lev && com.command_pos() == com_slot_abs)
		{
			com.place();
			break;
		}

	// assign new unit?
	auto com_sel_id = listHierCommanders->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(com_sel_id >= 0 && com_sel_id < m_bigmap.commanders.size())
	{
		// place new commander
		auto& com = m_bigmap.commanders[com_sel_id];

		// check required rank
		std::vector<int> min_rank_list = {0, 3, 6};
		if(com.rank < min_rank_list[lev])
		{
			wxMessageBox(string_format("Commander \"%ls\" has not required rank for this position!",com.name.c_str()),"Command assignement", wxICON_EXCLAMATION);						
			FillHierarchy();
			return;
		}

		com.place(lev,com_slot_abs);
		// make sure there is no other colliding
		for(auto& other_com: m_bigmap.commanders)
			if(&com != &other_com && other_com.flags == com.flags)
				other_com.place();
		listHierCommanders->SetItemState(com_sel_id,0,wxLIST_STATE_SELECTED);
	}
	
	FillHierarchy();
}

// hierarchy commander-unit click
void FormSaveEdit::OnHierComUnitClick(wxCommandEvent& event)
{
	auto ev_id = event.GetId();
	int lev = -1;
	int com_slot = -1;
	if(ev_id >= wxID_CH_HIERARCH_COM1U_0 && ev_id < wxID_CH_HIERARCH_COM2U_0)
	{		
		lev = 0;
		com_slot = ev_id - wxID_CH_HIERARCH_COM1U_0;
	}
	else if(ev_id >= wxID_CH_HIERARCH_COM2U_0 && ev_id < wxID_CH_HIERARCH_COM3U_0)
	{
		lev = 1;
		com_slot = ev_id - wxID_CH_HIERARCH_COM2U_0;
	}
	else if(ev_id >= wxID_CH_HIERARCH_COM3U_0 && ev_id < wxID_CH_HIERARCH_COM3U_0 + 2)
	{
		lev = 2;
		com_slot = ev_id - wxID_CH_HIERARCH_COM3U_0;
	}
	if(lev < 0)
		return;		
	int com_count_part = 4/(1<<lev);
	int com_slot_abs = com_slot;
	if(chHierPart->GetSelection())
		com_slot_abs += com_count_part;
	
	// first remove old unit assignement
	for(auto& com: m_bigmap.commanders)
		if(com.command_level() == lev && com.command_pos() == com_slot_abs)
			com.unit_id = -1;

	// enable unit buttons available for selected commander position
	int com_unit_count = 4*(1<<lev);
	int com_unit_ofs = com_slot*4*(1<<lev);
	auto btn = (wxToggleButton*)event.GetEventObject();
	bool foc = btn->GetValue();
	for(int k = 0; k < 16; k++)
	{
		bool active = k >= com_unit_ofs && k < com_unit_ofs + com_unit_count;
		auto unit_btn = (wxButton*)panHierarchy->FindItem(wxID_CH_HIERARCH_UNIT_0 + k);
		if(!unit_btn)
			continue;
		unit_btn->Enable(active || !foc);
	}
	
	if(foc)
	{
		// untoggle all other buttons
		std::vector<int> wx_id_unit_list ={wxID_CH_HIERARCH_COM1U_0, wxID_CH_HIERARCH_COM2U_0, wxID_CH_HIERARCH_COM3U_0};
		for(int lev = 0; lev < 2; lev++)
		{
			int com_cnt = 4/(1<<lev);
			for(int k = 0; k < com_cnt; k++)
			{
				auto unit_btn = (wxToggleButton*)panHierarchy->FindItem(wx_id_unit_list[lev] + k);
				if(!unit_btn)
					continue;
				unit_btn->SetValue(false);
			}
		}
		btn->SetValue(true);
	}
	

	FillHierarchy();
}

// on change hierarchy part selector
void FormSaveEdit::OnHierPartSelect(wxCommandEvent& event)
{
	// untoggle all other buttons
	std::vector<int> wx_id_com_list ={wxID_CH_HIERARCH_COM1C_0, wxID_CH_HIERARCH_COM2C_0, wxID_CH_HIERARCH_COM3C_0};
	std::vector<int> wx_id_unit_list ={wxID_CH_HIERARCH_COM1U_0, wxID_CH_HIERARCH_COM2U_0, wxID_CH_HIERARCH_COM3U_0};
	for(int lev = 0; lev < 2; lev++)
	{
		int com_cnt = 4/(1<<lev);
		for(int k = 0; k < com_cnt; k++)
		{
			auto unit_btn = (wxToggleButton*)panHierarchy->FindItem(wx_id_unit_list[lev] + k);
			if(!unit_btn)
				continue;
			unit_btn->SetValue(false);
			auto com_btn = (wxToggleButton*)panHierarchy->FindItem(wx_id_com_list[lev] + k);
			if(!com_btn)
				continue;
			com_btn->SetValue(false);
		}
	}
	for(int k = 0; k < 16; k++)
	{
		auto unit_btn = (wxButton*)panHierarchy->FindItem(wxID_CH_HIERARCH_UNIT_0 + k);
		if(!unit_btn)
			continue;
		unit_btn->Enable(true);
	}

	FillHierarchy();
}

// fill command hierarchy selectors
void FormSaveEdit::FillHierarchy()
{
	int hier_part = chHierPart->GetSelection();
	int hier_ofs = hier_part*16;

	for(int k = 0; k < 16; k++)
	{		
		auto choice = (wxButton*)panHierarchy->FindItem(wxID_CH_HIERARCH_UNIT_0 + k);
		if(!choice)
			return;
		choice->SetLabel("<<< empty slot >>>");		
		for(auto& unit: m_bigmap.units)
		{
			if(!unit.is_permanent())
				continue;
			if(unit.hierarch_pos != k + hier_ofs)
				continue;
			auto unit_id = &unit - m_bigmap.units.data();
			auto unit_name = string_format("#%02d: ",unit_id) + unit.name;
			choice->SetLabel(unit_name);
		}
	}

	std::vector<int> wx_id_com_list = {wxID_CH_HIERARCH_COM1C_0, wxID_CH_HIERARCH_COM2C_0, wxID_CH_HIERARCH_COM3C_0};
	std::vector<int> wx_id_unit_list = {wxID_CH_HIERARCH_COM1U_0, wxID_CH_HIERARCH_COM2U_0, wxID_CH_HIERARCH_COM3U_0};	
	for(int lev = 0; lev < 3; lev++)
	{
		int com_count = 4/(1<<lev);
		int com_ofs = hier_part*com_count;
		for(int k = 0; k < com_count; k++)
		{
			auto choice_unit = (wxToggleButton*)panHierarchy->FindItem(wx_id_unit_list[lev] + k);
			auto choice_com = (wxToggleButton*)panHierarchy->FindItem(wx_id_com_list[lev] + k);
			if(!choice_unit || !choice_com)
				return;

			choice_com->SetLabel("<<< empty slot >>>");
			int com_unit_id = -1;
			for(auto& com: m_bigmap.commanders)
			{				
				auto com_name = string_format("#%02d: ",&com - m_bigmap.commanders.data()) + com.full_name();
				if(com.is_placed() && com.command_pos() == k + com_ofs && com.command_level() == lev)
				{
					choice_com->SetLabel(com_name);
					com_unit_id = com.unit_id;
				}
			}

			int lev_unit_count = 16/com_count;
			choice_unit->SetLabel("<<< empty slot >>>");
			for(auto& unit: m_bigmap.units)
			{
				if(!unit.is_permanent())
					continue;
				if(unit.hierarch_pos < hier_ofs + k*lev_unit_count || unit.hierarch_pos >= hier_ofs + (k + 1)*lev_unit_count)
					continue;
				int unit_id = &unit - m_bigmap.units.data();
				auto unit_name = string_format("#%02d: ",unit_id) + unit.name;
				if(unit_id == com_unit_id)
					choice_unit->SetLabel(unit_name);
			}
		}
	}

	listHierUnits->Refresh();
	listHierCommanders->Refresh();
}

// on select research item
void FormSaveEdit::OnResSelect(wxCommandEvent& event)
{
	gridResProp->Clear();
	
	auto rid = listRes->GetSelection();
	if(rid >= m_bigmap.research.size())
		return;
	auto &res = m_bigmap.research[rid];

	gridResProp->Freeze();
	gridResProp->Clear();
	gridResProp->Append(new wxStringProperty(wxT("Name"),wxT(""),res.name));
	gridResProp->Append(new wxStringProperty(wxT("Upgrade price"),wxT(""),string_format("%d",res.cost)));
	gridResProp->Append(new wxStringProperty(wxT("Group"),wxT(""),string_format("%d",res.group_code)));
	gridResProp->Append(new wxStringProperty(wxT("Level"),wxT(""),string_format("%d",res.level)));
	gridResProp->Append(new wxStringProperty(wxT("Time"),wxT(""),string_format("%d",res.time)));
	gridResProp->Append(new wxStringProperty(wxT("Data"),wxT(""),string_format("%d",res.data_id)));
	gridResProp->Append(new wxStringProperty(wxT("Flags"),wxT(""),string_format("%d",res.flags)));
	gridResProp->Append(new wxStringProperty(wxT("State"),wxT(""),string_format("%d",res.state)));
	gridResProp->Thaw();
	gridResProp->FitColumns();
}

// on select research item
void FormSaveEdit::OnUpgSelect(wxCommandEvent& event)
{
	gridUpgProp->Clear();

	auto rid = listUpg->GetSelection();
	if(rid >= m_bigmap.upgrade.size())
		return;
	auto& upg = m_bigmap.upgrade[rid];

	gridUpgProp->Freeze();
	gridUpgProp->Clear();
	gridUpgProp->Append(new wxStringProperty(wxT("Name"),wxT(""),upg.name));
	gridUpgProp->Append(new wxStringProperty(wxT("Upgrade price"),wxT(""),string_format("%d",upg.upg_price)));
	gridUpgProp->Append(new wxStringProperty(wxT("Upgrade time"),wxT(""),string_format("%d",upg.upg_time)));
	gridUpgProp->Append(new wxStringProperty(wxT("Move"),wxT(""),string_format("%d",upg.move)));
	gridUpgProp->Append(new wxStringProperty(wxT("Attack"),wxT(""),string_format("%d",upg.attack)));
	gridUpgProp->Append(new wxStringProperty(wxT("Attack PT"),wxT(""),string_format("%d",upg.attack_pt)));
	gridUpgProp->Append(new wxStringProperty(wxT("Defence"),wxT(""),string_format("%d",upg.defence)));
	gridUpgProp->Append(new wxStringProperty(wxT("Range"),wxT(""),string_format("%d",upg.range)));
	gridUpgProp->Append(new wxStringProperty(wxT("Sight"),wxT(""),string_format("%d",upg.sight)));
	gridUpgProp->Append(new wxStringProperty(wxT("Flags"),wxT(""),upg.GetUpgradeClassStr()));
	
	std::ostringstream types_str;
	std::copy(std::begin(upg.suitable_types),std::end(upg.suitable_types),std::ostream_iterator<int>(types_str,", "));
	gridUpgProp->Append(new wxStringProperty(wxT("Suitable types"),wxT(""),std::string(types_str.str())));
	
	gridUpgProp->Thaw();
	gridUpgProp->FitColumns();
}

// on select unit item
void FormSaveEdit::OnUnitSelect(wxCommandEvent& event)
{
	gridUnitProp->Clear();

	auto rid = listUnits->GetSelection();
	if(rid >= m_bigmap.units.size())
		return;
	auto& unit = m_bigmap.units[rid];

	int row = 0;
	int col = 0;
	for(auto &val: unit.raw)
	{
		gridRawUnit->SetCellValue(row,col,string_format("%d",val));
		col++;
		if(col >= gridRawUnit->GetNumberCols())
		{
			col = 0;
			row++;
		}
		if(row >= gridRawUnit->GetNumberRows())
			break;
	}
	

	gridUnitProp->Freeze();
	gridUnitProp->Clear();
	gridUnitProp->Append(new wxStringPropertyExt(wxT("Name"),wxT(""),&unit.name,29));
	gridUnitProp->Append(new wxStringProperty(wxT("Flags"),wxT(""),string_format("0x%02X",unit.flags)));
	//gridUnitProp->Append(new wxStringProperty(wxT("Action timeout"),wxT(""),string_format("%d",unit.action_timeout)));
	gridUnitProp->Append(new wxEnumPropertyExt(wxT("Unit type"),wxT(""),MapToPGenumChoices(m_bigmap.GetUnitTypeList(false,true)),&unit.unit_type_id));
	gridUnitProp->Append(new wxIntPropertyExt(wxT("XP"),wxT(""),&unit.xp));
	gridUnitProp->Append(new wxIntPropertyExt(wxT("XP level"),wxT(""),&unit.xp_level));
	gridUnitProp->Append(new wxIntPropertyExt(wxT("HP"),wxT(""),&unit.hp));
	gridUnitProp->Append(new wxIntPropertyExt(wxT("HP max"),wxT(""),&unit.hp_max));
	gridUnitProp->Append(new wxStringProperty(wxT("Hierarchy pos"),wxT(""),string_format("%d",unit.hierarch_pos)));		
	gridUnitProp->Append(new wxEnumPropertyExt(wxT("Upgrade armor"),wxT("armor"),MapToPGenumChoices(m_bigmap.GetUpgradeList(SpellSaveUpgrade::UpgradeClass::ARMOR)),&unit.upg_armor));
	gridUnitProp->Append(new wxEnumPropertyExt(wxT("Upgrade weapon"),wxT(""),MapToPGenumChoices(m_bigmap.GetUpgradeList(SpellSaveUpgrade::UpgradeClass::WEAPON)),&unit.upg_weapon));
	gridUnitProp->Append(new wxEnumPropertyExt(wxT("Upgrade engine"),wxT(""),MapToPGenumChoices(m_bigmap.GetUpgradeList(SpellSaveUpgrade::UpgradeClass::ENGINE)),&unit.upg_engine));	
	gridUnitProp->Append(new wxIntPropertyExt(wxT("Upgrade timeout"),wxT(""),&unit.upg_timeout));
	gridUnitProp->Append(new wxEnumPropertyExt(wxT("Upgrade unit type ID"),wxT(""),MapToPGenumChoices(m_bigmap.GetUnitTypeList(true,true)),&unit.upg_unit_type));
	

	gridUnitProp->Thaw();
	gridUnitProp->FitColumns();
}

// edit unit properties
void FormSaveEdit::OnUnitPropChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;

	auto prop = event.GetProperty();
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
	{
		obj->Update(prop);
		UpdateList();
	}
}


// on select commander item
void FormSaveEdit::OnCommanderSelect(wxCommandEvent& event)
{
	gridCommanderProp->Clear();

	auto rid = listComanders->GetSelection();
	if(rid >= m_bigmap.commanders.size())
		return;
	auto& com = m_bigmap.commanders[rid];

	int row = 0;
	int col = 0;
	for(auto& val: com.raw)
	{
		gridRawCommander->SetCellValue(row,col,string_format("%d",val));
		col++;
		if(col >= gridRawCommander->GetNumberCols())
		{
			col = 0;
			row++;
		}
		if(row >= gridRawCommander->GetNumberRows())
			break;
	}


	gridCommanderProp->Freeze();
	gridCommanderProp->Clear();
	gridCommanderProp->Append(new wxStringPropertyExt(wxT("Name"),wxT(""),&com.name,29));
	gridCommanderProp->Append(new wxStringProperty(wxT("Flags"),wxT(""),string_format("0x%04X",com.flags)));
	gridCommanderProp->Append(new wxIntPropertyExt(wxT("Battles"),wxT(""),&com.battles));
	gridCommanderProp->Append(new wxIntPropertyExt(wxT("Rank"),wxT(""),&com.rank));
	gridCommanderProp->Append(new wxStringProperty(wxT("Hierarchy Level"),wxT(""),string_format("%d",com.command_level())));
	gridCommanderProp->Append(new wxStringProperty(wxT("Hierarchy position"),wxT(""),string_format("%d",com.command_pos())));
	gridCommanderProp->Append(new wxStringProperty(wxT("Assigned to unit"),wxT(""),string_format("%d",com.unit_id)));
	gridCommanderProp->Append(new wxStringProperty(wxT("x[38]"),wxT(""),string_format("%d",com.x38)));
	gridCommanderProp->Thaw();
	gridCommanderProp->FitColumns();
}

// on select commander item
void FormSaveEdit::OnTerritorySelect(wxCommandEvent& event)
{
	m_territory_mouse = 0;
	gridTerritoriesProp->Clear();

	auto rid = listTerritory->GetSelection();
	if(rid >= m_bigmap.bigmap.terr.size())
		return;
	auto& terr = m_bigmap.bigmap.terr[rid];
	gridTerritoriesProp->SetClientData(&m_bigmap.bigmap.terr[rid]);


	gridTerritoriesProp->Freeze();
	gridTerritoriesProp->Clear();
	gridTerritoriesProp->Append(new wxStringPropertyExt(wxT("DEF name"),wxT(""),&terr.def_name,12));
	gridTerritoriesProp->Append(new wxStringPropertyExt(wxT("DTA name"),wxT(""),&terr.dta_name,12));
	gridTerritoriesProp->Append(new wxStringPropertyExt(wxT("Music name"),wxT(""),&terr.mus_name,12));
	gridTerritoriesProp->Append(new wxIntPropertyExt(wxT("Remaining money"),wxT(""),&terr.remain_money));
	gridTerritoriesProp->Append(new wxIntPropertyExt(wxT("Money/round"),wxT(""),&terr.money_per_round));
	gridTerritoriesProp->Append(new wxIntPropertyExt(wxT("Remaining time"),wxT(""),&terr.remain_time));
	gridTerritoriesProp->Append(new wxIntPropertyExt(wxT("FrequencyOfRandomAttacks(?,)"),wxT(""),&terr.freq_random_attacks_b));
	gridTerritoriesProp->Append(new wxIntPropertyExt(wxT("FrequencyOfRandomAttacks(,?)"),wxT(""),&terr.freq_random_attacks_a));
	gridTerritoriesProp->Append(new wxStringProperty(wxT("Neighbor flags (SSD)"),wxT(""),format_bin(terr.ssd_flags,32,true)));
	gridTerritoriesProp->Append(new wxStringProperty(wxT("Flags"),wxT(""),string_format("%02X",terr.flags)));
	gridTerritoriesProp->Append(new wxEnumProperty(wxT("State"),wxT("state"),MapToPGenumChoices(SpellSaveTerritories::c_states),terr.flags));	

	gridTerritoriesProp->Thaw();
	gridTerritoriesProp->FitColumns();
	
	int row = 0;
	int col = 0;
	for(auto& val: terr.raw)
	{
		gridRawBigmap->SetCellValue(row,col,string_format("%d",val));
		col++;
		if(col >= gridRawBigmap->GetNumberCols())
		{
			col = 0;
			row++;
		}
		if(row >= gridRawBigmap->GetNumberRows())
			break;
	}
	
	canvasBigmap->Refresh();
}

// edit territory property
void FormSaveEdit::OnTerrPropChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;
	auto terr = (SpellSaveTerritories*)pgrid->GetClientData();
	if(!terr)
		return;

	auto prop = event.GetProperty();
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
	{
		obj->Update(prop);
	}
	else if(event.GetPropertyName() == "state")
	{
		auto prop = (wxEnumProperty*)event.GetProperty();
		terr->flags = prop->GetValue().GetLong();
	}

	OnTerritorySelect(event);
}

// render preview
void FormSaveEdit::OnPaintBigmapCanvas(wxPaintEvent& event)
{	
	int surf_x = canvasBigmap->GetClientSize().GetWidth();
	int surf_y = canvasBigmap->GetClientSize().GetHeight();

	auto terr_id = listTerritory->GetSelection();		
	terr_id++;

	// make enemy territory mask
	std::vector<bool> is_enemy(m_bigmap.bigmap.terr_count,true);
	for(auto &terr: m_bigmap.bigmap.terr)
		is_enemy[&terr - m_bigmap.bigmap.terr.data()] = terr.isEnemy();

	int x_size = m_bigmap.bigmap.x_size;
	int y_size = m_bigmap.bigmap.y_size;
	if(m_bigmap.bigmap.image.empty())
	{
		canvasBigmap->ClearBackground();
		return;
	}
	
	// leave if surface not big enough
	if(surf_x < x_size || surf_y < y_size)
	{
		canvasBigmap->ClearBackground();
		return;
	}

	wxBitmap* bmp = new wxBitmap(surf_x,surf_y,32);
	bmp->UseAlpha(true);

	// source dat end
	uint8_t* end = m_bigmap.bigmap.image.data() + m_bigmap.bigmap.image.size();

	// image should fit to surface
	int x_ofs = (surf_x - x_size)/2;
	int y_ofs = (surf_y - y_size)/2;

	// palette
	auto pal_buf = m_bigmap.bigmap.pal;
	for(auto& col: pal_buf)
		col = pow((double)col/255.0,1.0/1.3)*255.0;
	uint8_t(*pal)[3] = (uint8_t(*)[3])pal_buf.data();
	
	// make highlighted pallette
	auto pal_high_buf = m_bigmap.bigmap.pal;
	for(auto &col: pal_high_buf)
		col = pow((double)col/255.0, 1.0/2.5)*255.0;
	uint8_t(*pal_high)[3] = (uint8_t(*)[3])pal_high_buf.data();

	int mouse_hover_id = m_territory_mouse + 0;
			
	// render 32bit RGBA data to raw bmp buffer
	uint8_t* buf = &m_bigmap.bigmap.image[0];
	uint8_t* terr = &m_bigmap.bigmap.terr_mask[0];
	uint8_t* hatch = &m_bigmap.bigmap.terr_hatch[0];
	typedef wxPixelData<wxBitmap,wxAlphaPixelFormat> PixelData;
	PixelData data(*bmp);
	PixelData::Iterator p(data);
	for(int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < surf_x; x++)
		{
			int is_visible = y >= y_ofs && y < y_ofs+y_size && x >= x_ofs && x < x_ofs+x_size && buf < end;
			// visible area
			if(is_visible)
			{				
				
				if(*terr && (*terr == terr_id || *terr == mouse_hover_id))
				{
					*scan++ = pal_high[*buf][2];
					*scan++ = pal_high[*buf][1];
					*scan++ = pal_high[*buf][0];
				}
				else
				{
					*scan++ = pal[*buf][2];
					*scan++ = pal[*buf][1];
					*scan++ = pal[*buf][0];
				}
				if(*terr > 128)
				{
					scan[-3] = 0;
					scan[-2] = 0;
					scan[-1] = 0;
				}
				if(*hatch && *hatch < 128 && is_enemy[*hatch - 1])
				{
					scan[-3] = 0;
					scan[-2] = 0;
					scan[-1] = 255;
				}
				buf++;
				terr++;
				hatch++;
			}
			else
			{
				*scan++ = 0;
				*scan++ = 0;
				*scan++ = 0;
			}
			*scan++ = (is_visible && *buf != 0)*255; // alpha channel			
		}
		p.OffsetY(data,1);
	}		

	// blit to screen
	wxPaintDC pdc(canvasBigmap);
	pdc.DrawBitmap(*bmp,wxPoint(0,0));
	delete bmp;
}


// render preview
void FormSaveEdit::OnBigmapMouse(wxMouseEvent& event)
{
	int surf_x = canvasBigmap->GetClientSize().GetWidth();
	int surf_y = canvasBigmap->GetClientSize().GetHeight();		
	int x_size = m_bigmap.bigmap.x_size;
	int y_size = m_bigmap.bigmap.y_size;
	if(m_bigmap.bigmap.terr_mask.empty())
		return;
	if(surf_x < x_size || surf_y < y_size)
		return;
	int x_ofs = (surf_x - x_size)/2;
	int y_ofs = (surf_y - y_size)/2;
	int x_pos = event.GetX() - x_ofs;
	int y_pos = event.GetY() - y_ofs;

	auto territory_mouse = 0;
	if(x_pos < 0 || x_pos >= x_size || y_pos < 0 || y_pos >= y_size)
	{
		// outside valid area		
	}
	else
	{
		// valid area	
		auto terr_id = m_bigmap.bigmap.terr_mask[x_pos + y_pos*x_size];
		if(terr_id < 128)
			territory_mouse = terr_id;
		if(event.LeftDown())
		{
			// select territory
			listTerritory->Select(terr_id - 1);
			wxCommandEvent dummyEvent(wxEVT_COMMAND_LISTBOX_SELECTED,wxID_LBOX_TERRITORY);
			OnTerritorySelect(dummyEvent);
		}	
	}

	// repaint?
	if(territory_mouse != m_territory_mouse)
	{
		m_territory_mouse = territory_mouse;
		canvasBigmap->Refresh();
	}
}
