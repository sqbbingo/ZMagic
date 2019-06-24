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
#define ID_BUTTON_0         (GUI_ID_USER + 0x01)
#define ID_TEXT_0         (GUI_ID_USER + 0x02)
#define ID_TEXT_1         (GUI_ID_USER + 0x03)
#define ID_LISTVIEW_0         (GUI_ID_USER + 0x04)
#define ID_BUTTON_1         (GUI_ID_USER + 0x05)
#define ID_BUTTON_2         (GUI_ID_USER + 0x06)
#define ID_BUTTON_3         (GUI_ID_USER + 0x07)
#define ID_BUTTON_4         (GUI_ID_USER + 0x08)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
static Calendar_t Calendar;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Calender", ID_WINDOW_0, 0, 20, 320, 220, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "<<", ID_BUTTON_0, 0, 0, 40, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "title", ID_TEXT_0, 50, 0, 220, 25, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "date", ID_TEXT_1, 60, 30, 200, 25, 0, 0x64, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 20, 55, 280, 152, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "<<", ID_BUTTON_1, 20, 30, 40, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ">>", ID_BUTTON_2, 260, 30, 40, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "<", ID_BUTTON_3, 70, 30, 40, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ">", ID_BUTTON_4, 210, 30, 40, 25, 0, 0x0, 0 },
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
void DrawCalendar(WM_HWIN hItem,Calendar_t calendar)
{
  char buf[32];
  short temp,x,y;
  Calendar_t date = Calendar_Get();
  
  calendar.day = 1;
  temp = CountWeek(calendar.year,calendar.month,calendar.day);
  if(temp == 0) temp = 6;
  else temp-=1;
  
  for(y=0;y<6;y++)
  {
    for(x=0;x<7;x++)
    {
      if(y == 0)
      {
        if(x >= temp)
        {
          sprintf(buf,"%d",calendar.day++);
          LISTVIEW_SetItemText(hItem,x,0,buf);
        }
        else
        {
          LISTVIEW_SetItemText(hItem,x,0," ");
        }
      }
      else
      {
        if(((calendar.month==2)&&(calendar.day<=28)) || 
           ((calendar.month%2==0)&&(calendar.day<=30)) || 
             (calendar.day<=31))
        {
          sprintf(buf,"%d",calendar.day++);
          LISTVIEW_SetItemText(hItem,x,y,buf);
        }
        else
        {
          LISTVIEW_SetItemText(hItem,x,y," ");
        }
      }
      
      if((date.year == calendar.year) && 
         (date.month == calendar.month) && 
         (date.day == calendar.day-1))
      {
        LISTVIEW_SetItemBkColor(hItem,x,y,LISTVIEW_CI_DISABLED,GUI_DARKBLUE);
      }
      else
      {
        LISTVIEW_SetItemBkColor(hItem,x,y,LISTVIEW_CI_DISABLED,GUI_WHITE);
      }
    }
  }
}
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
  const char Week[][7][10] = {
    {"Mon.","Tue.","Wed.","Thu.","Fri.","Sat.","Sun."},
    {"周一","周二","周三","周四","周五","周六","周日"},
  };
  char buf[32];
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Calender'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00C0C0C0));
    //
    // Initialization of '<<'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    //
    // Initialization of 'title'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, FontList16[System.font]);
    TEXT_SetText(hItem, "Calendar");
    //
    // Initialization of 'date'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, FontList16[System.font]);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "0000/00");
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
    LISTVIEW_SetGridVis(hItem, 1);
    LISTVIEW_SetHeaderHeight(hItem, 20);
    LISTVIEW_SetRowHeight(hItem, 22);
    LISTVIEW_SetFont(hItem, FontList16[System.font]);
    LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);
    for(short i=0;i<7;i++)
      LISTVIEW_AddColumn(hItem, 40, Week[0][i], GUI_TA_HCENTER | GUI_TA_VCENTER);
    for(short i=0;i<6;i++)
    {
      LISTVIEW_AddRow(hItem, NULL);
      LISTVIEW_DisableRow(hItem,i);
    }
    //
    // Initialization of '<<'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    //
    // Initialization of '>>'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    //
    // Initialization of '<'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    //
    // Initialization of '>'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetFont(hItem, FontList16[System.font]);
    // USER START (Optionally insert additional code for further widget initialization)
    Calendar = Calendar_Get();
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    sprintf(buf,"%04d/%02d",Calendar.year,Calendar.month);
    TEXT_SetText(hItem, buf);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
    DrawCalendar(hItem,Calendar);
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by '<<'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        WM_DeleteWindow(pMsg->hWin);
        CreateAppList();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_LISTVIEW_0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by '<<'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        if(Calendar.year > 1)
          Calendar.year--;
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        sprintf(buf,"%04d/%02d",Calendar.year,Calendar.month);
        TEXT_SetText(hItem, buf);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        DrawCalendar(hItem,Calendar);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by '>>'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        Calendar.year++;
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        sprintf(buf,"%04d/%02d",Calendar.year,Calendar.month);
        TEXT_SetText(hItem, buf);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        DrawCalendar(hItem,Calendar);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by '<'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        if(Calendar.month > 1)
          Calendar.month--;
        else if(Calendar.year > 1)
        {
          Calendar.month = 12;
          Calendar.year--;
        }
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        sprintf(buf,"%04d/%02d",Calendar.year,Calendar.month);
        TEXT_SetText(hItem, buf);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        DrawCalendar(hItem,Calendar);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_4: // Notifications sent by '>'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        if(Calendar.month < 12)
          Calendar.month++;
        else
        {
          Calendar.month = 1;
          Calendar.year++;
        }
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        sprintf(buf,"%04d/%02d",Calendar.year,Calendar.month);
        TEXT_SetText(hItem, buf);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        DrawCalendar(hItem,Calendar);
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
*       CreateCalendar
*/
WM_HWIN CreateCalendar(void);
WM_HWIN CreateCalendar(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/