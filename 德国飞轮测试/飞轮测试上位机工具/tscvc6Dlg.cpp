// tscvc6Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "tscvc6.h"
#include "tscvc6Dlg.h"
#include "uart.h"
#include "crc32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define UPDATE_START 1
#define UPDATE_SEND  2
#define UPDATE_DONE  3
#define UPDATE_ERR   4
#define UPDATE_OK    5

CWinThread*    Update_Thread=0;//声明线程
UINT StartUpdateThread(void *param);//声明线程函数
CWinThread*    UartRcv_Thread=0;//声明线程
UINT UartRcvThread(void *param);//声明线程函数

char SaveFileName[256]={0};
char CmdFileName[256]={0};
CFile SaveFile;      // CFile对象 
CStdioFile CmdFile;      // CFile对象 


#define CMD_MAXNUM 64
#define CMD_BUFFNUM 128
typedef enum
{
	CMD_LOOP,
	CMD_SEND,
	CMD_DELAY,
	CMD_RCV,
	CMD_END,
}CMD_e;

typedef struct
{
	UINT  loop;      /*循环次数*/
	UINT  cmdnum;    /*一条循环的个数*/
	UINT  cmdindex;  /*当前指令索引*/

	CMD_e cmd[CMD_MAXNUM];
	BYTE cmd_str[CMD_MAXNUM][CMD_BUFFNUM];  /*一个循环最多64条命令*/
	UINT cmd_val[CMD_MAXNUM];
}CMD_Ctrl_t;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTscvc6Dlg dialog

CTscvc6Dlg::CTscvc6Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTscvc6Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTscvc6Dlg)
	m_SetSpeed = 0.0f;
	m_SetCurrent = 0;
	m_NetTq = 0.0f;
	m_SetTms = 0;
	m_SetCanReset = 0;
	m_SrcAddr = 0;
	m_DesAddr = 0;
	m_RcvStr = _T("");
	m_ACmdCnt = 0;
	m_BCmdCnt = 0;
	m_AErrCnt = 0;
	m_BErrCnt = 0;
	m_ATxErrCnt = 0;
	m_ARxErrCnt = 0;
	m_BTxErrCnt = 0;
	m_BRxErrCnt = 0;
	m_ARstCnt = 0;
	m_BRstCnt = 0;
	m_Time = 0;
	m_MaxCurrent = 0;
	m_MaxTq = 0;
	m_PcbTemp = 0;
	m_MotorTemp = 0;
	m_Current = 0;
	m_SendStr = _T("");
	m_Version = _T("");
	m_Speed = 0.0f;
	m_Tq = 0.0f;
	m_StatusReg = _T("");
	m_CmdFileStr = _T("");
	m_SendPacketNum = 0;
	m_RcvPacketNum = 0;
	m_RcvErrPacketNum = 0;
	m_Power = 0;
	m_SetMaxTq = 0.0f;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTscvc6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTscvc6Dlg)
	DDX_Control(pDX, IDC_COMBO5, m_ComBoParity);
	DDX_Control(pDX, IDC_COMBO4, m_ComBoStopBit);
	DDX_Control(pDX, IDC_COMBO2, m_ComBoBaud);
	DDX_Control(pDX, IDC_COMBO3, m_ComBoData);
	DDX_Control(pDX, IDC_COMBO1, m_ComBoCom);
	DDX_Text(pDX, IDC_EDIT5, m_SetSpeed);
	DDX_Text(pDX, IDC_EDIT6, m_SetCurrent);
	DDX_Text(pDX, IDC_EDIT8, m_NetTq);
	DDX_Text(pDX, IDC_EDIT9, m_SetTms);
	DDX_Text(pDX, IDC_EDIT10, m_SetCanReset);
	DDX_Text(pDX, IDC_EDIT11, m_SrcAddr);
	DDX_Text(pDX, IDC_EDIT12, m_DesAddr);
	DDX_Text(pDX, IDC_EDIT1, m_RcvStr);
	DDX_Text(pDX, IDC_EDIT13, m_ACmdCnt);
	DDX_Text(pDX, IDC_EDIT14, m_BCmdCnt);
	DDX_Text(pDX, IDC_EDIT15, m_AErrCnt);
	DDX_Text(pDX, IDC_EDIT16, m_BErrCnt);
	DDX_Text(pDX, IDC_EDIT18, m_ATxErrCnt);
	DDX_Text(pDX, IDC_EDIT19, m_ARxErrCnt);
	DDX_Text(pDX, IDC_EDIT20, m_BTxErrCnt);
	DDX_Text(pDX, IDC_EDIT21, m_BRxErrCnt);
	DDX_Text(pDX, IDC_EDIT31, m_ARstCnt);
	DDX_Text(pDX, IDC_EDIT32, m_BRstCnt);
	DDX_Text(pDX, IDC_EDIT30, m_Time);
	DDX_Text(pDX, IDC_EDIT29, m_MaxCurrent);
	DDX_Text(pDX, IDC_EDIT28, m_MaxTq);
	DDX_Text(pDX, IDC_EDIT27, m_PcbTemp);
	DDX_Text(pDX, IDC_EDIT26, m_MotorTemp);
	DDX_Text(pDX, IDC_EDIT25, m_Current);
	DDX_Text(pDX, IDC_EDIT4, m_SendStr);
	DDX_Text(pDX, IDC_EDIT2, m_Version);
	DDX_Text(pDX, IDC_EDIT23, m_Speed);
	DDX_Text(pDX, IDC_EDIT24, m_Tq);
	DDX_Text(pDX, IDC_EDIT17, m_StatusReg);
	DDX_Text(pDX, IDC_EDIT7, m_CmdFileStr);
	DDX_Text(pDX, IDC_EDIT33, m_SendPacketNum);
	DDX_Text(pDX, IDC_EDIT34, m_RcvPacketNum);
	DDX_Text(pDX, IDC_EDIT35, m_RcvErrPacketNum);
	DDX_Text(pDX, IDC_EDIT22, m_Power);
	DDX_Text(pDX, IDC_EDIT3, m_SetMaxTq);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTscvc6Dlg, CDialog)
	//{{AFX_MSG_MAP(CTscvc6Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WN_SHOW_UPDATE,ShowUpdate)
	ON_MESSAGE(WN_SHOW_UARTRCV,ShowUartRcv)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTscvc6Dlg message handlers

