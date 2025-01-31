/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined __CGUIDIALOGMANAGER_H__
#define __CGUIDIALOGMANAGER_H__
/***************************************************************************
* INCLUDES
***************************************************************************/
#include "GUIObjectHandleResource.h"
#include "eC_TList_doubleLinked.h"
#include "GUI.h"
#include "SafeGUIObjectPtr.h"


/***************************************************************************
* DEFINES
***************************************************************************/
// as Microsoft defines a Macro called CreateDialog we need to undefine it for Windows
#if defined WIN32
#undef CreateDialog
#endif

/***************************************************************************
* DECLARATIONS
***************************************************************************/

class CDescription;
class CGUIObject;

/**
**************************************************************************
@brief Base class for all application specific dialog manager

@deprecated
CGUIDialogManager is deprecated!!! In future Guiliani versions a new incompatible DialogManager will be introduced.

This class implements a dialog manager including a dialog history and a popup stack.
It also defines the interface which may be implemented in the derived class to react on certain events like:
- creating a dialog
- deleting a dialog
- activating a popup
- closing a popup

Whenever a new dialog is added (CreateDialog(), AddDialog()) the old dialog and the id of the focused
object will be stored in the history before the new one will be added.
In addition to that the old dialog will be either deleted, set invisible or
stays untouched depending on the given flag EOldDialogAction.

When the current dialog will be deleted with Back() the last recent dialog
will be come the active dialog and will be recreated, set visible depending on the result of
OnDialogDeletion() and the focus is restored.
A dialog may not allow deletion by the DialogManager if it uses a Animation to move out of the
screen and deletes itself afterwards.

To be able to create dialogs, dialog definition files have to be registered using RegisterDialogDefinition(),
or a derived class has to implement the CreateDialogImpl() and react on all IDs
which will be called with CreateDialog().

The other callback functions:
- OnDialogActivation()
- OnDialogDeactivation()
- OnDialogDeletion()
may be overwritten if neccessary.

To display popups (ShowPopup()) popup definitions have to be registered by using RegisterPopupDefinition().
The path to a dialog definition file is optional. If no path to a dialog definition file is set,
CreatePopupImpl() needs to be implemented in a derived dialog manager to provide the popup.

The dialog manager holds all popups which where activated by calling ShowPopup in the popup queue.
Only the popups with the highest priority are displayed. This may be more than one popup at a time.
Popups with a lower priority are hidden. If a popup is closed using ClosePopup() the next popup in the stack is
displayed. The popup stack is sorted by priority.
To react on popup activation and deactivation a derived class can reimplement callback functions:

- OnPopupActivation()
- OnPopupDeactivation()


@code

// Instanciate a dialogmanager
CGUIDialogManager* pDialogManager = new CGUIDialogManager();


// Add a dialog to the dialogmanager.
// 1) Add an existing dialog.
CGUICompositeObject* pkDialog = new CGUICompositeObject(&GETGUI, eC_FromInt(0), eC_FromInt(95), eC_FromInt(240), eC_FromInt(200), OBJ_MY_DIALOG);
pDialogManager->AddDialog(pkDialog);

// 2) Register a dialog streaming file which contains the dialog definition (e.g. generated by an GSE export)
// and let the dialog manager create the dialog by reading the definition file.
pDialogManager->RegisterDialogDefinition("MyDialog.xml", OBJ_MY_DIALOG);
pDialogManager->CreateDialog(OBJ_MY_DIALOG);


// 3) Derive your dialog manager and implement the CreateDialogImpl which is called when a dialog is not part of the GUI
// and no matching dialog definition is registered.

// Declaration:
class CDialogManagerTest : public CGUIDialogManager
{
public:
    CDialogManagerTest()
        :CGUIDialogManager() {}
    virtual CGUIObject* CreateDialogImpl(const ObjectHandle_t eID, eC_Bool& bAllowDeletionOUT, eC_Bool& bRecoverFocusOUT) const;

    static CDialogManagerTest& GetInstance() {return ms_Instance;}

private:
    static CDialogManagerTest ms_Instance;
};

// Definition:

CDialogManagerTest CDialogManagerTest::ms_Instance;

CGUIObject* CDialogManagerTest::CreateDialogImpl(const ObjectHandle_t eID, eC_Bool& bAllowDeletionOUT, eC_Bool& bRecoverFocusOUT) const
{
    bAllowDeletionOUT = true; // Alow deletion of all dialogs since they can be re-created by calling this function again.
    bRecoverFocusOUT = true;

    CGUICompositeObject* pkDialog = NULL;

    if (eID == OBJ_MY_DIALOG)
    {
        pkDialog = new CGUICompositeObject(&GETGUI, eC_FromInt(0), eC_FromInt(0), eC_FromInt(240), eC_FromInt(320), eID);
        new CGUITextField(pkDialog, eC_FromInt(100), eC_FromInt(100), eC_FromInt(100), eC_FromInt(20), "My Dialog");
    }
    return pkDialog;
}

// Use the dialog manager in your application code:
CDialogManagerTest::GetInstance().CreateDialog(OBJ_MY_DIALOG);

@endcode

***************************************************************************/
class CGUIDialogManager
{
public:
    /** flag describing what should happen with the old dialogs if the new one is added*/
    enum EOldDialogAction
    {
        ODA_INVISIBLE,     /**< the last dialog is set to invisible, OnDialogDeactivation is called*/
        ODA_INVISIBLE_ALL, /**< all old dialogs are set invisible, OnDialogDeactivation is called*/
        ODA_DELETE,        /**< the old dialog is deleted, OnDeletion is called.
                           If the old dialog does not allow deletion it is only set invisible.*/
        ODA_DELETE_ALL,    /**< all old dialogs are deleted, OnDeletion is called.
                           If one of the old dialog does not allow deletion it is only set invisible.*/
        ODA_NOTHING        /**< the old dialog stays unchanged, OnDialogDeactivation is called*/
    };

