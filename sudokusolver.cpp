#include "global.h"

int createSudoku(int i, int j) {//�����������
	if (i > 9 || j > 9) {
		return true;
	}
	for (int k = 1; k <= 9; ++k) {
		int type = true;                // type�������ڼ�¼����k�ܷ���� ( i , j ) ��
		if (type) {			//��Լ��
			for (int n = 1; n < j; ++n) {
				if (sudokuterminal[i][n] == k) {  // ���ͬһ���Ƿ���ֹ�����k
					type = false;
					break;
				}
			}
		}
		if (type) {			//��Լ��
			for (int s = 1; s < i; ++s) {
				if (sudokuterminal[s][j] == k) {  // ���ͬһ���Ƿ���ֹ�����k
					type = false;
					break;
				}
			}
		}
		if (type) {				//����Լ��
			int upi = ((i - 1) / 3) * 3 + 3; // i���������
			int upj = ((j - 1) / 3) * 3 + 3;   // j���������
			for (int p = upi - 2; p <= upi; ++p) { //�����3��3��С�������Ƿ������ͬһ������ 
				if (type == false) {
					break;
				}
				for (int q = upj - 2; q <= upj; ++q) {
					if (sudokuterminal[p][q] == k) {
						type = false;
						break;
					}
				}
			}
		}
		if (type) {				//�Խ�Լ��1
			if (i == j) {
				for(int t=1;t<i;t++)
					if (sudokuterminal[t][t] == k) {
						type = false;
						break;
					}
			}
		}
		if (type) {			//�Խ�Լ��2
			if (i+j==10) {
				for (int t = 1; t < i; t++)
					if (sudokuterminal[t][10-t] == k) {
						type = false;
						break;
					}
			}
		}
		if (type) {//����ʣ�����
			sudokuterminal[i][j] = k;
			if (j < 9) {
				if (createSudoku(i, j + 1)) //��ͬһ�е���һλ�ÿ�ʼ���� 
					return true;
			}
			else {
				if (i < 9) {
					if (createSudoku(i + 1, 1))   //��һ�еĵ�һ��λ�ÿ�ʼ����
						return true;
				}
				else {
					return true;  //i >= 9  && j >= 9  , ��������
				}
			}
			sudokuterminal[i][j] = 0;   //����
		}
	}
	return false;		//����ʧ��
}

Sudoku* DigHoleSudoku(Sudoku* s, int up_times, int purpose_num) {//�����ڶ�����������
	int i, j, k, c, flag = 0, x, y;
	x = rand() % 9;
	y = rand() % 9;
	k = s->s[x][y];
	s->s[x][y] = 0;//���ڵ�1����
	--s->n_current;
	while (true) {//�ڶ�����
		startsolveSudoku(s, &c);//�����������õ���ĸ���c
		if (c > 1) {	//��ʱ�ⲻΨһ��ȡ���ڶ�
			s->s[x][y] = k;
			++s->n_current;
			flag++; //
			if (flag >= up_times) {
				break;
			}
		}
		if (purpose_num >= s->n_current) {
			break;
		}
		do {					//�ҵ�һ���ڶ���
			x = rand() % 9;
			y = rand() % 9;
		} while (s->s[x][y] == 0);
		k = s->s[x][y]; //��ʱ(x,y)��Ϊ0����¼�����ֵ
		s->s[x][y] = 0;
		--s->n_current;
	}
	return s;
}

int startsolveSudoku(Sudoku* s, int* count) {//��ʼ�������
	*count = 0;
	solveSudoku(s, 0, 0, count);
	if (*count > 0) {
		return true;
	}
	else {
		return false;
	}
}

int solveSudoku(Sudoku* s, int x, int y, int* count) {//�������
	int i, j, k;
	while (s->s[x][y] != 0 && x < 9) {//�����������ҵ���һ��Ϊ0�Ŀ�
		y++;
		if (y == 9) {//������һ��
			x++;
			y = 0;
		}
	}
	if (x > 8) {//��ʱ��������û�пյ�
		(*count)++;
		return true;
	}
	for (i = 1; i <= 9; i++) {
		s->s[x][y] = i;
		if ((k = checkkeyword(s, x, y)) == true) {
			if (x == 8 && y == 8) {//�ﵽ���һ������
				s->s[x][y] = 0;
				(*count)++;
				return true;
			}
			else {
				if (y == 8)  //�����һ��
					solveSudoku(s, x + 1, 0, count);
				else  //�����һ��
					solveSudoku(s, x, y + 1, count);
			}
		}
	}
	s->s[x][y] = 0;
	return false;
}