BOOL CTscvc6Dlg::OnInitDialog()
{
		char strtmp[64];
		int i;
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	for(i=0;i<64;i++)
	{
	    _snprintf(strtmp,10,"COM%d",i);
		m_ComBoCom.AddString(strtmp);
	}
    m_ComBoCom.SetCurSel(0);

	m_ComBoBaud.AddString("115200");
	m_ComBoBaud.AddString("57600");
    m_ComBoBaud.AddString("38400");
	m_ComBoBaud.AddString("19200");
	m_ComBoBaud.AddString("9600");
    m_ComBoBaud.AddString("4800");
    m_ComBoBaud.SetCurSel(0);

	m_ComBoData.AddString("8");
    m_ComBoData.AddString("7");
    m_ComBoData.AddString("6");
    m_ComBoData.SetCurSel(0);

    m_ComBoStopBit.AddString("1");
    m_ComBoStopBit.AddString("1.5");
    m_ComBoStopBit.AddString("2");
    m_ComBoStopBit.SetCurSel(0);

    m_ComBoParity.AddString("无");
    m_ComBoParity.AddString("奇校验");
    m_ComBoParity.AddString("偶校验");
    m_ComBoParity.SetCurSel(0);

	//((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(FALSE);

	UpdateData(FALSE);  // 变量->控件
    uart_init();
	/*创建接收线程*/
#if 01
	UartRcv_Thread =  AfxBeginThread(UartRcvThread, 
                                     (LPVOID)this,
                                     THREAD_PRIORITY_BELOW_NORMAL,
                                     5*1024*1024,
                                     0,
                                     NULL);    
#endif
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTscvc6Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTscvc6Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTscvc6Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

static int ComOpenState = 0;
static int FileSaveState = 0;
static int FileCmdState = 0;
static char ComStr[64]={0};
static int ComBaud=0;
static int ComData=0;
static int ComStop=0;
static int ComParity=0;
static CString EditStr = _T("");
static BYTE gReadBuff[64]={0};
static char gReadStr[64]={0};
static char gWriteBuff[32]={(char)0xFF,(char)0x3C,(char)0x01,(char)0x00,(char)0x3D,(char)0x0D};
static char xPointStr[64];
static char yPointStr[64];
static DWORD gReadLen;
static char gPosition = 0;
static unsigned int gPrintNum =0;
static gRaiodSel = 1;
static int xOffset;
char DigToChar(unsigned char val)
{
	if((val>=0) && (val<=9))
	{
	    return (val+'0');
	}
	else if((val>9) && (val<=15))
	{
	    return (val-10 +'A');
	}
	else
	{
	    return ' ';
	}
}

BYTE CharToHex(char val)
{
	if((val>='0') && (val<='9'))
	{
	    return (val-'0');
	}
	else if((val>='A') && (val<='F'))
	{
	    return (val-'A' + 10);
	}
	else if((val>='a') && (val<='f'))
	{
	    return (val-'a' + 10);
	}
	else
	{
	    return 0;
	}
}

void CTscvc6Dlg::OnButton1() 
{
	BYTE buff[128];
	BYTE hexbuff[128];
    UINT len;
	UINT i;
    UINT hexlen=0;
	UpdateData(TRUE); //   控件 -> 变量
    BOOL writestat;
	memset(buff,0,sizeof(buff));
	memset(hexbuff,0,sizeof(hexbuff));
	//m_SendStr;
	if(((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
	    /*16进制发送*/
		strncpy((char*)buff,m_SendStr.GetBuffer(m_SendStr.GetLength()),sizeof(buff)-1);
        len = strlen((char*)buff);

		for(i=0;i<len-1;i++)
		{
			if(isxdigit(buff[i]) && isxdigit(buff[i+1]))
			{
				hexbuff[hexlen++] = (CharToHex(buff[i])<<4) + CharToHex(buff[i+1]);
				i++;
			}
			else
			{

			}
		}
		uart_send(hexbuff, hexlen, &writestat);

	}
	else
	{
	    /*ASCII发送*/	
		strncpy((char*)buff,m_SendStr.GetBuffer(m_SendStr.GetLength()),sizeof(buff)-1);
        len = strlen((char*)buff);
		uart_send(buff, len, &writestat);
	}
}




void CTscvc6Dlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if(0 == ComOpenState)
	{
		nIndex = m_ComBoCom.GetCurSel();
		if(nIndex>=10)
		{
			_snprintf(ComStr,20,"\\\\.\\COM%d",nIndex);
		}
		else
		{
			_snprintf(ComStr,20,"COM%d",nIndex);
		}
		if(uart_open(ComStr) != 0)
		{
			MessageBox("打开失败");
		}
		else
		{
            nIndex = m_ComBoBaud.GetCurSel();
            switch(nIndex)
			{
			case 0:
				ComBaud = 115200;
				break;
			case 1:
				ComBaud = 57600;
				break;
			case 2:
				ComBaud = 38400;
				break;
			case 3:
				ComBaud = 19200;
				break;
			case 4:
				ComBaud = 9600;
				break;
			case 5:
				ComBaud = 4800;
				break;
			default:
				ComBaud = 9600;
				break;
			}
            nIndex = m_ComBoData.GetCurSel();
            switch(nIndex)
			{
			case 0:
				ComData = 8;
				break;
			case 1:
				ComData = 7;
				break;
			case 2:
				ComData = 6;
				break;
			default:
				ComData = 8;
				break;
			}
            nIndex = m_ComBoParity.GetCurSel();
            switch(nIndex)
			{
			case 0:
				ComParity = NOPARITY;
				break;
			case 1:
				ComParity = ODDPARITY;
				break;
			case 2:
				ComParity = EVENPARITY;
				break;
			default:
				ComParity = NOPARITY;
				break;
			}
            nIndex = m_ComBoStopBit.GetCurSel();
            switch(nIndex)
			{
			case 0:
				ComStop = ONESTOPBIT;
				break;
			case 1:
				ComStop = ONE5STOPBITS;
				break;
			case 2:
				ComStop = TWOSTOPBITS;
				break;
			default:
				ComStop = ONESTOPBIT;
				break;
			}
			if(uart_config(ComBaud,ComData,ComParity,ComStop,100) != 0)
			{
				uart_close(ComStr);
				MessageBox("配置失败");
			}
			else
			{
				ComOpenState = 1;
				SetDlgItemText(IDC_BUTTON2,"关闭");
				((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
			}
		}
	}
	else
	{
		if(uart_close(ComStr) != 0)
		{
			MessageBox("关闭失败");
		}
		else
		{
			ComOpenState = 0;
			SetDlgItemText(IDC_BUTTON2,"打开");
			((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
		}
	}
}

void CTscvc6Dlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
    m_RcvStr = _T("");
   ((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowText(str);
}

BOOL CTscvc6Dlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}

void CTscvc6Dlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	BYTE buff[8]={0};
	BOOL writestat;

	buff[0] = 0x20;
	//buff[1] - buff[7] 
	uart_send(buff, 8, &writestat);
}

void CTscvc6Dlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	BYTE buff[8]={0};
	BOOL writestat;

	buff[0] = 0xD2;
	//buff[1] - buff[7] 
	uart_send(buff, 8, &writestat);
}

void CTscvc6Dlg::OnButton7() 
{
	BYTE buff[8]={0};
	unsigned int tq;
	BOOL writestat;
	// TODO: Add your control notification handler code here

	/*
	设置最大力矩  大端模式  21 xxxxxxxx dcdcdc
	21 3C F5 C2 8F dc dc dc
	*/
	UpdateData(TRUE); //控件 -> 变量
    //tq = htonl(m_MaxTq);
    memcpy(&tq,&m_SetMaxTq,4);

	buff[0] = 0x21;
	/*大端*/
	buff[1] = (tq >> 24) & 0x00FF;
	buff[2] = (tq >> 16) & 0x00FF;
	buff[3] = (tq >> 8) & 0x00FF;
	buff[4] = (tq >> 0) & 0x00FF;
	//buff[5] - buff[7] 
	uart_send(buff, 8, &writestat);
}

void CTscvc6Dlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
    ShellExecute(NULL,"open","help.docx",NULL,NULL, SW_SHOWNORMAL); 
}

void CTscvc6Dlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	BYTE buff[8]={0};
	short current;
	BOOL writestat;
	// TODO: Add your control notification handler code here

	/*
	设置电流  大端模式  23 xxxxdcdc dcdcdc
	23 FF 38 dc dc dc dc dc
	*/
	UpdateData(TRUE); //控件 -> 变量

	current = m_SetCurrent;
	buff[0] = 0x23;
	/*大端*/
	buff[1] = (current >> 8) & 0x00FF;
	buff[2] = (current >> 0) & 0x00FF;
	//buff[3] - buff[7] 
	uart_send(buff, 8, &writestat);
}

CString CTscvc6Dlg::SelectFile()
{
    CString strFile = _T("");

    CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.csv)|*.csv|All Files (*.*)|*.*||"), NULL);

    if (dlgFile.DoModal())
    {
        strFile = dlgFile.GetPathName();
    }

    return strFile;
}

void CTscvc6Dlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
	m_SendStr = str;
   ((CEdit *)GetDlgItem(IDC_EDIT4))->SetWindowText(str);
}

