// OutputDialog.cpp: 實作檔案
//

#include "pch.h"
#include "SIC-Simulator.h"
#include "afxdialogex.h"
#include "OutputDialog.h"


// OutputDialog 對話方塊

IMPLEMENT_DYNAMIC(OutputDialog, CDialogEx)

OutputDialog::OutputDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OUTPUT, pParent)
	, m_Output(_T(""))
{

}

OutputDialog::~OutputDialog()
{
}

void OutputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OUTPUT_HANDLE, m_Output);
}


BEGIN_MESSAGE_MAP(OutputDialog, CDialogEx)
END_MESSAGE_MAP()


// OutputDialog 訊息處理常式