    /// Constructor.
    CGUIDialogManager();

    /// Destructor.
    virtual ~CGUIDialogManager() {}

    /**
    @brief Stores the old dialog and creates the new dialog with the given ID by loading
    a registered dialog definition from file or, if no matching dialog descrition was registered, by
    calling CreateDialogImpl(). If CreateDialogImpl() returns a NULL pointer for the given ID
    nothing happens.
    If an object with the given ID is already contained in the GUI it is used instead of creating a new dialog.
    The old dialog is handled depending on the given EOldDialogAction. And put on the
    history stack together with the id of the current focused object.

    @param[in] eID ID of the Dialog to be created.
    @param[in] eAction action describing what should happen with the old dialog
    @return pointer to the dialog with given ID, if no dialog has been found/created NULL is returned
    */
    CGUIObject* CreateDialog(ObjectHandle_t eID, EOldDialogAction eAction = ODA_NOTHING);

#if defined GUILIANI_STREAM_GUI
    /**
    Register a dialog definition file (streaming file).
    When CreateDialog() is called the dialog manager tries to load a registered dialog defintion
    before calling CreateDialogImpl().
    @param kDefinitionFilePath The path to the streaming file.
    @param eDialogID The Object ID which is mapped to the dialog definition.
    @return True if the definition was added, False if a dialog with the same object id was already
    registered.
    */
    eC_Bool RegisterDialogDefinition(const eC_String& kDefinitionFilePath, const ObjectHandle_t eDialogID);

    /**
        Unregister a dialog definition.
        @param eDialogID The ID of the dialog whose definition shall be removed.
        @return True if the registered path was successfully removed, else False.
    */
    eC_Bool UnregisterDialogDefinition(const ObjectHandle_t eDialogID);


    /**
    Returns the path for a registered dialog or a registered popup.
    @param eDialogID The dialog id that shall be checked.
    @return The path which was registered for the dialog id or empty string if no description was registered.
    */
    eC_String GetRegisteredPath(const ObjectHandle_t eDialogID) const;

