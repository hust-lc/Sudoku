#include "global.h"

int createSudoku(int i, int j) {//填充数独终盘
	if (i > 9 || j > 9) {
		return true;
	}
	for (int k = 1; k <= 9; ++k) {
		int type = true;                // type变量用于记录数字k能否放在 ( i , j ) 处
		if (type) {			//行约束
			for (int n = 1; n < j; ++n) {
				if (sudokuterminal[i][n] == k) {  // 检查同一行是否出现过数字k
					type = false;
					break;
				}
			}
		}
		if (type) {			//列约束
			for (int s = 1; s < i; ++s) {
				if (sudokuterminal[s][j] == k) {  // 检查同一列是否出现过数字k
					type = false;
					break;
				}
			}
		}
		if (type) {				//方格约束
			int upi = ((i - 1) / 3) * 3 + 3; // i坐标的上限
			int upj = ((j - 1) / 3) * 3 + 3;   // j坐标的上限
			for (int p = upi - 2; p <= upi; ++p) { //检查在3×3的小方格中是否出现了同一个数字 
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
		if (type) {				//对角约束1
			if (i == j) {
				for(int t=1;t<i;t++)
					if (sudokuterminal[t][t] == k) {
						type = false;
						break;
					}
			}
		}
		if (type) {			//对角约束2
			if (i+j==10) {
				for (int t = 1; t < i; t++)
					if (sudokuterminal[t][10-t] == k) {
						type = false;
						break;
					}
			}
		}
		if (type) {//搜索剩余格子
			sudokuterminal[i][j] = k;
			if (j < 9) {
				if (createSudoku(i, j + 1)) //到同一行的下一位置开始搜索 
					return true;
			}
			else {
				if (i < 9) {
					if (createSudoku(i + 1, 1))   //下一行的第一个位置开始搜索
						return true;
				}
				else {
					return true;  //i >= 9  && j >= 9  , 搜索结束
				}
			}
			sudokuterminal[i][j] = 0;   //回溯
		}
	}
	return false;		//创建失败
}

Sudoku* DigHoleSudoku(Sudoku* s, int up_times, int purpose_num) {//利用挖洞发产生数独
	int i, j, k, c, flag = 0, x, y;
	x = rand() % 9;
	y = rand() % 9;
	k = s->s[x][y];
	s->s[x][y] = 0;//先挖掉1个洞
	--s->n_current;
	while (true) {//挖洞过程
		startsolveSudoku(s, &c);//求解该数独，得到解的个数c
		if (c > 1) {	//此时解不唯一，取消挖洞
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
		do {					//找到一个挖洞点
			x = rand() % 9;
			y = rand() % 9;
		} while (s->s[x][y] == 0);
		k = s->s[x][y]; //此时(x,y)不为0，记录下这个值
		s->s[x][y] = 0;
		--s->n_current;
	}
	return s;
}

int startsolveSudoku(Sudoku* s, int* count) {//开始求解数独
	*count = 0;
	solveSudoku(s, 0, 0, count);
	if (*count > 0) {
		return true;
	}
	else {
		return false;
	}
}

int solveSudoku(Sudoku* s, int x, int y, int* count) {//求解数独
	int i, j, k;
	while (s->s[x][y] != 0 && x < 9) {//从上往下找找到第一个为0的空
		y++;
		if (y == 9) {//搜索完一行
			x++;
			y = 0;
		}
	}
	if (x > 8) {//这时数独棋盘没有空的
		(*count)++;
		return true;
	}
	for (i = 1; i <= 9; i++) {
		s->s[x][y] = i;
		if ((k = checkkeyword(s, x, y)) == true) {
			if (x == 8 && y == 8) {//达到最后一个格子
				s->s[x][y] = 0;
				(*count)++;
				return true;
			}
			else {
				if (y == 8)  //求解下一行
					solveSudoku(s, x + 1, 0, count);
				else  //求解下一列
					solveSudoku(s, x, y + 1, count);
			}
		}
	}
	s->s[x][y] = 0;
	return false;
}

int checkkeyword(Sudoku* s, int x, int y) {//检验填入的数字是否正确
	for (int i = x / 3 * 3; i < x / 3 * 3 + 3; i++) {//检查3*3九宫格是否冲突
		for (int j = y / 3 * 3; j < y / 3 * 3 + 3; j++) {
			if (s->s[x][y] == s->s[i][j] && (i != x || j != y)) {
				return false;
			}
		}
	}
	for (int i = 0; i < 9; i++) {  //检查横行是否冲突
		if (s->s[x][y] == s->s[i][y] && i != x) {
			return false;
		}
	}
	for (int i = 0; i < 9; i++) {  //检查竖列是否冲突
		if (s->s[x][y] == s->s[x][i] && i != y) {
			return false;
		}
	}
	return true;
}

inline int Location(int x, int y, int z) {//将数独中数字的位置转化为729个变元（9进制）
	return x * 81 + y * 9 + z;
}

int ToCNFFile(Sudoku* s, char* filename) {														//将数独转换为cnf文件
	ofstream outFile;
	outFile.open(filename);
	if (!outFile.is_open()) {
		cout << "文件创建失败" << endl;
		exit(EXIT_FAILURE);
	}
	outFile<<'c'<<endl << "p cnf 729 12654" << endl;
	for (int i = 0; i < 9; i++) 																//单元格约束
		for (int j = 0; j < 9; j++) {
			if (s->s[i][j] == 0) 
				for (int k = 1; k <= 9; k++) outFile << Location(i, j, k) << ' ';				//数独未知位置，至少有一个元素
			else outFile << Location(i, j, s->s[i][j]) << ' ';									//将数独已知元素赋值
			outFile << 0 << endl;
		}
	for (int i = 0; i < 9; i++) 																
		for (int j = 0; j < 9; j++)
			for (int m = 1; m <= 9; m++)
				for (int n = m + 1; n <= 9; n++)
					outFile << -1 * Location(i, j, m) << ' ' << -1 * Location(i, j, n) << ' ' << 0 << endl;

	for (int k = 1; k <= 9; k++) 																//行约束
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) outFile << Location(i, j, k) << ' ';					//每一行都含有1~9
			outFile <<0 << endl;
		}
	for (int k = 1; k <= 9; k++) 														
		for (int i = 0; i < 9; i++) 
			for (int m = 0; m < 9; m++) 
				for (int n = m + 1; n < 9; n++) 
					outFile << -1*Location(i, m, k) << ' ' <<-1* Location(i, n, k) <<' ' << 0 << endl;	//同一行中每两格不为同一个数字
	
	for (int k = 1; k <= 9; k++) 																//列约束
		for (int j = 0; j < 9; j++) {
			for (int i = 0; i < 9; i++) outFile << Location(i, j, k) << ' ';
			outFile << 0<< endl;
		}
	for (int k = 1; k <= 9; k++) 							
		for (int j = 0; j < 9; j++) 
			for (int m = 0; m < 9; m++) 
				for (int n = m + 1; n < 9; n++) 
					outFile << -1 * Location(m, j, k) << ' ' << -1 * Location(n, j, k) << ' ' << 0 << endl;
	
	for (int k = 1; k <= 9; k++) {														//正对角线约束
		for (int i = 0; i < 9; i++)outFile << Location(i, i, k) << ' ';					//正对角线含有1~9
		outFile << 0 << endl;
	}
	for (int k = 1; k <= 9; k++) 								
		for (int i = 0; i < 9; i++) 
			for (int m = i+1; m < 9; m++) 
					outFile << -1 * Location(i, i, k) << ' ' << -1 * Location(m, m, k) << ' ' << 0 << endl;		//正对角线每两格不相同
	
	for (int k = 1; k <= 9; k++) {														//负对角线约束
		for (int i = 0; i < 9; i++)outFile << Location(i, 8-i, k) << ' ';				
		outFile << 0 << endl;
	}
	for (int k = 1; k <= 9; k++) 							
		for (int i = 0; i < 9; i++) 
			for (int m = i + 1; m < 9; m++) 
				outFile << -1 * Location(i, 8-i, k) << ' ' << -1 * Location(m, 8-m, k) << ' ' << 0 << endl;		
	
	for (int i = 0; i < 7; i = i + 3) 												//九宫格约束
		for (int j = 0; j < 7; j = j + 3) 
			for (int k = 1; k <= 9; k++) {
				for (int pi = 0; pi < 3; pi++) 
					for (int pj = 0; pj < 3; pj++) 
						outFile << Location(i + pi, j + pj, k) << ' ';				//每个3*3格子包含1~9
					
				
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

Sudoku* ResultToSudoku() {//将求解结果转化为数独
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
						cout<<"同一位置有两个数字"<<endl;
						return NULL;
					}
					else {
						type = 1;
						S->s[i][j] = k;
					}
				}
				if (result[countk].value == UNASSIGNED) {
					cout<<"有数字没有赋值"<<endl;
				}
			}
		}
	}
	return S;
}

