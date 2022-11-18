#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cctype>
#include <vector>
using namespace std;
map<string, int> label; //存放label位置的地方

//        _____________________________________________________
//R類型  | funct5 | funct2 | rs2 | rs1 | funct3 | rd | opcdode |
//       |_____________________________________________________| 
void R_type(string s, string rs[]) {
	map<string, string> funct5; //找出各個code 對應的funct5
	funct5["add"] = funct5["slli"] = funct5["srli"] = funct5["sll"] =
		funct5["slt"] = funct5["sltu"] = funct5["xor"] = funct5["srl"] =
		funct5["or"] = funct5["and"] = "00000";
	funct5["srai"] = funct5["sub"] = funct5["sra"] = "01000";

	map<string, string> funct3; //找出各個code 對應的funct3
	funct3["add"] = funct3["sub"] = "000"; funct3["slt"] = "010";
	funct3["slli"] = funct3["sll"] = "001"; funct3["sltu"] = "011";
	funct3["xor"] = "100"; funct3["or"] = "110"; funct3["and"] = "111";
	funct3["srli"] = funct3["srai"] = funct3["srl"] = funct3["sra"] = "101";

	string funct2 = "00"; //funct2都為00

	string opcode; //找出各個code 對應的opcode
	if (s == "slli" || s == "srli" || s == "srai") opcode = "0010011";
	else opcode = "0110011";

	string BinaryOutPut = "";
	BinaryOutPut = funct5[s] + funct2 + rs[2] + rs[1] + funct3[s] + rs[0] + opcode;

	cout << BinaryOutPut << endl;
}
//        ___________________________________________
//I類型  | imm[ 11:0 ] | rs1 | funct3 | rd | opcdode |
//       |___________________________________________| 
void I_type(string s, string rs[]) {
	map<string, string> funct3; //找出各個code 對應的funct3
	funct3["jalr"] = funct3["lb"] = funct3["addi"] = "000";
	funct3["lh"] = "001"; funct3["lw"] = funct3["slti"] = "010";
	funct3["sltiu"] = "011"; funct3["lbu"] = funct3["xori"] = "100";
	funct3["lhu"] = "101"; funct3["ori"] = "110"; funct3["andi"] = "111";

	string opcode;  //找出各個code 對應的opcode
	if (s == "jarl")
		opcode = "1100111";
	else if (s == "lb" || s == "lh" || s == "lw" || s == "lbu" || s == "lhu")
		opcode = "0000011";
	else
		opcode = "0010011";

	string BinaryOutPut = "";
	BinaryOutPut = rs[2] + rs[1] + funct3[s] + rs[0] + opcode;
	if (BinaryOutPut.size() > 32) { //如果offset過大
		cout << s << "的offset過大\n";
		return;
	}
	cout << BinaryOutPut << endl;
}
//        _________________________________________________________
//S類型  | imm[ 11:5 ] | rs2 | rs1 | funct3 | imm[ 4:0 ] | opcdode |
//       |_________________________________________________________| 
void S_type(string s, string rs[]) {
	map<string, string> funct3; //找出各個code 對應的funct3
	funct3["sb"] = "000"; funct3["sw"] = "010"; funct3["sh"] = "001";

	string opcode = "0100011";

	string BinaryOutPut = "";
	for (int i = 11; i >= 5; i--)
		BinaryOutPut += rs[1][i];
	BinaryOutPut += rs[0] + rs[2] + funct3[s];
	for (int i = 4; i >= 0; i--)
		BinaryOutPut += rs[1][i];
	BinaryOutPut += opcode;
	if (rs[1].size() > 12) { //如果offset超過所能讀取的bit
		cout << s << "的offset過大\n";
		return;
	}
	cout << BinaryOutPut << endl;
}
//        ___________________________________________________________________
//SB類型 | imm[ 12 | 10:5 ] | rs2 | rs1 | funct3 | imm[ 4:1 | 11 ] | opcdode |
//       |___________________________________________________________________| 
void SB_type(string s, string rs[]) {
	string opcode = "1100011";
	map<string, string> funct3; //找出各個code 對應的funct3
	funct3["beq"] = "000"; funct3["ble"] = "100"; funct3["bne"] = "001";
	funct3["bge"] = "101"; funct3["bltu"] = "110"; funct3["bgeu"] = "111";

	string BinaryOutPut = "";
	BinaryOutPut += rs[2][12];
	for (int i = 10; i > 4; i--)
		BinaryOutPut += rs[2][i];
	BinaryOutPut += rs[1] + rs[0] + funct3[s];
	for (int i = 4; i > 0; i--)
		BinaryOutPut += rs[2][i];
	BinaryOutPut += rs[2][11] + opcode;

	cout << BinaryOutPut << endl;
}
//        ______________________________________________
//UJ類型 | imm[ 20 | 10:1 | 11 | 19:12 ] | rd | opcdode |
//       |______________________________________________| 	
void UJ_type(string s, string rs[]) { //rs[0] 放 rd 的 binary number rs[1]放immediate
	string BinaryOutPut = "";
	BinaryOutPut += rs[1][20];
	for (int i = 10; i > 0; i--)
		BinaryOutPut += rs[1][i];
	BinaryOutPut += rs[1][11];
	for (int i = 19; i >= 12; i--)
		BinaryOutPut += rs[1][i];
	BinaryOutPut += rs[0] + "1101111"; //1101111是opcode

	cout << BinaryOutPut << endl;
}

