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

string FillSpace(string str, int size) {
    while (str.length() != size) {
        str.push_back(' ');
    }
    return str;
}

// The normalized format, characters transformed to uppercase
// + ---------------------------------------- +
// | 1         2         3         |
// | 1234567890123456789012345678901234567  |
// | ======== ======  ==================    |
// |   1 - 8    10 - 15     18 - |
// |  label   opcode    operand |
// +---------------------------------------- +
void CSICSimulatorView::Normalization(CString filename, vector<string>& code) {
    infile.open(filename);

    string input;
    string label, opcode, operand;

    while (getline(infile, input)) {
        string temp, All = "";

        bool trigger = false;

        if (input.at(0) == '.')
            continue;

        // arrange label field
        label = input.substr(LABELSTA, LABELLEN);
        string Aout;
        for (size_t i = 0; i < label.length(); i++) {
            if (label.at(i) != ' ')
                Aout.push_back(toupper(label.at(i)));
        }
        All += FillSpace(Aout, 9);  // 8 + 1

        // arrange opcode field
        opcode = input.substr(CODESTA, CODELEN);
        string Bout;
        for (size_t i = 0; i < opcode.length(); i++) {
            if (opcode.at(i) != ' ')
                Bout.push_back(toupper(opcode.at(i)));
        }
        if (Bout.compare("RSUB") == false) {
            // printf("RSUB\n");
            All += "RSUB";
            code.push_back(All);
            continue;
        }
        else {
            All += FillSpace(Bout, 8);  // 6 + 2
        }

        // arrange operand field
        operand = input.substr(OPERSTA, OPERLEN);
        string Cout;
        for (size_t i = 0; i < operand.length() - 1; i++) {
            if (operand.at(i) != ' ' && trigger == false)
                Cout.push_back(toupper(operand.at(i)));
            else if (trigger == true)
                Cout.push_back(operand.at(i));

            if (operand.at(i + 1) == 39 && (operand.at(i) == 'c' || operand.at(i) == 'C')) // output characters
                trigger = true;
        }
        if (operand.at(operand.length() - 1) != ' ')
            Cout.push_back(toupper(operand.at(operand.length() - 1)));

        All += Cout;
        code.push_back(All);
    }

    infile.close();
}

void CSICSimulatorView::Pass1(map<string, int*>& integer, map<string, int>& subroutine, vector<string>& location, vector<string> code) {
    string input, label, opcode, operand;
    int loc = 0, start = 0;
    vector<string> symbol;
    vector<int> address;

    for (int i = 0; i < code.size(); i++) {
        // get opcode
        stringstream aa;
        input = code.at(i);

        if (GetLabel(input).length() > 0) {
            symtab[GetLabel(input)] = i;
        }

        opcode = GetOpcode(input);

        // get starting address
        if (opcode.compare("START") == false) {
            unsigned int x;
            stringstream ss;
            ss << hex << GetOper(input);
            ss >> x;
            loc = start = static_cast<int>(x);
            symbol.push_back(GetLabel(input));
            address.push_back(start);

            aa << hex << uppercase << setw(6) << setfill('0') << loc << ' ';
            location.push_back(aa.str());
            aa.clear();
            continue;
        }

        // store location
        aa << hex << uppercase << setw(6) << setfill('0') << loc << ' ';
        location.push_back(aa.str());
        aa.clear();

        // add location counter
        if (opcode.compare("RESW") == false) {
            loc += (3 * stoi(GetOper(input)));
            string temp = GetLabel(input);
            int* a = new int[stoi(GetOper(input))];
            integer[temp] = a;
        }
        else if (opcode.compare("RESB") == false) {
            loc += stoi(GetOper(input));
            string temp = GetLabel(input);
            int* a = new int[stoi(GetOper(input)) / sizeof(int)];
            integer[temp] = a;
        }
        else if (opcode.compare("BYTE") == false) {
            string op = GetOper(input);

            if (op.at(0) == 'C') {
                loc += op.length() - 3;
                string temp = GetLabel(input);
                temp = string(temp.begin() + 2, temp.end() - 1);
                int* a = new int[temp.length()];
                for (int i = 0; i < temp.length(); i++) {
                    *(a + i) = temp[i];
                }
                integer[temp] = a;
            }
            else
                loc += (op.length() - 3) / 2;
        }
        else {
            loc += 3;
            if (GetLabel(input).length() > 0) {
                if (opcode != "WORD") {
                    string temp = GetLabel(input);
                    subroutine[temp] = i;
                }
                else {
                    string temp = GetLabel(input);
                    int* a = new int;
                    *a = stoi(GetOper(input));
                    integer[temp] = a;
                }
            }
        }
    }
    loc -= 3;
}