void game(Sudoku* gameboard, Sudoku* ans) {//游戏
	int is_acc = false;
	if (gameboard->n_current == 81) {
		is_acc = true;
	}
	while (is_acc == false) {
		system("cls");
		printSudoku(gameboard);
		cout << "还有" << 81 - gameboard->n_current << "个数字等待填充" << endl; 
		cout << "0 查看答案\n1 输入答案\n2 我要提示" << endl;
		int type;
		cin >> type;
		if (type == 0) {
			cout<<"答案即将公布"<<endl;
			printSudoku(ans);
			is_acc = TRUE;
		}
		else if (type == 1) {
			int x, y, z;
			cout<<"请输入：行号 列号 数字(1~9)"<<endl<<"例如，1 2 3表示在第1行第2列填入数字3"<<endl;
			cin >> x >> y >> z;
			if (1 <= x && x <= 9 && 1 <= y && y <= 9 && 1 <= z && z <= 9) {
				if (gameboard->s[x - 1][y - 1]) {
					cout<<"输入不合法，请重新输入"<<endl;
				}
				else {
					if (z == ans->s[x - 1][y - 1]) {
						cout<<"恭喜你，这是正确的"<<endl;
						gameboard->s[x - 1][y - 1] = z;
						gameboard->n_current++;
						if (gameboard->n_current == 81) {
							is_acc = TRUE;
							cout << "恭喜你求解出数独，你可真厉害！" << endl;
							printSudoku(ans);
						}
						else cout << "请继续输入数独的其他位置" << endl;
					}
					else {
						cout << "很遗憾，这是错误的，再试一试吧" << endl;;
					}
				}
			}
			else {
				printf("输入不合法，请重新输入\n");
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
				if (j == 8) {		//将i，j不断改变，直到该位置数独空缺
					j = 0; i++;
				}
				else {
					j++;
				}
				if (i == 9) {
					i = 0;
				}
			} while (i != x || j != y);
			cout << "第" << i + 1 << "行第" << j + 1 << "列为" << ans->s[i][j] << endl; 
			cout << "还剩"<<80- gameboard->n_current <<"个元素未解答" << endl;
			gameboard->s[i][j] = ans->s[i][j];
			gameboard->n_current++;
			if (gameboard->n_current == 81) {
				is_acc = TRUE;
				cout<<endl<<"数独已经求解出来啦！"<<endl;
				printSudoku(ans);
			}
		}
		else cout<<"输入不合法"<<endl;
		
		system("pause");
		
	}
}

