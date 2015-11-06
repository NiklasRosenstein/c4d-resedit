/*********************************************************************\
	File name        : ResEditToolBar.cpp
	Description      : Implementation if the class CResEditToolBar
	Created at       : 11.08.01, @ 12:52:15
	Created by       : Thomas Kunert
	Modified by      :
\*********************************************************************/


#include "stdafx.h"
#include "globals.h"
#include "ResEditToolBar.h"
#include "DialogDoc.h"
#include "DialogItem.h"
#include "IconButton.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

enum {
	IDC_FIST_BUTTON = 1000,

	IDC_STATIC_BTN = 1000,
	IDC_EDIT_BTN, // all edit boxes (text, number, arrow, slider may be changed in the settings)
	IDC_BUTTON_BTN,
	IDC_ARROW_BTN,
	IDC_CHECK_BOX_BTN,
	IDC_RADIO_BTN,

	IDC_LIST_VIEW_BTN,
	IDC_COMBO_BOX_BTN,
	IDC_COMBO_BUTTON_BTN,
	IDC_DIALOG_GROUP_BTN,
	IDC_GROUP_BTN,
	IDC_SEPARATOR_BTN,
	IDC_USER_AREA_BTN,

	IDC_SLIDER_BTN,
	IDC_COLOR_FIELD_BTN,
	IDC_SUB_DLG_BTN,
	IDC_CUSTOM_ELEMENT_BTN,

	IDC_ICONSIZE_SMALL,
	IDC_ICONSIZE_MEDIUM,
	IDC_ICONSIZE_LARGE,
	IDC_ICONORDER_HORIZ,
	IDC_ICONORDER_VERT,
	IDC_ICONSINGLE,
	IDC_ICONGROUP,

	IDC_DUMMY // just for the mac comiler :-)
};

typedef struct {
	Int32 lID;
	ItemType type;
	Int32 x, y;
	Char* tooltip;
} _TagButtonInfo;

_TagButtonInfo g_pButtons[17] = {
	{ IDC_STATIC_BTN,       Static, 1, 0, "Static Text"},
	{ IDC_EDIT_BTN,         EditBox, 7, 0, "Edit Field"},

	{ IDC_BUTTON_BTN,       Button, 5, 0, "Button" },
	{ IDC_ARROW_BTN,        ArrowBtn, 6, 0, "Arrow" },

	{ IDC_CHECK_BOX_BTN,    CheckBox, 3, 0, "Checkbox" },
	{ IDC_RADIO_BTN,        RadioBtn, 4, 0, "Radio" },

	{ IDC_LIST_VIEW_BTN,    ListBox, 2, 1, "List View" },
	{ IDC_COMBO_BOX_BTN,    ComboBox, 3, 1, "Combobox" },
	{ IDC_COMBO_BUTTON_BTN, ComboButton, 8, 1, "Combobutton" },

	{ IDC_GROUP_BTN,        Group, 2, 0, "Group" },
	{ IDC_DIALOG_GROUP_BTN, DialogGroup, 4, 1, "Dialog Group" },

	{ IDC_SEPARATOR_BTN,    Separator, 1, 1, "Separator" },
	{ IDC_USER_AREA_BTN,    UserArea, 5, 1, "User Area" },

	{ IDC_SLIDER_BTN,       Slider, 8, 0, "Slider" },
	{ IDC_COLOR_FIELD_BTN,  Color, 0, 1, "Color Field" },

	{ IDC_SUB_DLG_BTN,      SubDialogControl, 6, 1, "Sub Dialog" },
	{ IDC_CUSTOM_ELEMENT_BTN,      CustomElement, 7, 1, "Custom GUI" },
};

#define TOOLBAR_BUTTON_COUNT (sizeof(g_pButtons) / sizeof(g_pButtons[0]))

CResEditToolBar::CResEditToolBar(CDialogDoc* pDoc)
{
	m_pDocument = pDoc;
	m_pDocument->m_pToolbar = this;

	m_bCloseState = false;
	m_buttons = bNewDeprecatedUseArraysInstead<IconButton>(TOOLBAR_BUTTON_COUNT);  // XXX: nullptr check
}

CResEditToolBar::~CResEditToolBar()
{
	bDelete(m_buttons);
}

void ChangeWindowFlags(void* pHandle);
void ChangeWindowWidth(void* pHandle, long lDiffX, long lDiffY);

/*********************************************************************\
	Function name    : CResEditToolBar::CreateLayout
	Description      :
	Created at       : 11.08.01, @ 12:54:40
	Created by       : Thomas Kunert
	Modified by      :
\*********************************************************************/
Bool CResEditToolBar::CreateLayout(void)
{
	void* pHandle = GetWindowHandle();
	ChangeWindowFlags(pHandle);

	Bool bRes = GeDialog::CreateLayout();
	SetTitle(GeLoadString(IDS_RES_EDIT_TOOLBAR));

	// Set up the buttons.
	IconData icon;
	icon.bmp = g_pControlImages;
	icon.w = CONTROLIMAGE_SIZE;
	icon.h = CONTROLIMAGE_SIZE;

	for (Int32 idx = 0; idx < TOOLBAR_BUTTON_COUNT; ++idx) {
		icon.x = g_pButtons[idx].x * CONTROLIMAGE_SIZE;
		icon.y = g_pButtons[idx].y * CONTROLIMAGE_SIZE;
		m_buttons[idx] = IconButton(icon, 20, 20);
		m_buttons[idx].m_tooltip = g_pButtons[idx].tooltip;
	}

	GroupBegin(IDC_ICONGROUP, BFH_SCALEFIT | BFV_SCALEFIT, 1, 1, "", 0);
	UpdateToolbarIcons();
	GroupEnd();

	if (++g_lNeedFileNew == 4)
		m_pDocument->OnFileNew();
	return bRes;
}