void CTscvc6Dlg::OnButton11() 
{
	m_CmdFileStr = this->SelectFile();
	strcpy(CmdFileName,m_CmdFileStr.GetBuffer(m_CmdFileStr.GetLength()));
	UpdateData(FALSE); // 变量 -> 控件
}

void CTscvc6Dlg::OnButton9() 
{
	if(FileCmdState==0)
    {
		FileCmdState = 1;
		SetDlgItemText(IDC_BUTTON9,"结束执行");

		Update_Thread =  AfxBeginThread(StartUpdateThread, 
                                      (LPVOID)this,
                                      THREAD_PRIORITY_BELOW_NORMAL,
                                      5*1024*1024,
                                      0,
                                      NULL); 
	}
	else
	{
		Update_Thread->PostThreadMessage(WN_EXIT_UPDATE,(WPARAM)0,0);
	    FileCmdState = 0;
		SetDlgItemText(IDC_BUTTON9,"开始执行");
	}
}

LRESULT CTscvc6Dlg::ShowUpdate(WPARAM wParam, LPARAM lParam)
{
	CString str("");
	CProgressCtrl* pProg = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
    pProg->SetRange(0, 100);
	switch(wParam)
	{
	case UPDATE_SEND:

		break;
	case UPDATE_ERR:
	    FileCmdState = 0;
		SetDlgItemText(IDC_BUTTON9,"开始执行");
		break;
	case UPDATE_OK:

		break;
	}
	return 0;
}



