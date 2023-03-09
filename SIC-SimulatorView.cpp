
// Project : SIC-Simulator MFC-GUI version
// Author: Da-Chih Lin and Xiao-Zhen Liu
// Institute : Dept. Computer Science and Informance Engineering, National Chi Nan University, Nantou, Taiwan
// E-mail : {s109321017, s109321008}@ncnu.edu.tw
// Ref : Leland L. Beck. System Software: An Introduction to Systems Programming. Addison-Wesley, 1997

// SIC-SimulatorView.cpp 
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "SIC-Simulator.h"
#endif

#include "SIC-SimulatorDoc.h"
#include "SIC-SimulatorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSICSimulatorView

IMPLEMENT_DYNCREATE(CSICSimulatorView, CFormView)

BEGIN_MESSAGE_MAP(CSICSimulatorView, CFormView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_IN, &CSICSimulatorView::OnBnClickedIn)
    ON_BN_CLICKED(IDC_STEP, &CSICSimulatorView::OnBnClickedStep)
//    ON_COMMAND(ID_I_INPUT, &CSICSimulatorView::OnIInput)
//    ON_COMMAND(ID_I_OUTPUT, &CSICSimulatorView::OnIOutput)
    ON_BN_CLICKED(IDC_FILE, &CSICSimulatorView::OnBnClickedFile)
    ON_BN_CLICKED(IDC_GO, &CSICSimulatorView::OnBnClickedGo)
END_MESSAGE_MAP()

// CSICSimulatorView 建構/解構

CSICSimulatorView::CSICSimulatorView() noexcept
	: CFormView(IDD_SICSIMULATOR_FORM)
{
    A = 0;
    X = 0;
    L = 0;
    PC = 0;
    SW = 0;
    first = 0;
    second = 0;
    current = 0;
    condition = 0;

    InputIndex = 0;
    OutputIndex = 0;
    return_addr = 0;
    goal = "";
    input = L"";
    output = L"";
}

CSICSimulatorView::~CSICSimulatorView()
{
}

void CSICSimulatorView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CSICSimulatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CFormView::PreCreateWindow(cs);
}

void CSICSimulatorView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

    optab["ADD"] = "18"; optab["ADDF"] = "58"; optab["ADDR"] = "90"; optab["AND"] = "40";
    optab["CLEAR"] = "B4"; optab["COMP"] = "28"; optab["COMPF"] = "88"; optab["COMPR"] = "A0";
    optab["DIV"] = "24"; optab["DIVF"] = "64"; optab["DIVR"] = "9C"; optab["FIX"] = "C4";
    optab["FLOAT"] = "C0"; optab["HIO"] = "F4"; optab["J"] = "3C"; optab["JEQ"] = "30";
    optab["JGT"] = "34"; optab["JLT"] = "38"; optab["JSUB"] = "48"; optab["LDA"] = "00";
    optab["LDB"] = "68"; optab["LDCH"] = "50"; optab["LDF"] = "70"; optab["LDL"] = "08";
    optab["LDS"] = "6C"; optab["LDT"] = "74"; optab["LDX"] = "04"; optab["LPS"] = "D0";
    optab["MUL"] = "20"; optab["MULF"] = "60"; optab["MULR"] = "98"; optab["NORM"] = "C8";
    optab["OR"] = "44"; optab["RD"] = "D8"; optab["RMO"] = "AC"; optab["RSUB"] = "4C";
    optab["SHIFTL"] = "A4"; optab["SHIFTR"] = "A8"; optab["SIO"] = "F0"; optab["SSK"] = "EC";
    optab["STA"] = "0C"; optab["STB"] = "78"; optab["STCH"] = "54"; optab["STF"] = "80";
    optab["STI"] = "D4"; optab["STL"] = "14"; optab["STS"] = "7C"; optab["STSW"] = "E8";
    optab["STT"] = "84"; optab["STX"] = "10"; optab["SUB"] = "1C"; optab["SUBF"] = "5C";
    optab["SUBR"] = "94"; optab["SVC"] = "B0"; optab["TD"] = "E0"; optab["TIO"] = "F8";
    optab["TIX"] = "2C"; optab["TIXR"] = "B8"; optab["WD"] = "DC";
}


// CSICSimulatorView 列印

BOOL CSICSimulatorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void CSICSimulatorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void CSICSimulatorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}

void CSICSimulatorView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此加入自訂的列印程式碼
}


// CSICSimulatorView 診斷

#ifdef _DEBUG
void CSICSimulatorView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSICSimulatorView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSICSimulatorDoc* CSICSimulatorView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSICSimulatorDoc)));
	return (CSICSimulatorDoc*)m_pDocument;
}
#endif //_DEBUG

vector<CString> Split(CString str, CString sep) {
    int nPos = 0;
    CString word;
    vector<CString> aVector;

    while (true) {
        word = str.Tokenize(sep, nPos);
        if (nPos == -1) break;
        aVector.push_back(word);
    }
    return aVector;
}

string CSICSimulatorView::GetLabel(string label) {
    label = label.substr(LABELSTA, LABELLEN);
    label = label.substr(0, label.find(' '));

    return label;
}

string CSICSimulatorView::GetOpcode(string opcode) {
    opcode = opcode.substr(CODESTA, CODELEN);
    opcode = opcode.substr(0, opcode.find(' '));

    return opcode;
}

string CSICSimulatorView::GetOper(string operand) {
    operand = operand.substr(OPERSTA, OPERLEN);
    operand = operand.substr(0, operand.find(' '));

    return operand;
}

