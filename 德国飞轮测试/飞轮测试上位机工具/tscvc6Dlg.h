// tscvc6Dlg.h : header file
//

#if !defined(AFX_TSCVC6DLG_H__88F02647_53D9_4B3B_B272_46DDC9036174__INCLUDED_)
#define AFX_TSCVC6DLG_H__88F02647_53D9_4B3B_B272_46DDC9036174__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTscvc6Dlg dialog

#define WN_SHOW_UPDATE            WM_USER+1  //升级消息
#define WN_EXIT_UPDATE            WM_USER+2  //升级EXIT消息
#define WN_SHOW_UARTRCV           WM_USER+3  //串口接收

class CTscvc6Dlg : public CDialog
{
// Construction
public:
	CString SelectFile();
	CTscvc6Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTscvc6Dlg)
	enum { IDD = IDD_TSCVC6_DIALOG };
	CComboBox	m_ComBoParity;
	CComboBox	m_ComBoStopBit;
	CComboBox	m_ComBoBaud;
	CComboBox	m_ComBoData;
	CComboBox	m_ComBoCom;
	float	m_SetSpeed;
	int		m_SetCurrent;
	float	m_NetTq;
	UINT	m_SetTms;
	UINT	m_SetCanReset;
	UINT	m_SrcAddr;
	UINT	m_DesAddr;
	CString	m_RcvStr;
	BYTE	m_ACmdCnt;
	BYTE	m_BCmdCnt;
	BYTE	m_AErrCnt;
	BYTE	m_BErrCnt;
	BYTE	m_ATxErrCnt;
	BYTE	m_ARxErrCnt;
	BYTE	m_BTxErrCnt;
	BYTE	m_BRxErrCnt;
	BYTE	m_ARstCnt;
	BYTE	m_BRstCnt;
	UINT	m_Time;
	short	m_MaxCurrent;
	short	m_MaxTq;
	short	m_PcbTemp;
	short	m_MotorTemp;
	short	m_Current;
	CString	m_SendStr;
	CString	m_Version;
	float	m_Speed;
	float	m_Tq;
	CString	m_StatusReg;
	CString	m_CmdFileStr;
	UINT	m_SendPacketNum;
	UINT	m_RcvPacketNum;
	UINT	m_RcvErrPacketNum;
	UINT	m_Power;
	float	m_SetMaxTq;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTscvc6Dlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTscvc6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton10();
	afx_msg void OnButton4();
	afx_msg void OnButton11();
	afx_msg void OnButton9();
	afx_msg void OnChangeEdit3();
	afx_msg void OnButton16();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton17();
	afx_msg void OnButton12();
	afx_msg void OnButton18();
	//}}AFX_MSG
	afx_msg LRESULT ShowUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ShowUartRcv(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSCVC6DLG_H__88F02647_53D9_4B3B_B272_46DDC9036174__INCLUDED_)