UINT StartUpdateThread(void *param)
{
    CTscvc6Dlg* dlg;
	CString lenstr;
	UINT cmdval;
	char cmdbuff[CMD_BUFFNUM];
	char lenbuff[CMD_BUFFNUM*2];
	UINT loopflag=0;

	BYTE buff[CMD_BUFFNUM];
	BYTE hexbuff[CMD_BUFFNUM];
    UINT len;
	UINT i;
    UINT hexlen=0;
    BOOL writestat;

	MSG msg;
    CMD_Ctrl_t cmd_ctrl;
	UINT state=0;
	dlg = (CTscvc6Dlg*)param;
    if(FileCmdState==1)
    {
		/*打开文件*/
		if(CmdFile.Open(CmdFileName, CFile::modeRead))
		{
			/*成功*/

		}
		else
		{
			/*失败*/
			AfxMessageBox("打开文件失败");
			dlg->PostMessage(WN_SHOW_UPDATE,UPDATE_ERR,0);
			Sleep(10);
			return 1;
		}
	}
	else
	{
		return 1;
	}
    memset(&cmd_ctrl,0,sizeof(cmd_ctrl));
	while(1)
	{
		Sleep(10);
		/*结束处理*/
		if(PeekMessage(&msg,NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
				case WN_EXIT_UPDATE:
					CmdFile.Close();
					return 0;
				break;
			}
		}


		if(state==0)
		{
		    /*加载LOOP*/
			do
			{
				if(CmdFile.ReadString(lenstr)==0)
				{
					CmdFile.Close();
					dlg->PostMessage(WN_SHOW_UPDATE,UPDATE_ERR,0);
					Sleep(10);
					return 1;
				}
				/*文件没有结束 且没有loop结束*/
                strcpy(lenbuff,lenstr.GetBuffer(lenstr.GetLength()));
				if(memcmp(lenbuff,"end",strlen("end"))==0)
				{
					loopflag = 1;
				}
				else if(memcmp(lenbuff,"send",strlen("send"))==0)
				{
					sscanf(lenbuff,"%[^','],%[^',']",cmdbuff,cmdbuff);
					cmd_ctrl.cmd[cmd_ctrl.cmdindex] = CMD_SEND;
					strcpy((char*)cmd_ctrl.cmd_str[cmd_ctrl.cmdindex],(char*)cmdbuff);
					cmd_ctrl.cmdnum++;
					cmd_ctrl.cmdindex++;
				}
				else if(memcmp(lenbuff,"loop",strlen("loop"))==0)
				{
					sscanf(lenbuff,"%[^','],%d",cmdbuff,&cmdval);
					cmd_ctrl.loop = cmdval;
					cmd_ctrl.cmdindex++;
				}
				else if(memcmp(lenbuff,"delay",strlen("delay"))==0)
				{
					sscanf(lenbuff,"%[^','],%d",cmdbuff,&cmdval);
					cmd_ctrl.cmd[cmd_ctrl.cmdindex] = CMD_DELAY;
					cmd_ctrl.cmd_val[cmd_ctrl.cmdindex] = cmdval;
					cmd_ctrl.cmdnum++;
					cmd_ctrl.cmdindex++;
				}
				else if(memcmp(lenbuff,"rcv",strlen("rcv"))==0)
				{
					sscanf(lenbuff,"%[^','],%d",cmdbuff,&cmdval);
					cmd_ctrl.cmd[cmd_ctrl.cmdindex] = CMD_RCV;
					cmd_ctrl.cmd_val[cmd_ctrl.cmdindex] = cmdval;
					cmd_ctrl.cmdnum++;
					cmd_ctrl.cmdindex++;
				}
				else
				{
					/*错误*/
				}
			}while(loopflag==0);
			if(loopflag==1)
			{
				loopflag=0;
				cmd_ctrl.cmdindex=0;
				state = 1;
			}
			else
			{
		        /*文件结束*/
				CmdFile.Close();
				dlg->PostMessage(WN_SHOW_UPDATE,UPDATE_ERR,0);
				Sleep(10);
				return 1;
			}
		}
		else
		{
			/*执行LOOP*/
			switch(cmd_ctrl.cmd[cmd_ctrl.cmdindex])
			{
			case CMD_SEND:
				/*16进制发送*/
				memset(buff,0,sizeof(buff));
				memset(hexbuff,0,sizeof(hexbuff));
				memcpy(buff,cmd_ctrl.cmd_str[cmd_ctrl.cmdindex],CMD_BUFFNUM);
				len = strlen((char*)buff);
				hexlen=0;
				for(i=0;i<len-1;i++)
				{
					if(isxdigit(buff[i]) && isxdigit(buff[i+1]))
					{
						hexbuff[hexlen++] = (CharToHex(buff[i])<<4) + CharToHex(buff[i+1]);
						i++;
					}
					else
					{

					}
				}
				uart_send(hexbuff, hexlen, &writestat);
				break;
			case CMD_DELAY:
				Sleep(cmd_ctrl.cmd_val[cmd_ctrl.cmdindex]);
				break;
			case CMD_RCV:
				break;
			default:
				break;
			}
			cmd_ctrl.cmdindex++;
			if(cmd_ctrl.cmdindex>=cmd_ctrl.cmdnum)
			{
				cmd_ctrl.loop--;
				cmd_ctrl.cmdindex=0;
				if(cmd_ctrl.loop <=0 )
				{
					state=0;
					memset(&cmd_ctrl,0,sizeof(cmd_ctrl));
				}
				else
				{
				    /*继续loop*/
				}
			}
		
		}
	}
	CmdFile.Close();
	dlg->PostMessage(WN_SHOW_UPDATE,UPDATE_ERR,0);
	Sleep(10);
	return 0;
}