void CSICSimulatorView::OnBnClickedIn()
{
	CFileDialog dialog(TRUE);
	CString filename;

	if (dialog.DoModal() == IDOK) {
		filename = dialog.GetPathName();
        vector<string> code, location;
        map<string, int*> integer;
        map<string, int> subroutine;

        Normalization(filename, code);
        CString out;
        for (int i = 0; i < code.size(); i++) {
            out += CA2W(code.at(i).c_str());
            out += L"\r\n";
            pointer.push_back(L"\r\n");
        }
        CFont aFont;
        aFont.CreatePointFont(130, _T("SimSun"));
        GetDlgItem(IDC_PROGRAM)->SetFont(&aFont, TRUE);
        GetDlgItem(IDC_PROGRAM)->SetWindowTextW(out);

        Pass1(integer, subroutine, location, code);
        out = "";
        for (int i = 0; i < location.size(); i++) {
            out += CA2W(location.at(i).c_str());
            out += L"\r\n";
        }
        GetDlgItem(IDC_LOC)->SetFont(&aFont, TRUE);
        GetDlgItem(IDC_LOC)->SetWindowTextW(out);

        //Conver to global variable.
        codes = code; 
        subroutines = subroutine;
        integers = integer;
        loc = location;

        Pass2(code);
	}
    
    for (int i = 1004; i < 1009; i++) {
        GetDlgItem(i)->SetWindowTextW(L"000000");
    }
}

void CSICSimulatorView::OnBnClickedStep()
{
    CString point;
    string opcode;

    // show programming counter
    GetDlgItem(IDC_PC)->SetWindowTextW(CA2W(loc[current].c_str()));

    pointer[current] = L"->\r\n";
    for (int i = 0; i < pointer.size(); i++) {
        point += pointer[i];
    }
    pointer[current] = L"\r\n";
    CFont aFont;
    aFont.CreatePointFont(130, _T("SimSun"));
    GetDlgItem(IDC_POINT)->SetFont(&aFont, TRUE);
    GetDlgItem(IDC_POINT)->SetWindowTextW(point);

    opcode = GetOpcode(codes[current]);

    if (opcode == "JSUB") {
        return_addr = current;

        // show return address
        GetDlgItem(IDC_L)->SetWindowTextW(CA2W(loc[current + 1].c_str()));

        current = subroutines[GetOper(codes[current])];
        current--;
    }
    else if (opcode == "RSUB") {
        current = return_addr;
        GetDlgItem(IDC_L)->SetWindowTextW(L"000000");
    }
    //conditional branch
    else if (opcode == "COMP") {
        first = FromReg();
        second = *integers[GetOper(codes[current])];

        // show conditional code
        if (first > second)
            GetDlgItem(IDC_SW)->SetWindowTextW(L"00003E");
        else if (first == second)
            GetDlgItem(IDC_SW)->SetWindowTextW(L"00003D");
        else
            GetDlgItem(IDC_SW)->SetWindowTextW(L"00003C");
    }
    else if (opcode == "TIX") {
        X++;
        first = X;
        second = *integers[GetOper(codes[current])];
        CString Cstr;
        Cstr.Format(L"%06x", X);
        GetDlgItem(IDC_X)->SetWindowTextW(Cstr);
    }
    else if (opcode == "JGT") {
        GetDlgItem(IDC_SW)->SetWindowTextW(L"000000");

        if (first > second)
            condition = 1;
    }
    else if (opcode == "JLT") {
        GetDlgItem(IDC_SW)->SetWindowTextW(L"000000");

        if (first < second)
            condition = 1;
    }
    else if (opcode == "JEQ") {
        GetDlgItem(IDC_SW)->SetWindowTextW(L"000000");

        if (first == second)
            condition = 1;
    }
    else if (opcode == "J") {
        condition = 1;
    }
    //Read or Write
    else if (opcode[0] == 'L' && opcode[1] == 'D') //If Load Register
        LoadReg();
    else if (opcode == "TD")
        current += 1;
    else if (opcode == "RD")
        LoadReg();
    else if (opcode == "WD")
        WriteReg();
    else if (opcode[0] == 'S' && opcode[1] == 'T')
        StoreReg();
    //Calculate
    else if (opcode == "ADD")
        AddReg();
    else if (opcode == "SUB")
        SubReg();
    else if (opcode == "MUL")
        MulReg();
    else if (opcode == "DIV")
        DivReg();


    if (condition) {
        current = subroutines[GetOper(codes[current])];
        condition = 0;
        return;
    }

    current++;

    GetDlgItem(IDC_OUTPUT)->SetWindowTextW(output);
}

void CSICSimulatorView::OnBnClickedFile()
{
    CFileDialog dialog(TRUE);
    CString filename;
    if (dialog.DoModal() == IDOK) {
        filename = dialog.GetPathName();
        ifstream file(filename);
        
        string in;
        while (getline(file, in)) {
            input += CA2W(in.c_str());
            input += L"\r\n";
        }

        GetDlgItem(IDC_INPUT)->SetWindowTextW(input);

        vector<CString> temp;
        temp = Split(input, L"\r\n");

        for (int i = 0; i < temp.size(); i++) {
            goal += CW2A(temp[i].GetString());
            goal.push_back(10);                     //EOL
        }
        goal.push_back(4);                          //EOF
    }
}

void CSICSimulatorView::OnBnClickedGo()
{
    CString temp;
    GetDlgItem(IDC_ADDR)->GetWindowTextW(temp);

    string addr = CW2A(temp.GetString());

    Do(addr);
}