    /**
        Returns the dialog id (or popup id) for a registered dialog path.
        @param kDialogPath The dialog path
        @return The ID
    */
    ObjectHandle_t GetRegisteredDialogID(const eC_String& kDialogPath) const;
#endif
    /**
    **************************************************************************
    @brief Stores the old dialog and adds the new given dialog.

    The old dialog is handled depending on the given EOldDialogAction. And put on the
    history stack together with the id of the current focused object.

    @param[in] pNewCurrentDialog pointer to the new dialog. if this is NULL, nothing happens
    @param[in] eAction action describing what should happen with the old dialog
    @param[in] bRecoverFocus True if the dialog manager should recover the focus when pNewCurrentDialog
    comes active again, after it was inactive, because another dialog has been added.
    ***************************************************************************/
    void AddDialog(CGUIObject* pNewCurrentDialog, const EOldDialogAction eAction = ODA_NOTHING, const eC_Bool bRecoverFocus = true);

    /**
    @brief Deletes the current active dialog and activates the last one from the history i.e. implements a back functionality.

    The current dialog is not deleted if the history is empty.
    To clear the current dialog and the history use ClearDialogHistory(0).

    First OnDeletion is called and if this returns true the dialog will be deleted, otherwise not.
    Then the last dialog from the history stack is activated.
    @param[in] bDeleteObject if true the object will be tried to delete
    */
    void Back(const eC_Bool bDeleteObject = true);

    /**
    @brief Deletes the current active dialog and all dialogs in the history
    that has not been deleted so far (ODA_INVISIBLE, ODA_NOTHING). If the current
    dialog is deleted, activates the last one from the history, if one is left.

    First OnDialogDeletion is called and if this returns true the dialog will be deleted, otherwise not.
    This applies also for all dialogs that are in the history  and have not been deleted so far.
    Then the last dialog from the history stack is activated if there is any. If uiKeepDialogs
    is 0 then there will be no current dialog after calling this method.

    @param[in] uiKeepDialogs number of dialogs that should not be deleted. Default value is 1.
    @param[in] bDeleteDialogs Determines if the dialogs are also deleted. Default value is true.
    @param[in] bDeleteCurrent Determines if the current dialog is also deleted. Default value is true
    */
    void ClearDialogHistory(const eC_UInt uiKeepDialogs = 1, const eC_Bool bDeleteDialogs = true, const eC_Bool bDeleteCurrent = true);

    /**
    @brief Goes back to the Dialog with the given ID.
    If the given Dialog ID is not part of the history stack, nothing happens.
    The Current dialog and all dialogs in the history "after" the dialog with the given ID
    will be deleted or not depending on bDeleteDialogsIfAllowed.

    @param[in]   eDialogID dialog ID to that should be jumped back.
    @param[in]   bDeleteDialogsIfAllowed Determines if the dialog objects are deleted.
                 The dialogs can only be deleted if it was allowed when creating them.
    @param[in]   bHistoryActivation Determines if each dialog that is removed from the
                 history shall be activated and deactivated again before removing it.
    @return[out] True if the dialog with the given ID was part of the history stack,
                 otherwise False.
    */
    eC_Bool BackToDialog(const ObjectHandle_t eDialogID, const eC_Bool bDeleteDialogsIfAllowed, const eC_Bool bHistoryActivation = false);

    /**
    Accessor for the ID of the current active dialog
    @return The object ID of the currently active dialog, or NO_HANDLE if
    there is no active dialog.
    **/
    inline ObjectHandle_t GetCurrentDialogID() const
    {
        return m_CurrentDialog.m_eDialogID;
    }

    /**
    Check wether the given dialog is part if the current dialog history
    or not.
    @param[in] eDialogID dialog ID that should be checked.
    @param[in] bConsiderCurrentDialog if true the current dialog is taken into account, otherwise not
    @return True if the dialog with the given ID is part of the history stack,
    otherwise False.
    **/
    eC_Bool ContainsDialog(const ObjectHandle_t eDialogID, const eC_Bool bConsiderCurrentDialog = true) const;

