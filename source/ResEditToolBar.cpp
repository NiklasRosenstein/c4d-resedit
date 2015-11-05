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

	IDC_ICONSIZE,
	IDC_ICONGROUP,

	IDC_DUMMY // just for the mac comiler :-)
};

typedef struct {
	Int32 lID;
	ItemType type;
	Int32 x, y;
} _TagButtonInfo;

_TagButtonInfo g_pButtons[17] = {
	{ IDC_STATIC_BTN,       Static, 1, 0 },
	{ IDC_EDIT_BTN,         EditBox, 7, 0 },

	{ IDC_BUTTON_BTN,       Button, 5, 0 },
	{ IDC_ARROW_BTN,        ArrowBtn, 6, 0 },

	{ IDC_CHECK_BOX_BTN,    CheckBox, 3, 0 },
	{ IDC_RADIO_BTN,        RadioBtn, 4, 0 },

	{ IDC_LIST_VIEW_BTN,    ListBox, 2, 1 },
	{ IDC_COMBO_BOX_BTN,    ComboBox, 3, 1 },
	{ IDC_COMBO_BUTTON_BTN, ComboButton, 8, 1 },

	{ IDC_GROUP_BTN,        Group, 2, 0 },
	{ IDC_DIALOG_GROUP_BTN, DialogGroup, 4, 1 },

	{ IDC_SEPARATOR_BTN,    Separator, 1, 1 },
	{ IDC_USER_AREA_BTN,    UserArea, 5, 1 },

	{ IDC_SLIDER_BTN,       Slider, 8, 0 },
	{ IDC_COLOR_FIELD_BTN,  Color, 0, 1 },

	{ IDC_SUB_DLG_BTN,      SubDialogControl, 6, 1 },
	{ IDC_CUSTOM_ELEMENT_BTN,      CustomElement, 7, 1 },
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

	GroupBeginInMenuLine();
	AddComboBox(IDC_ICONSIZE, 0);
	AddChild(IDC_ICONSIZE, 20, "20");
	AddChild(IDC_ICONSIZE, 32, "32");
	SetInt32(IDC_ICONSIZE, 32);  // XXX: Load/Save icon size in preferences
	GroupEnd();

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
		case IDC_ICONSIZE:
			UpdateToolbarIcons();
			return true;
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
	LayoutFlushGroup(IDC_ICONGROUP);
	Int32 iconSize = 20;
	GetInt32(IDC_ICONSIZE, iconSize);

	IconData icon;
	icon.bmp = g_pControlImages;
	icon.w = 20;
	icon.h = 20;

	GroupBegin(0, 0, 2, 1, "", 0);
	for (Int32 idx = 0; idx < TOOLBAR_BUTTON_COUNT; ++idx) {
		icon.x = g_pButtons[idx].x * 20;
		icon.y = g_pButtons[idx].y * 20;
		m_buttons[idx] = IconButton(icon, iconSize, iconSize);
		auto gptr = AddUserArea(g_pButtons[idx].lID, 0);
		AttachUserArea(m_buttons[idx], gptr);
	}
	GroupEnd();
	LayoutChanged(IDC_ICONGROUP);
}
