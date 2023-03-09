#pragma once
#include "afxdialogex.h"


// OutputDialog 對話方塊

class OutputDialog : public CDialogEx
{
	DECLARE_DYNAMIC(OutputDialog)

public:
	OutputDialog(CWnd* pParent = nullptr);   // 標準建構函式
	virtual ~OutputDialog();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OUTPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	CString m_Output;
};
