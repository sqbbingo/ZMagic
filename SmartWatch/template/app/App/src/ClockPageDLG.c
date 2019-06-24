/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
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
#define ID_WINDOW_0         (GUI_ID_USER + 0x00)
#define ID_TEXT_0         (GUI_ID_USER + 0x03)
#define ID_TEXT_1         (GUI_ID_USER + 0x04)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "ClockPage", ID_WINDOW_0, 0, 20, 320, 200, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "time", ID_TEXT_0, 20, 40, 280, 60, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "date", ID_TEXT_1, 20, 110, 280, 40, 0, 0x64, 0 },
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
  // USER START (Optionally insert additional variables)
  Calendar_t calendar;
  char buf[64]={0};
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'ClockPage'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(GUI_LIGHTGRAY));
    //
    // Initialization of 'time'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(GUI_BLACK));
    TEXT_SetFont(hItem, GUI_FONT_D48);
    TEXT_SetText(hItem, "00:00:00");
    //
    // Initialization of 'date'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(GUI_BLACK));
    TEXT_SetFont(hItem, FontList24[System.language]);
    TEXT_SetText(hItem, "0000/00/00 week");
    // USER START (Optionally insert additional code for further widget initialization)
    calendar = Calendar_Get();
    sprintf(buf,"%02d:%02d:%02d",calendar.hour,calendar.minute,calendar.second);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), buf);
    
    sprintf(buf,"%04u/%02u/%02u %s",calendar.year,calendar.month,calendar.day,WeekText[System.language][calendar.week]);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), buf);
    // USER END
    break;
  case WM_TIMER://��ʱ����Ϣ,��ʱ��ʱ��Ч
    WM_RestartTimer(pMsg->Data.v, 50);
    calendar = Calendar_Get();
    sprintf(buf,"%02d:%02d:%02d",calendar.hour,calendar.minute,calendar.second);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), buf);
    
    sprintf(buf,"%04u/%02u/%02u %s",calendar.year,calendar.month,calendar.day,WeekText[System.language][calendar.week]);
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), buf);
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
*       CreateClockPage
*/
WM_HWIN CreateClockPage(void);
WM_HWIN CreateClockPage(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  WM_CreateTimer(hWin, 0x80, 50, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/