void CSICSimulatorView::Pass2(vector<string> code) {
    string object = "";
    bool after = false;
    for (int i = 0; i < code.size(); i++) {
        string input = code.at(i);

        string opcode = GetOpcode(input);

        if (opcode == "WORD") {
            stringstream ss;
            ss << hex << uppercase << setw(6) << setfill('0') << right << stoi(GetOper(input));
            object += ss.str();
        }
        else if (opcode == "BYTE") {
            string oper = GetOper(input), content = string(oper.begin() + 2, oper.end() - 1);
            if (oper.at(0) == 'X') {
                object += content;
            }
            else
            {
                if (after) {
                    object += content;
                }
                else {
                    string temp = "";
                    for (int j = 0; j < content.size(); j++) {
                        int num = content.at(j);
                        stringstream ss;
                        ss << hex << uppercase << num;
                        temp += ss.str();
                    }
                    object += temp;
                }
            }
        }
        else if (opcode == "START" || opcode == "RESB" || opcode == "RESW" || opcode == "END") {
            // do nothing
        }
        else if (opcode=="RSUB") {
            object += "4C0000";
            after = true;
        }
        else {
            object += (optab[opcode] + string(loc.at(symtab[GetOper(input)]).begin() + 2, loc.at(symtab[GetOper(input)]).end()));
        }
        
        object += "\r\n";
    }
    CFont aFont;
    aFont.CreatePointFont(130, _T("SimSun"));
    GetDlgItem(IDC_OBJ)->SetFont(&aFont, TRUE);
    GetDlgItem(IDC_OBJ)->SetWindowTextW(CA2W(object.c_str()));
}

int CSICSimulatorView::FromReg() {
    if (reg == "A") {
        return A;
    }
    else if (reg == "X") {
        return X;
    }
    else if (reg == "L") {
        return L;
    }
    else if (reg == "PC") {
        return PC;
    }
    else if (reg == "SW") {
        return SW;
    }
}

void CSICSimulatorView::LoadReg() {
    string opcode = GetOpcode(codes[current]);
    CString Cstr;
    if (opcode != "RD" && opcode != "LDCH") {
        int num = *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", num);
        Cstr.MakeUpper();

        if (opcode == "LDA") {
            GetDlgItem(IDC_A)->SetWindowTextW(Cstr);
            A = num;
            reg = "A";
        }
        else if (opcode == "LDX") {
            GetDlgItem(IDC_X)->SetWindowTextW(Cstr);
            X = num;
            reg = "X";
        }
        else if (opcode == "LDL") {
            GetDlgItem(IDC_L)->SetWindowTextW(Cstr);
            L = num;
            reg = "L";
        }
        else if (opcode == "LDPC") {
            GetDlgItem(IDC_PC)->SetWindowTextW(Cstr);
            PC = num;
            reg = "PC";
        }
        else if (opcode == "LDSW") {
            GetDlgItem(IDC_SW)->SetWindowTextW(Cstr);
            SW = num;
            reg = "SW";
        }
    }
    else if (opcode == "LDCH") {
        string str = GetOper(codes[current]);
        if (str[str.size() - 1] == 'X') {
            str.erase(str.end() - 2, str.end());
            A = Buffer[OutputIndex];
            OutputIndex++;
        }
    }
    else {
        A = goal[InputIndex];
        Cstr.Format(L"%06x", A);
        Cstr.MakeUpper();
        GetDlgItem(IDC_A)->SetWindowTextW(Cstr);
        reg = "A";
        InputIndex++;
    }
}

void CSICSimulatorView::WriteReg() {
    CString s;
    if (A != 4 && A != 10)
        s.Format(L"%c", A);
    else if (A == 10)
        s.Format(L"\r\n");
    output += s;
}

void CSICSimulatorView::StoreReg() {
    string opcode = GetOpcode(codes[current]);

    if (opcode == "STA") {
        *integers[GetOper(codes[current])] = A;
    }
    else if (opcode == "STX") {
        *integers[GetOper(codes[current])] = X;
    }
    else if (opcode == "STL") {
        *integers[GetOper(codes[current])] = L;
    }
    else if (opcode == "STPC") {
        *integers[GetOper(codes[current])] = PC;
    }
    else if (opcode == "STSW") {
        *integers[GetOper(codes[current])] = SW;
    }
    else if (opcode == "STCH") {
        string str = GetOper(codes[current]);
        if (str[str.size() - 1] == 'X') {
            str.erase(str.end() - 2, str.end());
        }
        *integers[str] = A;
        Buffer.push_back(*integers[str]);
    }
}

