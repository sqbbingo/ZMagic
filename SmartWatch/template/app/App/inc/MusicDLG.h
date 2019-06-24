#ifndef _musicdlg_h_
#define _musicdlg_h_

#include "DIALOG.h"
#include "App.h"
#include "View.h"
#include "fml_audio/bsp_wm8978.h"  


#define MUSIC_MAX_NUM           20
#define FILE_NAME_LEN           200			
#define MUSIC_NAME_LEN          50

#define BUFFER_SIZE             (1024*16)
#define MP3BUFFER_SIZE          2304		
#define INPUTBUF_SIZE           3000	

#define LYRIC_MAX_SIZE          200				
#define MAX_LINE_LEN            200	

#define WM_USERMSG_PLAYSTART    (WM_USER+1)
#define WM_USERMSG_PLAYOVER     (WM_USER+2)
#define WM_USERMSG_KEYEVENT     (WM_USER+3)


/* WAV�ļ�ͷ��ʽ */
typedef __packed struct
{ 
	uint32_t	riff;							/* = "RIFF"	0x46464952*///4
	uint32_t	size_8;						/* ���¸���ַ��ʼ���ļ�β�����ֽ���	*///4
	uint32_t	wave;							/* = "WAVE" 0x45564157*///4
	
	uint32_t	fmt;							/* = "fmt " 0x20746d66*///4
	uint32_t	fmtSize;					/* ��һ���ṹ��Ĵ�С(һ��Ϊ16) *///4	 
	uint16_t	wFormatTag;				/* ���뷽ʽ,һ��Ϊ1	*///2
	uint16_t	wChannels;				/* ͨ������������Ϊ1��������Ϊ2 *///2
	uint32_t	dwSamplesPerSec;	/* ������ *///4
	uint32_t	dwAvgBytesPerSec;	/* ÿ���ֽ���(= ������ �� ÿ���������ֽ���) *///4
	uint16_t	wBlockAlign;			/* ÿ���������ֽ���(=����������/8*ͨ����) *///2
	uint16_t	wBitsPerSample;		/* ����������(ÿ��������Ҫ��bit��) *///2
																			   
	uint32_t	data;							/* = "data" 0x61746164*///4
	uint32_t	datasize;					/* �����ݳ��� *///4
} WavHead;


typedef struct
{
	uint8_t ucVolume;			      /* ��ǰ�������� */
  uint8_t ucMute;             /* ������־ 1��������0������  */
  uint8_t ucSpk;              /* ���״̬ */
	uint32_t ucFreq;			      /* ����Ƶ�� */
  uint32_t ucbps;            /* ������ ��ÿ�봫�Ͷ��ٸ�λ */
}MusicDevice_t;

/* ����״̬ */
typedef enum
{
	STA_IDLE = 0,	/* ����״̬ */
	STA_PLAYING,	/* ����״̬ */
	STA_PAUSE,	  /* ��ͣ״̬ */
  STA_SWITCH,   /* �и�״̬ */
}MusicStatus_t;

typedef struct
{
  FIL fs_file;
  UINT fs_num;
  FRESULT fs_result;
  char pathList[MUSIC_MAX_NUM][FILE_NAME_LEN];
  char nameList[MUSIC_MAX_NUM][MUSIC_NAME_LEN];
  uint8_t num;
}MusicFile_t;

typedef struct
{
  uint32_t allTime;
  uint32_t currTime;
  uint16_t index;
  uint8_t file[FILE_NAME_LEN];
  uint8_t type;
  uint16_t flags;
  uint8_t IsReady;
  uint8_t bufFlag;
  uint16_t dataBuffer[2][BUFFER_SIZE];
}MusicPlay_t;


typedef struct
{
	uint16_t frameSize;
	uint32_t ID3V2Size;
  uint32_t frameTime;
  int32_t readOffset;
  int32_t readSurplus;
  uint8_t data[INPUTBUF_SIZE];
  uint8_t* readPtr;
  //HMP3Decoder Mp3Decoder;/* mp3������ָ��	*/
  //MP3FrameInfo Mp3FrameInfo;/* mP3֡��Ϣ  */
}Mp3Play_t;

typedef struct
{
  MusicDevice_t device;
  MusicFile_t file;
  WM_HWIN hWin;
  MusicPlay_t play;
  //Mp3Play_t mp3;
}AppMusic_t;


extern AppMusic_t* Music;
extern MusicStatus_t MusicStatus;
PROCESS_NAME(MusicProcess);

#endif