/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef _MYGUI_SR_H_
#define _MYGUI_SR_H_

#include "GUI.h"
#include "GUIButton.h"
#include "GUIComboBox.h"
#include "GUIListItem.h"

#include "StreamRuntimeGUI.h"
#include "GUIObjectHandleResource.h"


#define GETMYGUI    static_cast<CMyGUI&>(GETGUI)

typedef enum _media_t {
	GUI_MSG_PLAY,
	GUI_MSG_STOP,
	GUI_MSG_EXIT
} media_t;


// Application specific CGUI instance. Implemented by customer.
class CMyGUI : public NStreamRuntime::CStreamRuntimeGUI
{

public:
    CMyGUI(eC_Value x, eC_Value y, eC_Value width, eC_Value height, ObjectHandle_t eID);
    ~CMyGUI();

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

    /// Example implementation for simple Application <-> GUI communication
    eC_Bool CallApplicationAPI(const eC_String& kAPI, const eC_String& kParam);

private:
    void CreateFileList ( const eC_Char* dir );

private:
    CGUIComboBox* m_pComboBox;
    CGUIButton* m_pPlayButton;
    CGUIButton* m_pStopButton;
    uint8_t m_working_drive = 'A';
    os_msg_queue_handle_t m_media_queue;


};



#endif //#ifndef _MYGUI_SR_H_