void CTscvc6Dlg::OnChangeEdit3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CTscvc6Dlg::OnButton16() 
{
	// TODO: Add your control notification handler code here
	BYTE buff[8]={0};
	unsigned int speed;
	BOOL writestat;
	// TODO: Add your control notification handler code here

	/*
	设置速度  大端模式  22 xxxxxxxx dcdcdc
	22 C4 02 E6 52 dc dc dc
	*/
	UpdateData(TRUE); //控件 -> 变量
    //speed = htonl(m_SetSpeed);
	memcpy(&speed,&m_SetSpeed,4);
	buff[0] = 0x22;
	/*大端*/
	buff[1] = (speed >> 24) & 0x00FF;
	buff[2] = (speed >> 16) & 0x00FF;
	buff[3] = (speed >> 8) & 0x00FF;
	buff[4] = (speed >> 0) & 0x00FF;
	//buff[5] - buff[7] 
	uart_send(buff, 8, &writestat);
}


void CTscvc6Dlg::OnButton13() 
{
	// TODO: Add your control notification handler code here
	BYTE buff[8]={0};
	unsigned int nettq;
	BOOL writestat;

	/*
	设置net力矩  大端模式  24 xxxxxxxx dcdcdc
	24 3B A3 D7 0A dc dc dc
	*/
	UpdateData(TRUE); //控件 -> 变量
	memcpy(&nettq,&m_NetTq,4);
	buff[0] = 0x24;
	/*大端*/
	buff[1] = (nettq >> 24) & 0x00FF;
	buff[2] = (nettq >> 16) & 0x00FF;
	buff[3] = (nettq >> 8) & 0x00FF;
	buff[4] = (nettq >> 0) & 0x00FF;
	//buff[5] - buff[7] 
	uart_send(buff, 8, &writestat);
}

