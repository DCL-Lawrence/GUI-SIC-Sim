
// SIC-SimulatorView.h: CSICSimulatorView 類別的介面
//

#pragma once
#define LABELSTA 0
#define LABELLEN 8
#define CODESTA 9
#define CODELEN 6
#define OPERSTA 17
#define OPERLEN 18

#include <fstream>
using std::ifstream;

#include <string>
using std::string;
using std::getline;

#include <cctype>

#include <vector>
using std::vector;

#include <sstream>
using std::stringstream;

#include <iomanip>
using std::setfill;
using std::setw;
using std::hex;
using std::uppercase;
using std::right;

#include <map>
using std::map;

class CSICSimulatorView : public CFormView
{
protected: // 僅從序列化建立
	CSICSimulatorView() noexcept;
	DECLARE_DYNCREATE(CSICSimulatorView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_SICSIMULATOR_FORM };
#endif

// 屬性
public:
	CSICSimulatorDoc* GetDocument() const;

// 作業
public:

// 覆寫
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual void OnInitialUpdate(); // 建構後第一次呼叫
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CSICSimulatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
public:
	// Assembly code file processing
	afx_msg void OnBnClickedIn();
	string GetLabel(string label);
	string GetOpcode(string opcode);
	string GetOper(string operand);
	void Normalization(CString filename, vector<string>& code);
	void Pass1(map<string, int*>& integer, map<string, int>& subroutine, vector<string>& location, vector<string> code);
	void Pass2(vector<string> code);
	ifstream infile;

	// Implement
	afx_msg void OnBnClickedStep();
	void LoadReg();
	void WriteReg();
	void StoreReg();
	int FromReg();

	void AddReg();
	void SubReg();
	void MulReg();
	void DivReg();

	// registers
	int A, X, L, PC, SW;
	// The nearest register
	string reg;
	unsigned int current;
	// For comparison
	int first, second;
	bool condition;
	
	// tables
	vector<string> codes;			
	map<string, int> subroutines;	//section, section index
	map<string, int*> integers;		//variable, value 
	map<string, int> symtab;
	map<string, string> optab;

	// location
	vector<string> loc;
	afx_msg void OnBnClickedGo();
	void Do(string addr);

	// index pointer
	vector<CString> pointer;

	// return address.
	int return_addr;

	// Input and Output
	CString input, output;
	string goal;
	int InputIndex;
	int OutputIndex;
	vector<int>Buffer;
	afx_msg void OnBnClickedFile();

};

#ifndef _DEBUG  // 對 SIC-SimulatorView.cpp 中的版本進行偵錯
inline CSICSimulatorDoc* CSICSimulatorView::GetDocument() const
   { return reinterpret_cast<CSICSimulatorDoc*>(m_pDocument); }
#endif

