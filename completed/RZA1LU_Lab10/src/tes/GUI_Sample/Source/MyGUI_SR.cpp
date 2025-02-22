/*
* Copyright (C) 2004 TES Electronic Solutions GmbH,
* All Rights Reserved.
* This source code and any compilation or derivative thereof is the
* proprietary information of TES Electronic Solutions GmbH
* and is confidential in nature.
* Under no circumstances is this software to be exposed to or placed
* under an Open Source License of any type without the expressed
* written permission of TES Electronic Solutions GmbH
*
*############################################################
*/

/******************************************************************************
*   PROJECT:        Guiliani
*******************************************************************************
*
*    MODULE:        MyGUI_SR.cpp
*
*    Archive:       $URL: https://10.25.129.51:3690/svn/GSE/branches/Releases/1.0_Guiliani_2.1/StreamRuntime/src/MyGUI_SR.cpp $
*
*    Date created:  2005
*
*
*
*    Author:        JRE
*
*******************************************************************************
*   MODIFICATIONS
*******************************************************************************
*    ID
*    --------------------------------------------------------------------------
*    $Id: MyGUI_SR.cpp 2159 2014-11-26 15:36:46Z christian.euler $
*
******************************************************************************/
#include "MyGUI_SR.h"

#include "UserConfig.h"
#define USB_ENABLED	1
#ifdef GFX_USE_EGML
#include "GfxWrapeGML.h"
#endif

#include "StreamRuntimeConfig.h"
//#include "GUIFramerate.h"

extern "C"
{
/* Dependencies */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "r_devlink_wrapper_cfg.h"
#include "dskManager.h"
#include "r_fatfs_abstraction.h"
#include "ff.h"


}


// LAST INCLUDE!!
#include "GUIMemLeakWatcher.h"



CMyGUI::CMyGUI(
    eC_Value x, eC_Value y,
    eC_Value width, eC_Value height,
    ObjectHandle_t eID) :
    CStreamRuntimeGUI(x, y, width, height, eID)

{
	// Get pointer to ComboBox
	m_pComboBox =  dynamic_cast<CGUIComboBox*>(GETGUI.GetObjectByID(AID_COMBOBOX_1));



	// Mount all Devices
	if ( 0 < dskMountAllDevices() ) {

		f_chdrive("A:\\");
		// Query for Text files
		CreateFileList ("*.txt");
	}

}

CMyGUI::~CMyGUI()
{
    // Add application specific de-initialization here if necessary


	delete m_pComboBox;
	m_pComboBox = NULL;



}

void CMyGUI::CreateFileList ( const eC_Char* pattern ) {

	char full_path[] = "A:\\";


	// Get pointer to FSFAT drive
	void *pDrive = dskGetDrive(m_working_drive);

	if ( pDrive == NULL)
		return;

	FATENTRY fatEntry;
	FRESULT fatResult;

	DIR dir;

	fatResult = R_FAT_FindFirst( &dir, &fatEntry, full_path, pattern);

	while ( FR_OK == fatResult ) {
		// Create Item with file entry
		CGUIListItem * pListItem = new CGUIListItem(NULL, 0, 0, eC_FromInt(80), eC_FromInt(20), fatEntry.FileName );
		// Modify the newly created item's label
		pListItem->GetLabel()->SetAligned(CGUIText::V_CENTERED);
		pListItem->GetLabel()->SetTextColor(0xff000000, 0xffffffff, 0xff000000, 0xffffffff);
		// Add it to the ComboBox
		m_pComboBox->AddItem(pListItem);

		/* Get the next one */
		fatResult = R_FAT_FindNext( &dir, &fatEntry);
	}

	// Set some visualization parameters. (e.g. Colors and Images)
	m_pComboBox->SetItemSelectedColor(0xff0000cc);
	m_pComboBox->SetHeaderButtonImages(
		IMG_STDCTRL_IMGBTN_STANDARD,
		IMG_STDCTRL_IMGBTN_PRESSED,IMG_STDCTRL_IMGBTN_HIGHLIGHTED,
		IMG_STDCTRL_IMGBTN_GRAYED_OUT,IMG_STDCTRL_IMGBTN_FOCUSED);
	m_pComboBox->GetHeader()->SetInputFieldImages(
		IMG_STDCTRL_INPUTFIELD_STANDARD,
		IMG_STDCTRL_INPUTFIELD_HIGHLIGHTED,
		IMG_STDCTRL_INPUTFIELD_FOCUSSED,
		IMG_STDCTRL_INPUTFIELD_GRAYEDOUT);
	// Select an item by index
	m_pComboBox->SetSelection(0);
	// Make the comboxbox-header editable, so that users can enter a search-string
	m_pComboBox->SetHeaderEditable(true);

}

void CMyGUI::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if (NULL != pkUpdatedObject)
    {

    }
}


eC_Bool CMyGUI::CallApplicationAPI(const eC_String& kAPI, const eC_String& kParam)
{

    return true;
}