void CSICSimulatorView::AddReg() {
    CString Cstr;
    if (reg == "A") {
        A += *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", A);
        Cstr.MakeUpper();
        GetDlgItem(IDC_A)->SetWindowTextW(Cstr);
    }
    else if (reg == "X") {
        X += *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", X);
        Cstr.MakeUpper();
        GetDlgItem(IDC_X)->SetWindowTextW(Cstr);
    }
    else if (reg == "L") {
        L += *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", L);
        Cstr.MakeUpper();
        GetDlgItem(IDC_L)->SetWindowTextW(Cstr);
    }
    else if (reg == "PC") {
        PC += *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", PC);
        Cstr.MakeUpper();
        GetDlgItem(IDC_PC)->SetWindowTextW(Cstr);
    }
    else if (reg == "SW") {
        SW += *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", SW);
        Cstr.MakeUpper();
        GetDlgItem(IDC_SW)->SetWindowTextW(Cstr);
    }
}

void CSICSimulatorView::SubReg() {
    CString Cstr;
    if (reg == "A") {
        A -= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", A);
        Cstr.MakeUpper();
        GetDlgItem(IDC_A)->SetWindowTextW(Cstr);
    }
    else if (reg == "X") {
        X -= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", X);
        Cstr.MakeUpper();
        GetDlgItem(IDC_X)->SetWindowTextW(Cstr);
    }
    else if (reg == "L") {
        L -= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", L);
        Cstr.MakeUpper();
        GetDlgItem(IDC_L)->SetWindowTextW(Cstr);
    }
    else if (reg == "PC") {
        PC -= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", PC);
        Cstr.MakeUpper();
        GetDlgItem(IDC_PC)->SetWindowTextW(Cstr);
    }
    else if (reg == "SW") {
        SW -= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", SW);
        Cstr.MakeUpper();
        GetDlgItem(IDC_SW)->SetWindowTextW(Cstr);
    }
}

void CSICSimulatorView::MulReg() {
    CString Cstr;
    if (reg == "A") {
        A *= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", A);
        Cstr.MakeUpper();
        GetDlgItem(IDC_A)->SetWindowTextW(Cstr);
    }
    else if (reg == "X") {
        X *= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", X);
        Cstr.MakeUpper();
        GetDlgItem(IDC_X)->SetWindowTextW(Cstr);
    }
    else if (reg == "L") {
        L *= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", L);
        Cstr.MakeUpper();
        GetDlgItem(IDC_L)->SetWindowTextW(Cstr);
    }
    else if (reg == "PC") {
        PC *= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", PC);
        Cstr.MakeUpper();
        GetDlgItem(IDC_PC)->SetWindowTextW(Cstr);
    }
    else if (reg == "SW") {
        SW *= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", SW);
        Cstr.MakeUpper();
        GetDlgItem(IDC_SW)->SetWindowTextW(Cstr);
    }
}

void CSICSimulatorView::DivReg() {
    CString Cstr;
    if (reg == "A") {
        A /= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", A);
        Cstr.MakeUpper();
        GetDlgItem(IDC_A)->SetWindowTextW(Cstr);
    }
    else if (reg == "X") {
        X /= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", X);
        Cstr.MakeUpper();
        GetDlgItem(IDC_X)->SetWindowTextW(Cstr);
    }
    else if (reg == "L") {
        L /= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", L);
        Cstr.MakeUpper();
        GetDlgItem(IDC_L)->SetWindowTextW(Cstr);
    }
    else if (reg == "PC") {
        PC /= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", PC);
        Cstr.MakeUpper();
        GetDlgItem(IDC_PC)->SetWindowTextW(Cstr);
    }
    else if (reg == "SW") {
        SW /= *integers[GetOper(codes[current])];
        Cstr.Format(L"%06x", SW);
        Cstr.MakeUpper();
        GetDlgItem(IDC_SW)->SetWindowTextW(Cstr);
    }
}

void CSICSimulatorView::Do(string addr) {
    while (1) {
        string temp = loc.at(current);
        OnBnClickedStep();
        if (temp == addr)
            break;
    }
    // for (int i = 0; i < loc.size(); i++)
}