    /**
    Accessor for the ID of the last active dialog
    @return The object ID
    **/
    ObjectHandle_t GetLastDialogID() const;

    /**
    Accessor for the current active dialog.
    The returned dialog MUST NOT been deleted! Use Back() instead!!!
    @return The object
    **/
    inline CGUIObject* GetCurrentDialog() const {return GETGUI.GetObjectByID(m_CurrentDialog.m_eDialogID);}

    /** Returns the number of entries on the dialog history stack. This method
    returns zero if there is no or one active dialog.
    @return Number of dialogs on the history stack.
    */
    inline eC_UInt GetHistoryStackSize() const { return m_History.GetQuantity(); }

    /**
    **************************************************************************
    Creates a dynamic ObjectHandle/ID this will be used for controls
    which do not have a static ID, but if a ID is needed e.g. for storing
    a focus by the DialogManager.

    @return new dynamic object ID. will be creater then OBJ_DYNAMIC_START_TAG
    **************************************************************************/
    ObjectHandle_t CreateDynamicObjectID();

    /** flag describing the priority of a popup*/
    enum PopupPriority_t
    {
        PRIO_HIGHEST = 9,
        PRIO_VERY_HIGH = 8,
        PRIO_HIGH = 7,
        PRIO_ABOVE_MEDIUM = 6,
        PRIO_MEDIUM = 5,
        PRIO_BELOW_MEDIUM = 4,
        PRIO_LOW = 3,
        PRIO_VERY_LOW = 2,
        PRIO_LOWEST = 1
    };

    /**
    Register a popup definition. This definition is used to create a popup from file or if no file is given to define its priority and display time.
    When CreatePopup() is called the dialog manager tries to load a registered popup defintion
    before calling CreatePopupImpl().
    @param ePopupID The id of the popup which is used to access it.
    @param ePriority The priority of the Popup.
    @param uiDisplayTime The display time of the Popup. After uiDisplayTime the popup will be closed automatically.
    @param bDeleteOnClose Determines if the popup is deleted or hidden when it is closed. A popup is closed by timeout or by calling ClosePopup().
    @param kDefinitionFilePath The path to the streaming file.
    @return True if the popup was registered, False if a popup with the id was already registered.
    @see ShowPopup()
    */
    eC_Bool RegisterPopupDefinition(
        const ObjectHandle_t ePopupID,
        const PopupPriority_t ePriority = PRIO_LOW,
        const eC_UInt uiDisplayTime = 0,
        const eC_Bool bDeleteOnClose = true,
        const eC_String& kDefinitionFilePath ="");

        /**
        Unregister a popup definition.
        @param ePopupID The ID of the popup whichs defintion shall be removed.
        @return True if the registered path was successfully removed, else False.
    */
    eC_Bool UnregisterPopupDefinition(const ObjectHandle_t ePopupID);

    /**
    Update the registered display time of a popup.
    @param eID The id of the popup whose display time is updated.
    @param uiDisplayTime The Display time that shall be set in ms. 0 ms means no timeout.
    @return True if the display time was sucessfully updated, False if the popup was not registered.
    */
    eC_Bool UpdateRegisteredPopupDisplayTime(const ObjectHandle_t eID, const eC_UInt uiDisplayTime);

    /**
        Get the display time of a popup. 0 means no timeout.
        @param[in] eID The id of the popup.
        @param[out] uiDisplayTime The display time of the popup.
        @return True if the display time was successfully returned in uiDisplayTime, False if there is no popup with the given id.
    */
    eC_Bool GetRegisteredPopupDisplayTime(const ObjectHandle_t eID, eC_UInt& uiDisplayTime);