int checkkeyword(Sudoku* s, int x, int y) {//��������������Ƿ���ȷ
	for (int i = x / 3 * 3; i < x / 3 * 3 + 3; i++) {//���3*3�Ź����Ƿ��ͻ
		for (int j = y / 3 * 3; j < y / 3 * 3 + 3; j++) {
			if (s->s[x][y] == s->s[i][j] && (i != x || j != y)) {
				return false;
			}
		}
	}
	for (int i = 0; i < 9; i++) {  //�������Ƿ��ͻ
		if (s->s[x][y] == s->s[i][y] && i != x) {
			return false;
		}
	}
	for (int i = 0; i < 9; i++) {  //��������Ƿ��ͻ
		if (s->s[x][y] == s->s[x][i] && i != y) {
			return false;
		}
	}
	return true;
}

inline int Location(int x, int y, int z) {//�����������ֵ�λ��ת��Ϊ729����Ԫ��9���ƣ�
	return x * 81 + y * 9 + z;
}

int ToCNFFile(Sudoku* s, char* filename) {														//������ת��Ϊcnf�ļ�
	ofstream outFile;
	outFile.open(filename);
	if (!outFile.is_open()) {
		cout << "�ļ�����ʧ��" << endl;
		exit(EXIT_FAILURE);
	}
	outFile<<'c'<<endl << "p cnf 729 12654" << endl;
	for (int i = 0; i < 9; i++) 																//��Ԫ��Լ��
		for (int j = 0; j < 9; j++) {
			if (s->s[i][j] == 0) 
				for (int k = 1; k <= 9; k++) outFile << Location(i, j, k) << ' ';				//����δ֪λ�ã�������һ��Ԫ��
			else outFile << Location(i, j, s->s[i][j]) << ' ';									//��������֪Ԫ�ظ�ֵ
			outFile << 0 << endl;
		}
	for (int i = 0; i < 9; i++) 																
		for (int j = 0; j < 9; j++)
			for (int m = 1; m <= 9; m++)
				for (int n = m + 1; n <= 9; n++)
					outFile << -1 * Location(i, j, m) << ' ' << -1 * Location(i, j, n) << ' ' << 0 << endl;

	for (int k = 1; k <= 9; k++) 																//��Լ��
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) outFile << Location(i, j, k) << ' ';					//ÿһ�ж�����1~9
			outFile <<0 << endl;
		}
	for (int k = 1; k <= 9; k++) 														
		for (int i = 0; i < 9; i++) 
			for (int m = 0; m < 9; m++) 
				for (int n = m + 1; n < 9; n++) 
					outFile << -1*Location(i, m, k) << ' ' <<-1* Location(i, n, k) <<' ' << 0 << endl;	//ͬһ����ÿ����Ϊͬһ������
	
	for (int k = 1; k <= 9; k++) 																//��Լ��
		for (int j = 0; j < 9; j++) {
			for (int i = 0; i < 9; i++) outFile << Location(i, j, k) << ' ';
			outFile << 0<< endl;
		}
	for (int k = 1; k <= 9; k++) 							
		for (int j = 0; j < 9; j++) 
			for (int m = 0; m < 9; m++) 
				for (int n = m + 1; n < 9; n++) 
					outFile << -1 * Location(m, j, k) << ' ' << -1 * Location(n, j, k) << ' ' << 0 << endl;
	
	for (int k = 1; k <= 9; k++) {														//���Խ���Լ��
		for (int i = 0; i < 9; i++)outFile << Location(i, i, k) << ' ';					//���Խ��ߺ���1~9
		outFile << 0 << endl;
	}
	for (int k = 1; k <= 9; k++) 								
		for (int i = 0; i < 9; i++) 
			for (int m = i+1; m < 9; m++) 
					outFile << -1 * Location(i, i, k) << ' ' << -1 * Location(m, m, k) << ' ' << 0 << endl;		//���Խ���ÿ������ͬ
	
	for (int k = 1; k <= 9; k++) {														//���Խ���Լ��
		for (int i = 0; i < 9; i++)outFile << Location(i, 8-i, k) << ' ';				
		outFile << 0 << endl;
	}
	for (int k = 1; k <= 9; k++) 							
		for (int i = 0; i < 9; i++) 
			for (int m = i + 1; m < 9; m++) 
				outFile << -1 * Location(i, 8-i, k) << ' ' << -1 * Location(m, 8-m, k) << ' ' << 0 << endl;		
	
	for (int i = 0; i < 7; i = i + 3) 												//�Ź���Լ��
		for (int j = 0; j < 7; j = j + 3) 
			for (int k = 1; k <= 9; k++) {
				for (int pi = 0; pi < 3; pi++) 
					for (int pj = 0; pj < 3; pj++) 
						outFile << Location(i + pi, j + pj, k) << ' ';				//ÿ��3*3���Ӱ���1~9
					
				
				outFile << 0 << endl;
			}
		
	int respi[10] = { 0 };
	int respj[10] = { 0 };
	int countt = 0;
	for (int pi = 0; pi < 3; pi++) 
		for (int pj = 0; pj < 3; pj++) {
			respi[countt] = pi;
			respj[countt] = pj;
			countt++;
		}
	for (int i = 0; i < 7; i = i + 3) 
		for (int j = 0; j < 7; j = j + 3) 
			for (int k = 1; k <= 9; k++) 
				for (int m = 0; m < 9; m++) 
					for (int n = m + 1; n < 9; n++) 
						outFile << -1 * Location(i + respi[m], j + respj[m], k) << ' ' << -1 * Location(i + respi[n], j + respj[n], k) << ' ' << 0 << endl;
	return 1;
}