void CTscvc6Dlg::OnButton14() 
{
	BYTE buff[8]={0};
	BOOL writestat;
	UpdateData(TRUE); //控件 -> 变量

	buff[0] = 0xFE;
	if(m_SetTms == 0)
	{
	    buff[1] = 0;
	}
	else
	{
		buff[1] = 0xFF;
	}
	//buff[2] - buff[7] 
	uart_send(buff, 8, &writestat);	
}

void CTscvc6Dlg::OnButton15() 
{
	// TODO: Add your control notification handler code here
	BYTE buff[8]={0};
	BOOL writestat;
	UpdateData(TRUE); //控件 -> 变量

	buff[0] = 0x2C;
    buff[1] = m_SetCanReset;
	//buff[2] - buff[7] 
	uart_send(buff, 8, &writestat);	
}

BYTE buff_sum(void* buff,BYTE len)
{
  BYTE sum=0;
  BYTE i;
  BYTE* p;
  p = (BYTE*)buff;
  for(i=0;i<len;i++)
  {
      sum += p[i];
  }
  return sum;
}

void CTscvc6Dlg::OnButton17() 
{
	// TODO: Add your control notification handler code here
	BYTE buff[8]={0xAA,0x55,0x00,0x00,0x00,0x00,0x00,0x00};
	BOOL writestat;
	UpdateData(TRUE); //控件 -> 变量

	buff[2] = m_SrcAddr;
	buff[3] = m_DesAddr;
    buff[7] = buff_sum(buff,7);
	//buff[2] - buff[7] 
	uart_send(buff, sizeof(buff), &writestat);	
}