    /**
    Creates and displays a popup. It is necessary to register a popup with its priority and display time.
    The path to the dialog definition (streaming file) is optional.
    If a popup is not registered it will not be displayed.

    A popup is created by loading the dialog from the streaming file, or if no path is set for the popup by calling CreatePopupImpl().
    If the popup is already part of the GUI, it will be set visible instead of re-creating it.
    If the popup is already displayed with timeout, the timer is restarted.
    @param eID The identifier of the popup.
    @return The pointer to the displayed popup.
    @see RegisterPopupDefinition()
    @see ClosePopup()
    */
    CGUIObject* ShowPopup(const ObjectHandle_t eID);

    /**
    Close a popup. If the popup is deleted or not is determined on registration. @see RegisterPopupDefinition().
    @param eID The identifier of the popup. If NO_HANDLE, the popup with the highest priority (displayed on top) is closed.
    @return True if the popup was closed, False if no popup was closed.
    */
    eC_Bool ClosePopup(const ObjectHandle_t eID);

    /** Determines if a popup is currently displayed.
    @param eID The id of the popup.
    @return True if the popup is currently displayed, else False.
    */
    eC_Bool IsPopupDisplayed(const ObjectHandle_t eID);

    /**
    Determines if a popup is part of the list of active popups.
    @param eID The id of the popup.
    @return True if the popup is part of the list, False if the popup was not found.
    @see IsPopupDisplayed().
    */
    eC_Bool IsPopupInPopupList(const ObjectHandle_t eID) const;

    /// @brief Holds information for a registered popup.
    struct RegisteredPopup_t
    {
        eC_String m_kPath;              ///< The path of the description file.
        ObjectHandle_t m_ePopupID;      ///< The id of the popup.
        PopupPriority_t m_ePriority;    ///< The priority of the popup.
        eC_UInt m_uiDisplayTime;        ///< The display time in ms.
        eC_Bool m_bDeleteOnClose;       ///< Determines if the popup is deleted or not.
    };

    /**
    Get the information for a registered popup.
    @param[in] eID The id of the popup.
    @param[out] kPopupInfo The Popup info.
    @return True if the popup was found and kPopupInfo was updated with the information, False if no popup with the id eID was registered.
    */
    eC_Bool GetRegisteredPopupInfo(const ObjectHandle_t eID, RegisteredPopup_t& kPopupInfo) const;

protected:

    struct HistoryEntry;

    /// @brief Struct describing a dialog
    struct DialogDescription
    {
        ObjectHandle_t     m_eDialogID; /**< id of the dialog*/
        EOldDialogAction   m_eOldDialogAction; /**< type of action wich is used when this dialog has been created */
        eC_Bool            m_bAllowObjectDeletion; /**< flag describing whether this dialog alows the deltion of the GUIObject or not*/
        eC_Bool            m_bRecoverFocus; /**< flag describing whether the focus for this dialog should be recovered or not*/

        /** Constructor
            @param eCurrentDialogID The dialog ID
            @param eOldDialogAction The old dialog action
            @param bAllowObjectDeletion Whether to allow object deletion
            @param bRecoverFocus Whethr to recover the focus
        */
        DialogDescription(ObjectHandle_t eCurrentDialogID = NO_HANDLE,
            EOldDialogAction eOldDialogAction = ODA_NOTHING,
            eC_Bool bAllowObjectDeletion = true,
            eC_Bool bRecoverFocus = true) :
            m_eDialogID(eCurrentDialogID),
            m_eOldDialogAction(eOldDialogAction),
            m_bAllowObjectDeletion(bAllowObjectDeletion),
            m_bRecoverFocus(bRecoverFocus)
        {}
    };

    /// @brief Struct describing a history entry
    struct HistoryEntry
    {
        /** description of the dialog*/
        DialogDescription m_Dialog;

        /** action was used when the dialog was put on the history.
            Same as the m_eOldDialogAction in the "next" dialog.
        */
        EOldDialogAction  m_eAction;

        /** id of the focused object when the dialog has been put on the history*/
        ObjectHandle_t    m_eFocussedObject;

