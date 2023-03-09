// InputDialog.cpp: 實作檔案
//

#include "pch.h"
#include "SIC-Simulator.h"
#include "afxdialogex.h"
#include "InputDialog.h"


// InputDialog 對話方塊

IMPLEMENT_DYNAMIC(InputDialog, CDialogEx)

InputDialog::InputDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT, pParent)
	, m_Input(_T(""))
{

}

InputDialog::~InputDialog()
{
}

void InputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_HANDLE, m_Input);
}


BEGIN_MESSAGE_MAP(InputDialog, CDialogEx)
END_MESSAGE_MAP()


// InputDialog 訊息處理常式
