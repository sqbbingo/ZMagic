/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.26                          *
*        Compiled Aug 18 2014, 17:12:05                              *
*        (c) 2014 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x1C)
#define ID_RADIO_0   (GUI_ID_USER + 0x1D)
#define ID_BUTTON_0   (GUI_ID_USER + 0x1E)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
const char LanguageText[][2][16] = {
  {
    "English",
    "Chinese",
  },
  {
    "英语",
    "简体中文",
  },
};
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "LanguageSet", ID_FRAMEWIN_0, 60, 40, 200, 140, 0, 0x64, 0 },
  { RADIO_CreateIndirect, "Radio LanList", ID_RADIO_0, 15, 10, 160, 60, 0, 0x1402, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_0, 55, 80, 80, 30, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  short index = -1;
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'LanguageSet'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetTitleHeight(hItem, 20);
    FRAMEWIN_SetFont(hItem, FontList16[System.font]);
    FRAMEWIN_SetText(hItem, "LanguageSetting");
    //
    // Initialization of 'Radio LanList'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
    RADIO_SetFont(hItem, FontList16[System.font]);
    RADIO_SetText(hItem, LanguageText[System.language][0], 0);
    RADIO_SetText(hItem, LanguageText[System.language][1], 1);
    RADIO_SetValue(hItem,System.language);
    //
    // Initialization of 'OK'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    BUTTON_SetText(hItem, "OK");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_RADIO_0: // Notifications sent by 'Radio LanList'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'OK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
        index = RADIO_GetValue(hItem);
        if(index != -1)
        {
          System.language = index;
          if(System.language == 1)
          {
            if(Creat_XBF_Font() != 0)//创建XBF字体
            {
              System.language = 0; 
              ErrorDialog("Creat_XBF_Font","Font Creat error!");
            }
          }
        }
        WM_DeleteWindow(pMsg->hWin);
        process_post_synch(&SettingProcess,AppInputEvent,"SetLanguage");
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateLanguageSet
*/
WM_HWIN CreateLanguageSet(void);
WM_HWIN CreateLanguageSet(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWinSetting, 0, 0);
  FRAMEWIN_SetMoveable(hWin,1);
  WM_SetStayOnTop(hWin, 1);
  WM_MakeModal(hWin);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/