        /** Constructor
            @param dialog The dialog description
            @param eAction The action
            @param eFocussedObject The focused object
        */
        HistoryEntry(const DialogDescription& dialog = DialogDescription(),
            EOldDialogAction eAction = ODA_DELETE_ALL,
            ObjectHandle_t eFocussedObject = NO_HANDLE) :
                m_Dialog(dialog),
                m_eAction(eAction),
                m_eFocussedObject(eFocussedObject){}

        /** Comparisom operator
            @param rCompare what to compare with
            @result True if identical, False otherwise
        */
        bool operator==(const HistoryEntry& rCompare)
        {
            return (m_Dialog.m_eDialogID == rCompare.m_Dialog.m_eDialogID);
        }

        //eC_Bool AllowsDeletion(){return m_Dialog.m_bAllowObjectDeletion);}
    };

    typedef eC_TListDoubleLinked<HistoryEntry> CDialogHistoryList_t; /**< List of HistoryEntries*/

    /**
        Returns a constant reference to the dialog history.
        @return The history list
    */
    const CDialogHistoryList_t& GetDialogHistory() const;

    /**
    Factory method for the dialogs created with CreateDialog().

    @param[in] eID ID of the dialog to create
    @param[out] bAllowDeletionOUT set this to True if the created dialog may be
    deleted by the dialog manager, False otherwise
    @param[out] bRecoverFocusOUT set this to True if the dialog manager
    should recover the focus when this dialog comes active again,
    after it was inactive, because another dialog has been added.

    @return pointer to the new created dialog
    */
    virtual CGUIObject* CreateDialogImpl(const ObjectHandle_t eID, eC_Bool& bAllowDeletionOUT, eC_Bool& bRecoverFocusOUT) const
    {
        GUILOG(GUI_TRACE_DEBUG, eC_String("CGUIDialogManager::CreateDialogImpl has been called.") +
            eC_String("Maybe you have forgotten to implement the CreateDialogImpl in you derived class.\n"));
        return NULL;
    }

    /**
        Helper funciton for CreateDialog() and RecreateDialog() providing the dialog object.
        Checks if the object is already available in the GUI.
        If not registered dialog definitions are checked by calling LoadDialogDescription().
        If no dialog is registered, CreateDialogImpl is called.
        @param eID The id of the dialog.
        @param[in,out] bAllowDeletion Determines if the deletion of the dialog is allowed.
        @param[in,out] bRecoverFocus Determines if the focus shall be recovered.
        @return Pointer to the Dialog or NULL if the dialog was not found and it was not possible to create it.
    */
    CGUIObject* GetDialogObject(const ObjectHandle_t eID, eC_Bool& bAllowDeletion, eC_Bool& bRecoverFocus) const;

    /** Checks the registered dialog descriptions and loads
    the dialog from file if a definition file was found.
    @param eID The id of the dialog description that shall be loaded.
    @return The loaded dialog.
    */
#if defined GUILIANI_STREAM_GUI
    CGUIObject* LoadDialogDescription(const ObjectHandle_t eID) const;
#endif

    /**
    Callback that is called when a dialog has been added, either by AddDialog()
    or by CreateDialog().
    In this callback new dialogs may be added.
    @param[in] pDialog pointer of the dialog that has been added
    */
    virtual void OnDialogAdded(CGUIObject* pDialog){}

    /**
    Callback that is called when a dialog becomes active again.
    @param[in] pDialog pointer of the dialog that becomes active again
    */
    virtual void OnDialogActivation(CGUIObject* pDialog){}

    /**
    Callback that is called when a dialog become inactive.
    A dialog becomes active again if it has been added to the history with ODA_INVISIBLE or ODA_NOTHING
    @param[in] pDialog pointer of the dialog that becomes active again
    */
    virtual void OnDialogDeactivation(CGUIObject* pDialog){}