void printSudoku(Sudoku* s) {//打印数独
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
	Sudoku* s = (Sudoku*)malloc(sizeof(Sudoku));		//对数独的第一行进行随即填充
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
	createSudoku(2, 1);						//从第二行开始搜索，建立数独终盘
	for (int i = 1; i < 10; i++) {					//将数独复制给s
		for (int j = 1; j < 10; j++) {
			s->s[i - 1][j - 1] = sudokuterminal[i][j];
		}
	}
	s->n_current = 81;								//用户选择要填充的数字个数
	cout << "请选择已知数字的个数：" << endl;
	cout << "24~80" << endl;
	int x;
	cin >> x;
	while (!(24 <= x && x <= 80)) {
		cout << "非法输入，请重新输入" << endl;
		cin >> x;
	}
	s = DigHoleSudoku(s, 100, x);//挖洞法生成数独//第二个数是难度
	if (s->n_current != x)cout << "数独无法仅保留" << x << "个数字" << endl << "本次生成数独保留了" << s->n_current << "个数字" << endl;
	else cout << "数独生成成功" << endl;
	int file_to_cnffile = ToCNFFile(s, filename);
	if (file_to_cnffile != 1) {
		cout << "数独转化为文件失败" << endl;
		exit(EXIT_FAILURE);
	}
	CNFparser(filename);//创建新的内部结构
	clock_t starttime, endtime;
	starttime = clock();
	Preprocesser();//预处理
	int value = dpll(op);
	endtime = clock();
	t = (double)(endtime - starttime) * 1000 / CLOCKS_PER_SEC;
	SaveToRes(value, t, filename);
	if (value == SAT) {//解出数独
		FinalSudoku = ResultToSudoku();
		game(s, FinalSudoku);//
		free(s);
		free(FinalSudoku);
	}
	else {//没有解出数独
		printf("数独没有解\n");
		free(s);
	}
	Free();
}