UINT UartRcvThread(void *param)
{

    CTscvc6Dlg* dlg;
	dlg = (CTscvc6Dlg*)param;
	BYTE inbuff[128];
    
	void* p;

	while(1)
	{
		BOOL readstat=0;
		DWORD readbytes=0;
		if(ComOpenState==0)
		{
			Sleep(1000);
		}
		else
		{
			//uart_flush();
	#if 1
			readbytes = uart_read(inbuff, sizeof(inbuff), &readstat);
			if((readbytes == 0) || (readstat != TRUE))
			{

			}

			else
			{
				p = malloc(readbytes);
				if(p)
				{
					memcpy(p,inbuff,readbytes);
					dlg->PostMessage(WN_SHOW_UARTRCV,readbytes,(long)p);
				}
			}
	#endif
		}
	}
	return 0;
}

LRESULT CTscvc6Dlg::ShowUartRcv(WPARAM wParam, LPARAM lParam)
{
	static unsigned int g_rcvnum = 0;
	char savefilebuff[256];
	CString str;
	BYTE* p=0;
	WPARAM i;
	UINT status;
	p = (BYTE*)lParam;
	UINT speed;
	UINT tq;
	if((wParam !=0) && (lParam !=0))
	{
		if(((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck())
		{
			for(i=0;i<wParam;i++)
			{
				str.Format("%02X ",p[i]);
				m_RcvStr += str;
				g_rcvnum++;
				if((g_rcvnum%8)==0)
				{
					m_RcvStr += "\r\n";
				}	
			}
		}
		else
		{
			str.Format("%s",p);
			m_RcvStr += str;
		}
        if(wParam == 40)
		{
		    UINT len;
			len = ((UINT)p[0]<<8) + p[1];
			if(len == 0x0025 )
			{
				if(buff_sum(p,39) == p[39])
				{
                    m_ACmdCnt = p[2];
					m_BCmdCnt = p[3];
					m_AErrCnt = p[4];
					m_BErrCnt = p[5];
					status = ((unsigned short)p[6]<<8) | p[7]; 
					m_StatusReg.Format("%x",status);

					m_ATxErrCnt = p[8];
					m_ARxErrCnt = p[9];
					m_BTxErrCnt = p[10];
					m_BRxErrCnt = p[11];
					m_ARstCnt = p[12];
					m_BRstCnt = p[13];

					m_Power = ((unsigned short)p[14]<<8) | p[15];
					speed = ((unsigned int)p[16]<<24) | ((unsigned int)p[17]<<16) | ((unsigned int)p[18]<<8) | ((unsigned int)p[19]<<0);
					memcpy(&m_Speed,&speed,4);
					tq = ((unsigned int)p[20]<<24) | ((unsigned int)p[21]<<16) | ((unsigned int)p[22]<<8) | ((unsigned int)p[23]<<0);
					memcpy(&m_Tq,&tq,4);
					m_Current = ((short)p[24]<<8) | ((short)p[25]<<0);
					m_MotorTemp = ((unsigned int)p[26]<<8) | ((unsigned int)p[27]<<0);

					m_PcbTemp = ((unsigned int)p[28]<<8) | ((unsigned int)p[29]<<0);
					m_MaxTq   = ((unsigned int)p[30]<<8) | ((unsigned int)p[31]<<0);
					m_MaxCurrent = ((unsigned int)p[32]<<8) | ((unsigned int)p[33]<<0);
					m_Time = ((unsigned int)p[34]<<24) | ((unsigned int)p[35]<<16) | ((unsigned int)p[36]<<8) | ((unsigned int)p[37]<<0);

					/*记录到文件*/
					memset(savefilebuff,0,sizeof(savefilebuff));
					sprintf(savefilebuff,"%d,%d,%d,%d,%2x,%d,%d,%d,%d,%d,%d,%d,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
						m_ACmdCnt,m_BCmdCnt,m_AErrCnt,m_BErrCnt,(unsigned short)status,
						m_ATxErrCnt,m_ARxErrCnt,m_BTxErrCnt,m_BRxErrCnt,m_ARstCnt,m_BRstCnt,
						(unsigned int)m_Power,m_Speed,m_Tq,(int)m_Current,(int)(0.4883*m_MotorTemp+273),
						(int)(-0.1842*m_PcbTemp+392.19+273),(int)m_MaxTq,(int)m_MaxCurrent,m_Time,m_SendPacketNum,m_RcvPacketNum,m_RcvErrPacketNum);
					if(FileSaveState==1)
					{
						SaveFile.Write(savefilebuff,strlen(savefilebuff));
					}
				}
			}
		
		}
		else if((wParam == 8) || (wParam == 14))
		{
			if((p[0] == 0xAA) && (p[1] == 0x66))
			{
				/*获取包计数*/
				m_SendPacketNum = ((UINT)p[2]<<24) | ((UINT)p[3]<<16) | ((UINT)p[4]<<8) | ((UINT)p[5]<<0);
				m_RcvPacketNum = ((UINT)p[6]<<24) | ((UINT)p[7]<<16) | ((UINT)p[8]<<8) | ((UINT)p[9]<<0);
				m_RcvErrPacketNum = ((UINT)p[10]<<24) | ((UINT)p[11]<<16) | ((UINT)p[12]<<8) | ((UINT)p[13]<<0);
			}
			else
			{
				UINT version = ((unsigned int)p[0]<<24) | ((unsigned int)p[1]<<16) | ((unsigned int)p[2]<<8) | ((unsigned int)p[3]<<0);
				m_Version.Format("%x",version);
			}
		}
		UpdateData(FALSE); // 变量 -> 控件
        free(p); 
	}
	return 0;
}

void CTscvc6Dlg::OnButton12() 
{
	CString filename;
    char headstr[] = "A命令计数,B命令计数,A错误计数,B错误计数,状态,A发送错误,A接收错误,B发送错误,B接收错误,A复位,B复位,功率,速度,力矩,电流,电机温度,PCB温度,最大力矩,最大电流,系统时间,发送包数,接收正确包数,接收错误包数\r\n";
	// TODO: Add your control notification handler code here
    if(FileSaveState==0)
    {
		filename = this->SelectFile();
		strcpy(SaveFileName,filename.GetBuffer(filename.GetLength()));
		/*打开文件*/
		if(SaveFile.Open(filename, CFile::modeReadWrite|CFile::modeCreate))
		{
			/*成功*/
			SaveFile.Write(headstr,strlen(headstr));
			FileSaveState = 1;
			SetDlgItemText(IDC_BUTTON12,"停止");

		}
		else
		{
			/*失败*/
			AfxMessageBox("打开文件失败");
		}
	}
	else
	{
	    FileSaveState = 0;
		SetDlgItemText(IDC_BUTTON12,"记录");
		/*关闭文件*/
		SaveFile.Close();
	}
}


/*遥测测试帧
00 25 01 02 03 04 AA 55 06 07 08 09 0A 0B 01 00 C4 02 E6 52 3C F5 C2 8F FF 01 01 00 02 00 03 00 04 00 11 22 33 44 00 96

00 25 01 02 03 04 AA 55 06 07 08 09 0A 0B 01 00
C4 02 E6 52 速度 -523.59878
3C F5 C2 8F 力矩 0.030
FF 01 01 00 02 00 03 00
11 22 33 44
00
xx 求和

20 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 

D2 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 

20 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 

D2 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 

20 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
D2 00 00 00 00 00 00 00 

*/



void CTscvc6Dlg::OnButton18() 
{
	BYTE buff[8]={0xAA,0x66,0x00,0x00,0x00,0x00,0x00,0x00};
	BOOL writestat;
    buff[7] = buff_sum(buff,7);
	uart_send(buff, sizeof(buff), &writestat);
}