    /**
    Callback that is called when a dialog will be deleted by either Back()
    or ClearDialogHistory.
    If this method returns true the dialog will be deleted, otherwise not (only removed from the DialogManager).
    A dialog may not be deleted by the DialogManager if it e.g. uses a Animation to move out of the
    screen and deletes itself afterwards.
    @param[in] pDialog pointer of the dialog that will be deleted
    @return True if the dialog may be deleted, otherwise False
    */
    virtual eC_Bool OnDialogDeletion(CGUIObject* pDialog){return true;}

    /**
    Callback that is called after a Dialog has been deleted.
    @param[in] eDeletedDialogID of the deleted dialog
    @param[in] eNewDialogID of the new dialog. NO_HANDLE if the current dialog is not touched
               since dialog are only deleted from history. @see ClearDialogHistory()
    */
    virtual void OnAfterDialogDeletion(ObjectHandle_t eDeletedDialogID, ObjectHandle_t eNewDialogID){}

    /**
    Called after the last dialog has been deleted and no new one could be
    created from the history.
    This base implementation does nothing.
    */
    virtual void OnLastDialogDeletion() {}

    /**
    Callback that is called when a popup becomes active.
    @param[in] pPopup pointer of the popup that becomes active.
    */
    virtual void OnPopupActivation(CGUIObject* pPopup){}

    /**
    Callback that is called when a popup is closed.
    @param[in] pPopup pointer of the dialog that becomes closed.
    */
    virtual void OnPopupDeactivation(CGUIObject* pPopup){}

    /**
    Callback that is called when a popup needs to be created and no dialog description (streaming file) was registered.
    It is necessary that the popup is has been registered with it's priority and time out prior to calling ShowPopup.
    Otherwise CreatePopupImpl() will not be called.
    @param[in] eID The id of the popup which shall be created.
    @return The created popup.
    */
    virtual CGUIObject* CreatePopupImpl(const ObjectHandle_t eID)
    {
        GUILOG(GUI_TRACE_DEBUG, eC_String("CGUIDialogManager::CreatePopupImpl has been called.") +
            eC_String("Maybe you have forgotten to implement the CreateDialogImpl in you derived class.\n"));
        return NULL;
    }

    /**
        @brief Helper class for the popup handling.
    */
    class PopupDescription : public CGUIAnimatable
    {
    public:
        /**
        Constructor.
        @param pPopup The popup.
        @param ePriority The priority of the popup.
        @param uiDisplayTime The display time of the popup in ms. 0 means no timeout.
        @param bDeleteOnClose  Determines if the popup is deleted when it is closed.
        @param pkDialogManager Instance of the dialog manager which is called to close the popup after uiDisplayTime.
        */
        PopupDescription(CGUIObject* pPopup, const PopupPriority_t ePriority, const eC_UInt uiDisplayTime, const eC_Bool bDeleteOnClose, CGUIDialogManager* pkDialogManager)
            : m_pObject(pPopup), m_pDialogManager(pkDialogManager), m_ePriority(ePriority), m_uiDisplayTime(uiDisplayTime), m_bDeleteOnClose(bDeleteOnClose)
        {
        }

        /// Default constructor.
        PopupDescription()
            : m_pObject(NULL),
            m_pDialogManager(NULL),
            m_ePriority(PRIO_LOW),
            m_uiDisplayTime(0),
            m_bDeleteOnClose(true)
        {}

        /// Destructor
        ~PopupDescription()
        {
        }

