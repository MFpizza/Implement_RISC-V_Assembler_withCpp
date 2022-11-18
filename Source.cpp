#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cctype>
#include <vector>
using namespace std;
map<string, int> label; //�s��label��m���a��

//        _____________________________________________________
//R����  | funct5 | funct2 | rs2 | rs1 | funct3 | rd | opcdode |
//       |_____________________________________________________| 
void R_type(string s, string rs[]) {
	map<string, string> funct5; //��X�U��code ������funct5
	funct5["add"] = funct5["slli"] = funct5["srli"] = funct5["sll"] =
		funct5["slt"] = funct5["sltu"] = funct5["xor"] = funct5["srl"] =
		funct5["or"] = funct5["and"] = "00000";
	funct5["srai"] = funct5["sub"] = funct5["sra"] = "01000";

	map<string, string> funct3; //��X�U��code ������funct3
	funct3["add"] = funct3["sub"] = "000"; funct3["slt"] = "010";
	funct3["slli"] = funct3["sll"] = "001"; funct3["sltu"] = "011";
	funct3["xor"] = "100"; funct3["or"] = "110"; funct3["and"] = "111";
	funct3["srli"] = funct3["srai"] = funct3["srl"] = funct3["sra"] = "101";

	string funct2 = "00"; //funct2����00

	string opcode; //��X�U��code ������opcode
	if (s == "slli" || s == "srli" || s == "srai") opcode = "0010011";
	else opcode = "0110011";

	string BinaryOutPut = "";
	BinaryOutPut = funct5[s] + funct2 + rs[2] + rs[1] + funct3[s] + rs[0] + opcode;

	cout << BinaryOutPut << endl;
}
//        ___________________________________________
//I����  | imm[ 11:0 ] | rs1 | funct3 | rd | opcdode |
//       |___________________________________________| 
void I_type(string s, string rs[]) {
	map<string, string> funct3; //��X�U��code ������funct3
	funct3["jalr"] = funct3["lb"] = funct3["addi"] = "000";
	funct3["lh"] = "001"; funct3["lw"] = funct3["slti"] = "010";
	funct3["sltiu"] = "011"; funct3["lbu"] = funct3["xori"] = "100";
	funct3["lhu"] = "101"; funct3["ori"] = "110"; funct3["andi"] = "111";

	string opcode;  //��X�U��code ������opcode
	if (s == "jarl")
		opcode = "1100111";
	else if (s == "lb" || s == "lh" || s == "lw" || s == "lbu" || s == "lhu")
		opcode = "0000011";
	else
		opcode = "0010011";

	string BinaryOutPut = "";
	BinaryOutPut = rs[2] + rs[1] + funct3[s] + rs[0] + opcode;
	if (BinaryOutPut.size() > 32) { //�p�Goffset�L�j
		cout << s << "��offset�L�j\n";
		return;
	}
	cout << BinaryOutPut << endl;
}
//        _________________________________________________________
//S����  | imm[ 11:5 ] | rs2 | rs1 | funct3 | imm[ 4:0 ] | opcdode |
//       |_________________________________________________________| 
void S_type(string s, string rs[]) {
	map<string, string> funct3; //��X�U��code ������funct3
	funct3["sb"] = "000"; funct3["sw"] = "010"; funct3["sh"] = "001";

	string opcode = "0100011";

	string BinaryOutPut = "";
	for (int i = 11; i >= 5; i--)
		BinaryOutPut += rs[1][i];
	BinaryOutPut += rs[0] + rs[2] + funct3[s];
	for (int i = 4; i >= 0; i--)
		BinaryOutPut += rs[1][i];
	BinaryOutPut += opcode;
	if (rs[1].size() > 12) { //�p�Goffset�W�L�ү�Ū����bit
		cout << s << "��offset�L�j\n";
		return;
	}
	cout << BinaryOutPut << endl;
}
//        ___________________________________________________________________
//SB���� | imm[ 12 | 10:5 ] | rs2 | rs1 | funct3 | imm[ 4:1 | 11 ] | opcdode |
//       |___________________________________________________________________| 
void SB_type(string s, string rs[]) {
	string opcode = "1100011";
	map<string, string> funct3; //��X�U��code ������funct3
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
//UJ���� | imm[ 20 | 10:1 | 11 | 19:12 ] | rd | opcdode |
//       |______________________________________________| 	
void UJ_type(string s, string rs[]) { //rs[0] �� rd �� binary number rs[1]��immediate
	string BinaryOutPut = "";
	BinaryOutPut += rs[1][20];
	for (int i = 10; i > 0; i--)
		BinaryOutPut += rs[1][i];
	BinaryOutPut += rs[1][11];
	for (int i = 19; i >= 12; i--)
		BinaryOutPut += rs[1][i];
	BinaryOutPut += rs[0] + "1101111"; //1101111�Oopcode

	cout << BinaryOutPut << endl;
}

string DecimalToBinary(int a) { //�Q�i���ন�G�i���
	string s;
	while (a != 0) { //�ন�G�i�쪺string�Φ�
		s.insert(s.begin(), (a % 2 + '0'));
		a /= 2;
	}
	while (s.size() < 5) //�p�G�S�F��5bit �n�ɦ�5bit
		s.insert(s.begin(), '0');

	return s;
}

void upperToLower(string& s) { //�j�g�ন�p�g
	for (int i = 0; i < s.size(); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += 'a' - 'A';
	}
}

string offsetToBinary(int k, string type) { //�Ƕi�@�ӼƦr��L������
	if (k < 0) { //�p�G�O�t��
		int z;
		if (type == "I" || type == "S") //�p�G�O I �� S �N��o�� �n�����ɼ� �n�[�W4096 (12bit��1+1) �A�ন�G�i��N�|�O�t�ƪ��G�i��
			z = k + 4096;
		else //�N��O UJ type �ݭn�[�W2097152 �A��G�i��~�|�O�t�ƪ��G�i��
			z = k + 2097152;
		return DecimalToBinary(z); //�ন�G�i��

	}
	else
		return DecimalToBinary(k); //�ন�G�i��
}

int main() {
	//rs[0] �q�`�� rd �ѤU�� rs;
	string s;
	vector<string> code;
	cout << "�ж}�l��J�Ҧ�code�Alabel�йj�@���X�B�̫�@�Ӧr���O:\n�̫�@���J-1����\nEx:\nloop:\nadd x0,x1,x3\nbeq x0,x0,loop\nsw x1,1000(x3)\n-1\n";
	cout << "code �}�l:\n";
	int i = 0;
	while (getline(cin, s)) {  //Ū���Ҧ���code ����Ū��-1�N��Ūcode
		if (s == "-1")
			break;
		if (s[s.size() - 1] == ':') { //�P�_�o�ӬO���Olabel���� �O �N���s�i�@��label�W�٪�map�̭�
			label[s] = i;
		}//loop:

		i++;
		code.push_back(s);
	}
	for (int k = 0; k < code.size(); k++) { //�q��0��{���X�}�l�]
		if (code[k][code[k].size() - 1] == ':')  //�p�G�Olabel�N�����~��]�]���S��machine code
			continue;
		stringstream ss;  //�ΨӰ��r����Υ�
		ss << code[k];  //�Ƕi�� k �� �{���X
		ss >> s;        //�|�ǥX�Ů�e��
		upperToLower(s);  //�קK���H��J�j�g��add,sub,addi��code
		if (s == "jal") {  //�p�G��J���O jal 
			string b, a[2]; //b �scode[k]�ťդ��᪺code  /  a �s b �̭����ΥX�Ӫ� rd rs1 rs2 offset immediate
			ss >> b;  //���Ϊťդ��᪺ code ��i b;
			for (int i = 1; i < b.size();) {
				if (i == 1) {  //jal��Ĥ@�ӬO�@��register �ҥH��i==1 �N�� �o�O�o��register�Ʀr���}�l
					int c = 0;
					c += (b[i] - '0') * 10;  //��Ĥ@�Ӧr��Ū�i��*10 �]�����i��O�Q���
					i++; //����Ū�@��

					if (b[i] == ',' || i == b.size()) c /= 10; //����@���P�_�o��O���O',' �O�N��o��register�u���@��� �N��e��Ū���O�Ӧ�� ������10�~�|�ܦ^�Ӧ��
					else {
						c += (b[i] - '0');  //�N��L�O�Q��ơA�n�bŪ�J�@��b[i]�~���� 
						i++;
					}
					i++;
					a[0] = DecimalToBinary(c);  //��Ū����register �令 binary number
				}
				else { //���᪺�Ҧ��r�����Olabel������ �ҥH����Ū�J�i�ha[1]�̭�
					a[1] += b[i];
					i++;
				}
			}
			a[1] += ':'; //�]��label(map)�̭��s��label����: ��a[1]Ū�i�Ӫ��S���A�ҥH�n�[�W: �~���Wlabel(map)�̭����r��
			if (!label.count(a[1])) { //�p�G�S���o��label�N��X�S�o��label �ø��L
				cout << "Doesn't have a label named:" << a[1] << endl;
				continue;
			}
			else {
				int z = k - label[a[1]]; //�p�G���N��X�ۮt���Ʀr�O�h��
				a[1] = offsetToBinary(z, "UJ");  //��o�ۮt���Ʀr�ন binary number
			}
			UJ_type(s, a); //�I�sfunction�ӿ�X UJ_type �� machine code
		}
		else if (s == "add" || s == "sub" || s == "sll" || s == "slt" || s == "sltu" || s == "xor" || //�p�G��J���O R_type ������code
			s == "srl" || s == "sra" || s == "or" || s == "and") {
			string b, a[3];    //b �scode[k]�ťդ��᪺code  /  a �s b �̭����ΥX�Ӫ� rd rs1 rs2 offset immediate
			ss >> b;  //���Ϊťդ��᪺ code ��i b;
			int j = 0; //�ΨӤ��O�{�b�n�s�Ja���ĴX�Ӧ�m
			for (int i = 0; i < b.size();) {
				if (b[i] == 'x') {  //�p�GŪ�J���O x �N���bŪ�Jregister Ū�k�� 189 ��᪺���ѬۦP�A�u�O�s�񪺦�m�|�]���o�O�ĴX��register��i���P��a����m
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
					a[j] = DecimalToBinary(c);   //��Ū����register �令 binary number
					j++;
				}
				else
					i++;
			}
			R_type(s, a);//�I�sfunction�ӿ�X R_type �� machine code
		}
		else if (s == "slli" || s == "srli" || s == "sral") { //�p�G��J���O R_type ���O�O �첾 ������code
			string b, a[3];    //b �scode[k]�ťդ��᪺code  /  a �s b �̭����ΥX�Ӫ� rd rs1 rs2 offset immediate
			ss >> b; //���Ϊťդ��᪺ code ��i b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (b[i] == 'x') { //�p�GŪ�J���O x �N���bŪ�Jregister Ū�k�� 189 ��᪺���ѬۦP�A�u�O�s�񪺦�m�|�]���o�O�ĴX��register��i���P��a����m
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
					a[j] = DecimalToBinary(c); //��Ū����register �令 binary number
					j++;
				}
				else {
					int c = 0;
					while (i < b.size()) { //�}�lŪ�J�ѤU���r���A�]�N��Ū�J�Ҧ�immediate���r���A���ন int ���Φ�
						c *= 10;
						c += (b[i] - '0');
						i++;
					}
					a[j] = offsetToBinary(c, "I"); //�� immediate �୼ binary number
					j++;
				}
			}
			R_type(s, a);//�I�sfunction�ӿ�X R_type �� machine code
		}
		else if (s == "jalr" || s == "addi" || s == "slti" ||  //�p�G��J���O I_type �B��J���O addi x10,x10,1000 �o��������code
			s == "sltiu" || s == "xori" || s == "ori" || s == "andi") {
			string b, a[3];    //b �scode[k]�ťդ��᪺code  /  a �s b �̭����ΥX�Ӫ� rd rs1 rs2 offset immediate
			ss >> b; //���Ϊťդ��᪺ code ��i b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (b[i] == 'x') { //�p�GŪ�J���O x �N���bŪ�Jregister Ū�k�� 189 ��᪺���ѬۦP�A�u�O�s�񪺦�m�|�]���o�O�ĴX��register��i���P��a����m
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
					a[j] = DecimalToBinary(c);  //��Ū����register �令 binary number
					j++;
				}
				else {
					int c = 0;
					bool negative = false; //�ΨӧP�_�o��Ū�i�Ӫ� immediate �O���O�t��
					if (b[i] == '-') { //�p�G�O�Ĥ@��Ū�쪺�N�|�O'-'�r��
						negative = true;
						i++;
					}
					while (i < b.size()) { //�}�lŪ�J�ѤU���r���A�]�N��Ū�J�Ҧ�immediate���r���A���ন int ���Φ�
						c *= 10;
						c += (b[i] - '0');
						i++;
					}
					if (negative) //�p�G�O�t�ƴN�� c * -1 �ܦ��t��
						c *= -1;
					a[j] = offsetToBinary(c, "I");  //�� immediate �୼ binary number
					j++;
				}
			}
			while (a[2].size() < 12) //�p�G�p��12��bit �n�ɦ�12��bit
				a[2].insert(a[2].begin(), '0');
			I_type(s, a);//�I�sfunction�ӿ�X I_type �� machine code
		}
		else if (s == "lb" || s == "lh" || s == "lw" || s == "lbu" || s == "lhu") { //�p�G��J���O I_type �B��J���O lb x10,1000(x10) �o��������code
			string b, a[3];    //b �scode[k]�ťդ��᪺code  /  a �s b �̭����ΥX�Ӫ� rd rs1 rs2 offset immediate
			ss >> b; //���Ϊťդ��᪺ code ��i b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (j == 1) { //�N��Ĥ@��register �w�gŪ���F �n�}�lŪ offset(register)
					int c = 0;
					bool negative = false; //�P�_offset�O���O�t��
					if (b[i] == '-') {
						negative = true;
						i++;
					}
					while (b[i] != '(') { //Ūoffset���� (
						c *= 10;
						c += (b[i] - '0');
						i++;
					}
					if (negative)
						c *= -1;
					a[j] = offsetToBinary(c, "I"); //�� offset �୼ binary number
					while (a[1].size() < 12)   //�p�G�p��12��bit �n�ɦ�12��bit
						a[1].insert(a[1].begin(), '0');
					j++;
				}
				else if (b[i] == 'x') { //�p�GŪ�J���O x �N���bŪ�Jregister Ū�k�� 189 ��᪺���ѬۦP�A�u�O�s�񪺦�m�|�]���o�O�ĴX��register��i���P��a����m
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
			string tmp = a[1]; //��� a[1] a[2]
			a[1] = a[2];
			a[2] = tmp;
			I_type(s, a);//�I�sfunction�ӿ�X I_type �� machine code
		}
		else if (s == "sb" || s == "sh" || s == "sw") { //�p�G��J���O S_type �o��������code
			string b, a[3];    //b �scode[k]�ťդ��᪺code  /  a �s b �̭����ΥX�Ӫ� rd rs1 rs2 offset immediate
			ss >> b;  //���Ϊťդ��᪺ code ��i b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (j == 1) { //�N��Ĥ@��register �w�gŪ���F �n�}�lŪ offset(register)
					int c = 0;
					bool negative = false;//�P�_offset�O���O�t��
					if (b[i] == '-') { 
						negative = true;
						i++;
					}
					while (b[i] != '(') {  //Ūoffset���� (
						c *= 10;
						c += (b[i] - '0');
						i++;
					}
					if (negative)
						c *= -1;
					a[j] = offsetToBinary(c, "S");  //�� offset �୼ binary number

					while (a[1].size() < 12)  //�p�G�p��12��bit �n�ɦ�12��bit
						a[1].insert(a[1].begin(), '0');
					j++;
				}
				else if (b[i] == 'x') { //�p�GŪ�J���O x �N���bŪ�Jregister Ū�k�� 189 ��᪺���ѬۦP�A�u�O�s�񪺦�m�|�]���o�O�ĴX��register��i���P��a����m
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
			S_type(s, a);//�I�sfunction�ӿ�X S_type �� machine code
		}
		else if (s == "beq" || s == "bne" || s == "blt" || s == "bge" || s == "bltu" || s == "bgeu") {  //�p�G��J���O SB_type ������code
			string b, a[3];     //b �scode[k]�ťդ��᪺code  /  a �s b �̭����ΥX�Ӫ� rd rs1 rs2 offset immediate
			ss >> b;  //���Ϊťդ��᪺ code ��i b;
			int j = 0;
			for (int i = 0; i < b.size();) {
				if (b[i] == 'x') { //�p�GŪ�J���O x �N���bŪ�Jregister Ū�k�� 189 ��᪺���ѬۦP�A�u�O�s�񪺦�m�|�]���o�O�ĴX��register��i���P��a����m
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
					if (j == 2) { //�p�G j==2 �N��w�gŪ���e���register �ҥH����N�n�}�lŪ����label�W��
						while (i < b.size()) {
							a[2] += b[i];
							i++;
						}
					}
				}
				else
					i++;
			}
			a[2].push_back(':'); //�P206��
			if (!label.count(a[2])) {
				cout << "Doesn't have a label named:" << a[2] << endl;
				continue;
			}
			else {
				int tmp = k - label[a[2]];
				a[2] = DecimalToBinary(tmp);
				while (a[2].size() < 13) //�p�G�p��13��bit �n�ɦ�13��bit �]��branch�|�ݨ��13��bit
					a[2].insert(a[2].begin(), '0');
			}
			SB_type(s, a);//�I�sfunction�ӿ�X SB_type �� machine code
		}
		else { //�p�G��Jcode ���O RV32I Base Integer Instruction Set �̭���code
			cout << endl << s << " is a wrong word in Risc V !\n";
		}
	}
}