/*********************************************************************\
	Function name    : CResEditToolBar::Command
	Description      :
	Created at       : 11.08.01, @ 16:17:58
	Created by       : Thomas Kunert
	Modified by      :
\*********************************************************************/
Bool CResEditToolBar::Command(Int32 lID, const BaseContainer &msg)
{
	Int32 lCount = sizeof(g_pButtons) / sizeof(g_pButtons[0]);
	for (Int32 a = 0; a < lCount; a++) {
		if (lID == g_pButtons[a].lID) {
			m_pDocument->AddItem(g_pButtons[a].type, false, true);
			return true;
		}
	}

	switch (lID) {
		case IDC_ICONSIZE_SMALL:
		case IDC_ICONSIZE_MEDIUM:
		case IDC_ICONSIZE_LARGE:
			g_pResEditPrefs->toolbarIconSize = lID;
			UpdateToolbarIcons();
			return true;
		case IDC_ICONORDER_HORIZ:
		case IDC_ICONORDER_VERT:
			g_pResEditPrefs->toolbarIconOrder = lID;
			UpdateToolbarIcons();
			break;
		case IDC_ICONSINGLE:
			g_pResEditPrefs->toolbarIconSingle = !g_pResEditPrefs->toolbarIconSingle;
			UpdateToolbarIcons();
			break;
	}
	return true;
}

Bool CResEditToolBar::InitValues(void)
{
	Int32 x1, y1, w1, h1;
	Int32 x2, y2, w2, h2;

	GetItemDim(100, &x1, &y1, &w1, &h1);
	GetItemDim(101, &x2, &y2, &w2, &h2);

	void* pHandle = GetWindowHandle();
	ChangeWindowWidth(pHandle, w1 - w2, h1 - h2);

	return true;
}

/*********************************************************************\
	Function name    : CResEditToolBar::AskClose
	Description      :
	Created at       : 11.10.01, @ 21:37:44
	Created by       : Thomas Kunert
	Modified by      :
\*********************************************************************/
Bool CResEditToolBar::AskClose()
{
	if (m_bCloseState) return false;

	m_bCloseState = true;
	if (!m_pDocument->CloseEditor()) return true;

	return false;
}

/*********************************************************************\
	Function name    : CResEditToolBar::UpdateToolbarIcons
	Description      :
	Created at       : 05.11.2015, @ 22:10
	Created by       : Niklas Rosenstein
\*********************************************************************/
void CResEditToolBar::UpdateToolbarIcons()
{
	auto check = [](String const& s, Bool const c) -> String {
		if (c) return s + "&c&";
		return s;
	};

	Int32& iconSizeMode = g_pResEditPrefs->toolbarIconSize;
	Int32& iconOrderMode = g_pResEditPrefs->toolbarIconOrder;

	Int32 iconSize;
	switch (iconSizeMode) {
		case IDC_ICONSIZE_LARGE:
			iconSize = 34;
			break;
		case IDC_ICONSIZE_MEDIUM:
			iconSize = 26;
			break;
		case IDC_ICONSIZE_SMALL:
		default:
			iconSize = 20;
			iconSizeMode = IDC_ICONSIZE_SMALL;
			break;
	}

	Int32 const n = (g_pResEditPrefs->toolbarIconSingle ? 1 : 2);
	Int32 cols = n, rows = 0;
	if (iconOrderMode != IDC_ICONORDER_VERT) {
		cols = 0, rows = n;
		iconOrderMode = IDC_ICONORDER_HORIZ;
	}

	LayoutFlushGroup(IDC_ICONGROUP);
	GroupBegin(0, 0, cols, rows, "", 0);
	for (Int32 idx = 0; idx < TOOLBAR_BUTTON_COUNT; ++idx) {
		m_buttons[idx].m_width = iconSize;
		m_buttons[idx].m_height = iconSize;
		auto gptr = AddUserArea(g_pButtons[idx].lID, 0);
		AttachUserArea(m_buttons[idx], gptr);
	}
	GroupEnd();
	LayoutChanged(IDC_ICONGROUP);

	// Update the menus.
	MenuFlushAll();
	MenuSubBegin("View");
	MenuAddString(IDC_ICONSIZE_SMALL, check("Small Icons", iconSizeMode == IDC_ICONSIZE_SMALL));
	MenuAddString(IDC_ICONSIZE_MEDIUM, check("Medium Icons", iconSizeMode == IDC_ICONSIZE_MEDIUM));
	MenuAddString(IDC_ICONSIZE_LARGE, check("Large Icons", iconSizeMode == IDC_ICONSIZE_LARGE));
	MenuAddSeparator();
	MenuAddString(IDC_ICONORDER_HORIZ, check("Horizontal Layout", iconOrderMode == IDC_ICONORDER_HORIZ));
	MenuAddString(IDC_ICONORDER_VERT, check("Vertical Layout", iconOrderMode == IDC_ICONORDER_VERT));
	MenuAddSeparator();
	MenuAddString(IDC_ICONSINGLE, check("Dual Icons", !g_pResEditPrefs->toolbarIconSingle));
	MenuSubEnd();
	MenuFinished();
}