        /// Callback called when display time is over. Closes the popup.
        virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1))
        {
            if (m_pDialogManager != NULL && m_pObject != NULL)
            {
                GETTIMER.RemoveAnimationCallback(this);
                m_pDialogManager->ClosePopup(m_pObject->GetID());
            }
        }

        /// Determines if two popups have the same id.
        eC_Bool operator== (const ObjectHandle_t eID)
        {
            return (m_pObject != NULL && m_pObject->GetID() == eID);
        }

        /// Operator to add the popups sorted to the active popup list.
        eC_Bool operator >(const PopupDescription& ePopup) const
        {
            //lowest priority i.e. lowest number shall be in front.
            return m_ePriority > ePopup.m_ePriority;
        }

        /// Returns the priority of the popup.
        PopupPriority_t GetPriority() const {return m_ePriority;}

        /// Returns the popup.
        CGUIObject* GetObject() const {return m_pObject;}

        /// Retruns the display time of the popup.
        eC_UInt GetDisplayTime() const {return m_uiDisplayTime;}

        /// Holds the popup.
        CSafeGUIObjectPtr m_pObject;

        /// Pointer used to close popups after display time.
        CGUIDialogManager* m_pDialogManager;

        /// The priority of the popup.
        PopupPriority_t m_ePriority;

        /// The display time of the popup.
        eC_UInt m_uiDisplayTime;

        /// Determines if the popup is deleted when it is closed.
        eC_Bool m_bDeleteOnClose;
    };

    /**
    Activates all popups with highest priority, and deactivates all popups with lower priority.
    */
    void UpdateActivePopups();

    /**
    Get the popup description of an active popup.
    @param eID The id of the popup.
    @return Pointer to the popup description. NULL if the popup is not active.
    */
    PopupDescription* GetActivePopup(const ObjectHandle_t eID) const;

private:

   void PrintDialogHistory();

    // finds a dialog discription in the history for given ID, returns NULL on failure
    HistoryEntry* FindDialogInHistory(const ObjectHandle_t eID) const;

    // Tries to set the focus to the given object.
    // If this is not possible, this function iterates through the parents and tries to set any focus.
    void RecoverFocus(CGUIObject* pObjectToFocus);

    // Sets the focus to added or activated dialog if it wasn't set before.
    void EnsureFocus(CGUIObject* pAddedDialog);

    // a dialog is created again
    CGUIObject* ReCreateDialog(const DialogDescription& dialog) const;

    // deletes the dialog with the given description
    void DeleteDialog(const DialogDescription& dialog, const eC_Bool bDeleteObject = true);

    // makes a dialog visible and draws it on top.
    void MakeDialogVisible(CGUIObject* pkDialog) const;

    // makes the dialog with the given id invisble
    void MakeDialogInvisible(CGUIObject* pkDialog);

    // store the current dialog on the history
    void StoreCurrentDialogInHistory(const EOldDialogAction eAction, CGUIObject* pFocusedObj);

    // forwards the action to all dialogs in the history
    void ApplyActionToHistory(const EOldDialogAction eAction);

    // creates the last added dialog from the history and removes it from it
    CGUIObject* CreateDialogFromHistory();

    CDialogHistoryList_t m_History; // lifo history stack
    DialogDescription m_CurrentDialog;  // description of the current dialog

    CSafeGUIObjectPtr m_pCurrentDialogFocus; // Holds the focused object of the current dialog when a popup is opend to recover the focus when the popup is closed.

    eC_Int m_iLastDynObjID; ///< The last used dynamic object ID.
    eC_TListDoubleLinked<eC_Int> m_DynamicObjectIDs; ///< list of all dynamic created object IDs

#if defined GUILIANI_STREAM_GUI
    /// Holds information for a registered dialog, used to load a dialog description from file.
    struct RegisteredDialog_t
    {
        eC_String m_kPath; ///< The path of the description file.
        ObjectHandle_t m_eDialogID; ///< The id of the dialog.
    };

    /// Type for the list of registered dialogs.
    typedef eC_TListDoubleLinked<RegisteredDialog_t> RegisteredDialogList_t;
    RegisteredDialogList_t m_RegisteredDialogDefinitions; ///< list of all registered dialogs.
#endif

    /// Type for the list of registered popups.
    typedef eC_TListDoubleLinked<RegisteredPopup_t> RegisteredPopupList_t;
    RegisteredPopupList_t m_RegisteredPopupDefinitions; ///< list of all registered popups.

    /// Type for the list of active popups.
    typedef eC_TListDoubleLinked<PopupDescription> PopupDescriptionList_t;
    PopupDescriptionList_t m_ActivePopups; ///< list of active popups.
};

#endif //__CGUIDIALOGMANAGER_H__