Sudoku* ResultToSudoku() {//�������ת��Ϊ����
	if (n_vars != 729) {
		return NULL;
	}
	Sudoku* S = (Sudoku*)malloc(sizeof(Sudoku));
	int countk = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int type = 0;
			for (int k = 1; k <= 9; k++) {
				if (result[++countk].value == TRUE) {
					if (type == 1) {
						cout<<"ͬһλ������������"<<endl;
						return NULL;
					}
					else {
						type = 1;
						S->s[i][j] = k;
					}
				}
				if (result[countk].value == UNASSIGNED) {
					cout<<"������û�и�ֵ"<<endl;
				}
			}
		}
	}
	return S;
}

void game(Sudoku* gameboard, Sudoku* ans) {//��Ϸ
	int is_acc = false;
	if (gameboard->n_current == 81) {
		is_acc = true;
	}
	while (is_acc == false) {
		system("cls");
		printSudoku(gameboard);
		cout << "����" << 81 - gameboard->n_current << "�����ֵȴ����" << endl; 
		cout << "0 �鿴��\n1 �����\n2 ��Ҫ��ʾ" << endl;
		int type;
		cin >> type;
		if (type == 0) {
			cout<<"�𰸼�������"<<endl;
			printSudoku(ans);
			is_acc = TRUE;
		}
		else if (type == 1) {
			int x, y, z;
			cout<<"�����룺�к� �к� ����(1~9)"<<endl<<"���磬1 2 3��ʾ�ڵ�1�е�2����������3"<<endl;
			cin >> x >> y >> z;
			if (1 <= x && x <= 9 && 1 <= y && y <= 9 && 1 <= z && z <= 9) {
				if (gameboard->s[x - 1][y - 1]) {
					cout<<"���벻�Ϸ�������������"<<endl;
				}
				else {
					if (z == ans->s[x - 1][y - 1]) {
						cout<<"��ϲ�㣬������ȷ��"<<endl;
						gameboard->s[x - 1][y - 1] = z;
						gameboard->n_current++;
						if (gameboard->n_current == 81) {
							is_acc = TRUE;
							cout << "��ϲ�����������������������" << endl;
							printSudoku(ans);
						}
						else cout << "�������������������λ��" << endl;
					}
					else {
						cout << "���ź������Ǵ���ģ�����һ�԰�" << endl;;
					}
				}
			}
			else {
				printf("���벻�Ϸ�������������\n");
			}
		}
		else if (type == 2) {
			int x = rand() % 9;
			int y = rand() % 9;
			int i = x, j = y;
			do {
				if (gameboard->s[i][j] == 0) {
					break;
				}
				if (j == 8) {		//��i��j���ϸı䣬ֱ����λ��������ȱ
					j = 0; i++;
				}
				else {
					j++;
				}
				if (i == 9) {
					i = 0;
				}
			} while (i != x || j != y);
			cout << "��" << i + 1 << "�е�" << j + 1 << "��Ϊ" << ans->s[i][j] << endl; 
			cout << "��ʣ"<<80- gameboard->n_current <<"��Ԫ��δ���" << endl;
			gameboard->s[i][j] = ans->s[i][j];
			gameboard->n_current++;
			if (gameboard->n_current == 81) {
				is_acc = TRUE;
				cout<<endl<<"�����Ѿ�����������"<<endl;
				printSudoku(ans);
			}
		}
		else cout<<"���벻�Ϸ�"<<endl;
		
		system("pause");
		
	}
}

