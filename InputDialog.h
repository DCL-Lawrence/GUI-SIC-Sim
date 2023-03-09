#pragma once
#include "afxdialogex.h"


// InputDialog 對話方塊

class InputDialog : public CDialogEx
{
	DECLARE_DYNAMIC(InputDialog)

public:
	InputDialog(CWnd* pParent = nullptr);   // 標準建構函式
	virtual ~InputDialog();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	CString m_Input;
};