string DecimalToBinary(int a) { //十進位轉成二進位用
	string s;
	while (a != 0) { //轉成二進位的string形式
		s.insert(s.begin(), (a % 2 + '0'));
		a /= 2;
	}
	while (s.size() < 5) //如果沒達到5bit 要補成5bit
		s.insert(s.begin(), '0');

	return s;
}

void upperToLower(string& s) { //大寫轉成小寫
	for (int i = 0; i < s.size(); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += 'a' - 'A';
	}
}

string offsetToBinary(int k, string type) { //傳進一個數字跟他的類型
	if (k < 0) { //如果是負數
		int z;
		if (type == "I" || type == "S") //如果是 I 或 S 代表這個 要換成補數 要加上4096 (12bit個1+1) 再轉成二進位就會是負數的二進位
			z = k + 4096;
		else //代表是 UJ type 需要加上2097152 再轉二進位才會是負數的二進位
			z = k + 2097152;
		return DecimalToBinary(z); //轉成二進位

	}
	else
		return DecimalToBinary(k); //轉成二進位
}

int main() {
	//rs[0] 通常為 rd 剩下的 rs;
	string s;
	vector<string> code;
	cout << "請開始輸入所有code，label請隔一行輸出且最後一個字元是:\n最後一行輸入-1結束\nEx:\nloop:\nadd x0,x1,x3\nbeq x0,x0,loop\nsw x1,1000(x3)\n-1\n";
	cout << "code 開始:\n";
	int i = 0;
	while (getline(cin, s)) {  //讀取所有的code 直到讀到-1就不讀code
		if (s == "-1")
			break;
		if (s[s.size() - 1] == ':') { //判斷這個是不是label標籤 是 就先存進一個label名稱的map裡面
			label[s] = i;
		}//loop:

		i++;
		code.push_back(s);
	}
	for (int k = 0; k < code.size(); k++) { //從第0行程式碼開始跑
		if (code[k][code[k].size() - 1] == ':')  //如果是label就不用繼續跑因為沒有machine code
			continue;
		stringstream ss;  //用來做字串分割用
		ss << code[k];  //傳進第 k 行 程式碼
		ss >> s;        //會傳出空格前的
		upperToLower(s);  //避免有人輸入大寫的add,sub,addi等code
		if (s == "jal") {  //如果輸入的是 jal 
			string b, a[2]; //b 存code[k]空白之後的code  /  a 存 b 裡面分割出來的 rd rs1 rs2 offset immediate
			ss >> b;  //分割空白之後的 code 放進 b;
			for (int i = 1; i < b.size();) {
				if (i == 1) {  //jal後第一個是一個register 所以當i==1 代表 這是這個register數字的開始
					int c = 0;
					c += (b[i] - '0') * 10;  //把第一個字元讀進來*10 因為它可能是十位數
					i++; //往後讀一格

					if (b[i] == ',' || i == b.size()) c /= 10; //往後一格後判斷這格是不是',' 是代表這個register只有一位數 代表前面讀的是個位數 必須除10才會變回個位數
					else {
						c += (b[i] - '0');  //代表他是十位數，要在讀入一格b[i]才完整 
						i++;
					}
					i++;
					a[0] = DecimalToBinary(c);  //把讀完的register 改成 binary number
				}
				else { //之後的所有字元都是label的部分 所以全部讀入進去a[1]裡面
					a[1] += b[i];
					i++;
				}
			}
			a[1] += ':'; //因為label(map)裡面存放的label都有: 但a[1]讀進來的沒有，所以要加上: 才能對上label(map)裡面的字串
			if (!label.count(a[1])) { //如果沒有這個label就輸出沒這個label 並跳過
				cout << "Doesn't have a label named:" << a[1] << endl;
				continue;
			}
			else {
				int z = k - label[a[1]]; //如果有就找出相差的數字是多少
				a[1] = offsetToBinary(z, "UJ");  //把這相差的數字轉成 binary number
			}
			UJ_type(s, a); //呼叫function來輸出 UJ_type 的 machine code
		}
		else if (s == "add" || s == "sub" || s == "sll" || s == "slt" || s == "sltu" || s == "xor" || //如果輸入的是 R_type 類型的code
			s == "srl" || s == "sra" || s == "or" || s == "and") {
			string b, a[3];    //b 存code[k]空白之後的code  /  a 存 b 裡面分割出來的 rd rs1 rs2 offset immediate
			ss >> b;  //分割空白之後的 code 放進 b;
			int j = 0; //用來分別現在要存入a的第幾個位置
			for (int i = 0; i < b.size();) {
				if (b[i] == 'x') {  //如果讀入的是 x 代表正在讀入register 讀法跟 189 行後的註解相同，只是存放的位置會因為這是第幾個register放進不同的a的位置
					int c = 0;
					i++;
					c += (b[i] - '0') * 10;
					i++;

					if (b[i] == ',' || i == b.size()) c /= 10;
					else {
						c += (b[i] - '0');
						i++;
					}
					i++;
					a[j] = DecimalToBinary(c);   //把讀完的register 改成 binary number
					j++;
				}
				else
					i++;
			}
			R_type(s, a);//呼叫function來輸出 R_type 的 machine code
		}
		else if (s == "slli" || s == "srli" || s == "sral") { //如果輸入的是 R_type 但是是 位移 類型的code
			string b, a[3];    //b 存code[k]空白之後的code  /  a 存 b 裡面分割出來的 rd rs1 rs2 offset immediate
			ss >> b; //分割空白之後的 code 放進 b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (b[i] == 'x') { //如果讀入的是 x 代表正在讀入register 讀法跟 189 行後的註解相同，只是存放的位置會因為這是第幾個register放進不同的a的位置
					int c = 0;
					i++;
					if (j < 2) {
						c += (b[i] - '0') * 10;
						i++;
						if (b[i] == ',') c /= 10;
						else {
							c += (b[i] - '0');
							i++;
						}
						i++;
					}
					a[j] = DecimalToBinary(c); //把讀完的register 改成 binary number
					j++;
				}
				else {
					int c = 0;
					while (i < b.size()) { //開始讀入剩下的字元，也代表讀入所有immediate的字元，並轉成 int 的形式
						c *= 10;
						c += (b[i] - '0');
						i++;
					}
					a[j] = offsetToBinary(c, "I"); //把 immediate 轉乘 binary number
					j++;
				}
			}
			R_type(s, a);//呼叫function來輸出 R_type 的 machine code
		}
		else if (s == "jalr" || s == "addi" || s == "slti" ||  //如果輸入的是 I_type 且輸入的是 addi x10,x10,1000 這樣類型的code
			s == "sltiu" || s == "xori" || s == "ori" || s == "andi") {
			string b, a[3];    //b 存code[k]空白之後的code  /  a 存 b 裡面分割出來的 rd rs1 rs2 offset immediate
			ss >> b; //分割空白之後的 code 放進 b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (b[i] == 'x') { //如果讀入的是 x 代表正在讀入register 讀法跟 189 行後的註解相同，只是存放的位置會因為這是第幾個register放進不同的a的位置
					int c = 0;
					i++;
					if (j < 2) {
						c += (b[i] - '0') * 10;
						i++;
						if (b[i] == ',') c /= 10;
						else {
							c += (b[i] - '0');
							i++;
						}
						i++;
					}
					a[j] = DecimalToBinary(c);  //把讀完的register 改成 binary number
					j++;
				}
				else {
					int c = 0;
					bool negative = false; //用來判斷這個讀進來的 immediate 是不是負數
					if (b[i] == '-') { //如果是第一個讀到的就會是'-'字元
						negative = true;
						i++;
					}
					while (i < b.size()) { //開始讀入剩下的字元，也代表讀入所有immediate的字元，並轉成 int 的形式
						c *= 10;
						c += (b[i] - '0');
						i++;
					}
					if (negative) //如果是負數就讓 c * -1 變成負數
						c *= -1;
					a[j] = offsetToBinary(c, "I");  //把 immediate 轉乘 binary number
					j++;
				}
			}
			while (a[2].size() < 12) //如果小於12個bit 要補成12個bit
				a[2].insert(a[2].begin(), '0');
			I_type(s, a);//呼叫function來輸出 I_type 的 machine code
		}
		else if (s == "lb" || s == "lh" || s == "lw" || s == "lbu" || s == "lhu") { //如果輸入的是 I_type 且輸入的是 lb x10,1000(x10) 這樣類型的code
			string b, a[3];    //b 存code[k]空白之後的code  /  a 存 b 裡面分割出來的 rd rs1 rs2 offset immediate
			ss >> b; //分割空白之後的 code 放進 b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (j == 1) { //代表第一個register 已經讀完了 要開始讀 offset(register)
					int c = 0;
					bool negative = false; //判斷offset是不是負數
					if (b[i] == '-') {
						negative = true;
						i++;
					}
					while (b[i] != '(') { //讀offset直到 (
						c *= 10;
						c += (b[i] - '0');
						i++;
					}
					if (negative)
						c *= -1;
					a[j] = offsetToBinary(c, "I"); //把 offset 轉乘 binary number
					while (a[1].size() < 12)   //如果小於12個bit 要補成12個bit
						a[1].insert(a[1].begin(), '0');
					j++;
				}
				else if (b[i] == 'x') { //如果讀入的是 x 代表正在讀入register 讀法跟 189 行後的註解相同，只是存放的位置會因為這是第幾個register放進不同的a的位置
					int c = 0;
					i++;
					c += (b[i] - '0') * 10;
					i++;
					if (b[i] == ',') c /= 10;
					else {
						c += (b[i] - '0');
						i++;
					}
					i++;

					a[j] = DecimalToBinary(c);
					j++;
				}
				else
					i++;
			}
			string tmp = a[1]; //對調 a[1] a[2]
			a[1] = a[2];
			a[2] = tmp;
			I_type(s, a);//呼叫function來輸出 I_type 的 machine code
		}
		else if (s == "sb" || s == "sh" || s == "sw") { //如果輸入的是 S_type 這樣類型的code
			string b, a[3];    //b 存code[k]空白之後的code  /  a 存 b 裡面分割出來的 rd rs1 rs2 offset immediate
			ss >> b;  //分割空白之後的 code 放進 b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (j == 1) { //代表第一個register 已經讀完了 要開始讀 offset(register)
					int c = 0;
					bool negative = false;//判斷offset是不是負數
					if (b[i] == '-') { 
						negative = true;
						i++;
					}
					while (b[i] != '(') {  //讀offset直到 (
						c *= 10;
						c += (b[i] - '0');
						i++;
					}
					if (negative)
						c *= -1;
					a[j] = offsetToBinary(c, "S");  //把 offset 轉乘 binary number

					while (a[1].size() < 12)  //如果小於12個bit 要補成12個bit
						a[1].insert(a[1].begin(), '0');
					j++;
				}
				else if (b[i] == 'x') { //如果讀入的是 x 代表正在讀入register 讀法跟 189 行後的註解相同，只是存放的位置會因為這是第幾個register放進不同的a的位置
					int c = 0;
					i++;
					c += (b[i] - '0') * 10;
					i++;
					if (b[i] == ',') c /= 10;
					else {
						c += (b[i] - '0');
						i++;
					}
					i++;
					a[j] = DecimalToBinary(c);
					j++;
				}
				else
					i++;
			}
			S_type(s, a);//呼叫function來輸出 S_type 的 machine code
		}
		else if (s == "beq" || s == "bne" || s == "blt" || s == "bge" || s == "bltu" || s == "bgeu") {  //如果輸入的是 SB_type 類型的code
			string b, a[3];     //b 存code[k]空白之後的code  /  a 存 b 裡面分割出來的 rd rs1 rs2 offset immediate
			ss >> b;  //分割空白之後的 code 放進 b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (b[i] == 'x') { //如果讀入的是 x 代表正在讀入register 讀法跟 189 行後的註解相同，只是存放的位置會因為這是第幾個register放進不同的a的位置
					int c = 0;
					i++;
					c += (b[i] - '0') * 10;
					i++;

					if (b[i] == ',' || i == b.size()) c /= 10;
					else {
						c += (b[i] - '0');
						i++;
					}
					i++;
					a[j] = DecimalToBinary(c);
					j++;
					if (j == 2) { //如果 j==2 代表已經讀完前兩個register 所以之後就要開始讀跳的label名稱
						while (i < b.size()) {
							a[2] += b[i];
							i++;
						}
					}
				}
				else
					i++;
			}
			a[2].push_back(':'); //同206行
			if (!label.count(a[2])) {
				cout << "Doesn't have a label named:" << a[2] << endl;
				continue;
			}
			else {
				int tmp = k - label[a[2]];
				a[2] = DecimalToBinary(tmp);
				while (a[2].size() < 13) //如果小於13個bit 要補成13個bit 因為branch會看到第13個bit
					a[2].insert(a[2].begin(), '0');
			}
			SB_type(s, a);//呼叫function來輸出 SB_type 的 machine code
		}
		else { //如果輸入code 不是 RV32I Base Integer Instruction Set 裡面的code
			cout << endl << s << " is a wrong word in Risc V !\n";
		}
	}
}