void printSudoku(Sudoku* s) {//��ӡ����
	int i, j;
	cout<<" -----------------"<<endl;
	for (i = 0; i < 9; i++) {
		cout<<'|';
		for (j = 0; j < 9; j++) {
			if ((j + 1) % 3 == 0)cout << s->s[i][j] << '|';
			else cout << s->s[i][j] << ' ';
		}
		cout << endl;
		if ((i + 1) % 3 == 0) cout<<" -----------------"<<endl;
		
	}
}

void sudoku() {
	Sudoku* s = (Sudoku*)malloc(sizeof(Sudoku));		//�������ĵ�һ�н����漴���
	Sudoku* FinalSudoku = NULL;
	char filename[100] = "Sudoku.cnf";
	srand(unsigned(time(NULL)));
	for (int i = 1; i <= 9; ++i)
		for (int j = 1; j <= 9; ++j)
			sudokuterminal[i][j] = 0;
	int visited[10] = { 0 };
	for (int i = 1; i <= 9; i++) {
		while (sudokuterminal[1][i] == 0) {
			int data = rand() % 9 + 1;
			if (visited[data] == 0) {
				visited[data] = 1;
				sudokuterminal[1][i] = data;
			}
		}
	}
	createSudoku(2, 1);						//�ӵڶ��п�ʼ������������������
	for (int i = 1; i < 10; i++) {					//���������Ƹ�s
		for (int j = 1; j < 10; j++) {
			s->s[i - 1][j - 1] = sudokuterminal[i][j];
		}
	}
	s->n_current = 81;								//�û�ѡ��Ҫ�������ָ���
	cout << "��ѡ����֪���ֵĸ�����" << endl;
	cout << "24~80" << endl;
	int x;
	cin >> x;
	while (!(24 <= x && x <= 80)) {
		cout << "�Ƿ����룬����������" << endl;
		cin >> x;
	}
	s = DigHoleSudoku(s, 100, x);//�ڶ�����������//�ڶ��������Ѷ�
	if (s->n_current != x)cout << "�����޷�������" << x << "������" << endl << "������������������" << s->n_current << "������" << endl;
	else cout << "�������ɳɹ�" << endl;
	int file_to_cnffile = ToCNFFile(s, filename);
	if (file_to_cnffile != 1) {
		cout << "����ת��Ϊ�ļ�ʧ��" << endl;
		exit(EXIT_FAILURE);
	}
	CNFparser(filename);//�����µ��ڲ��ṹ
	clock_t starttime, endtime;
	starttime = clock();
	Preprocesser();//Ԥ����
	int value = dpll(op);
	endtime = clock();
	t = (double)(endtime - starttime) * 1000 / CLOCKS_PER_SEC;
	SaveToRes(value, t, filename);
	if (value == SAT) {//�������
		FinalSudoku = ResultToSudoku();
		game(s, FinalSudoku);//
		free(s);
		free(FinalSudoku);
	}
	else {//û�н������
		printf("����û�н�\n");
		free(s);
	}
	Free();
}