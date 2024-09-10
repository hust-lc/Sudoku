#include "global.h"

int CNFparser(char* filename) {
	//重置全局变量
	dpll_called = 0;
	max_clause_len = 0;
	for (int i = 0; i < MAX_VARS; i++) {
		literal_info[i][SATISFIED].is_assigned = UNASSIGNED;	//没有被赋值 
		literal_info[i][SATISFIED].is_in_unit_clause = NO;		//没有在单子句中出现
		literal_info[i][SATISFIED].literal_clause = NULL;		//在公式中的位置
		literal_info[i][SATISFIED].literal_clause_pos = NULL;	//在子句中的位置
		literal_info[i][SATISFIED].n_number = 0;				//文字个数为0
		literal_info[i][SHRUNK].is_assigned = UNASSIGNED;
		literal_info[i][SHRUNK].is_in_unit_clause = NO;
		literal_info[i][SHRUNK].literal_clause = NULL;
		literal_info[i][SHRUNK].literal_clause_pos = NULL;
		literal_info[i][SHRUNK].n_number = 0;
	}
	for (int i = 0; i < 8 * MAX_CLAUSES; i++) {			//改变数都为0（未进行DPLL）
		changes_stack[i].index_of_clause = 0;
		changes_stack[i].index_of_literal = 0;
	}
	for (int i = 0; i < MAX_VARS; i++) {			//DPLL某一层满足或不满足的句子个数
		n_changes[i][0] = 0;
		n_changes[i][1] = 0;
	}
	changes_stack_index = 0;
	for (int i = 0; i < MAX_VARS; i++) {			//变元还未赋值
		result[i].value = UNASSIGNED;			
	}
	is_contradicted = 0;//是否用冲突的单子句
	conflicting_literal = 0;//用于记录冲突的文字
	n_unit_clause_stack = 0;//全局栈，用于记录栈顶位置
	depth = 0;//DPLL树中，结点的深度
	ifstream inFile;			//打开文件
	inFile.open(filename);
	if (!inFile.is_open()) {
		cout << "文件打开失败" << endl;
		exit(EXIT_FAILURE);
	}
	char c;
	char cnf[5];
	int x = 1;
	while (c = inFile.get() != 'p');	//跳过注释部分
	inFile.get();	//处理换行符
	inFile >> cnf >> n_vars >> original_formula_length;	//读入变元数和字句数
	inFile.get();	//处理换行符
	if (strcmp(cnf, "cnf"))return 0;	//判断文件格式
	current_formula_length = original_formula_length;	//初始化
	clauses = (clause_infomation*)malloc(sizeof(clause_infomation) * original_formula_length);//建立字句集
	for (int i = 0; i < original_formula_length; i++) {
		clauses[i].literals = NULL;				//字句的字母为空
		clauses[i].length_current = 0;			//字句字母的数量为0
		clauses[i].is_clause_satisfied = 0;	//初始为不满足
		clauses[i].literals_is_assigned = NULL;	//初始没有被安排
		clauses[i].unit_cluase_literal = 0;		//初始没有在单字句存在
		while (1) {
			//if(!(inFile >> x))goto file_is_finish;
			inFile >> x;
			if (x == 0) break;
			clauses[i].literals = (int*)realloc(clauses[i].literals, (++clauses[i].length_current) * (sizeof(int))); //建立每个字句的文字
			clauses[i].literals[clauses[i].length_current - 1] = x;		//初始化文字
			clauses[i].literals_is_assigned = (int*)realloc(clauses[i].literals_is_assigned, (clauses[i].length_current) * (sizeof(int)));;		//重新申请空间
			clauses[i].literals_is_assigned[clauses[i].length_current - 1] = UNASSIGNED;		//变元的布尔值未知,初始化为-1
			int t = x > 0 ? SATISFIED : SHRUNK;		//正值/负值各自建立一个文字结构
			int s = abs(x);

			++literal_info[s][t].n_number;					//变元x的数量++
			literal_info[s][t].literal_clause = (int*)realloc(literal_info[s][t].literal_clause, literal_info[s][t].n_number * (sizeof(int)));			//文字在公式中的第i个字句
			literal_info[s][t].literal_clause[literal_info[s][t].n_number - 1] = i;
			literal_info[s][t].literal_clause_pos = (int*)realloc(literal_info[s][t].literal_clause_pos, literal_info[s][t].n_number * (sizeof(int)));	//文字在子句中的位置
			literal_info[s][t].literal_clause_pos[literal_info[s][t].n_number - 1] = clauses[i].length_current - 1;										//文字在该字句中的第几个位置
		}
		clauses[i].length_original = clauses[i].length_current;		//当前长度等于初始长度
		if (clauses[i].length_original > max_clause_len) {
			max_clause_len = clauses[i].length_original;		//更新全局变量
		}
	}
	//file_is_finish:
	//fclose(fp);
	inFile.close();
	return 1;
}

void Preprocesser() {//预处理，将单子句加入全局栈
	for (int i = 0; i < original_formula_length; i++)
		if (clauses[i].length_original == 1) //将长度为一的变量加入全局栈  也就是将单子句加入全局栈
			unit_clause_stack[n_unit_clause_stack++] = clauses[i].literals[0];		//文字的值
}

int Display() {//功能选择界面显示
	system("cls");
	cout<<"选择一个选项"<<endl;
	cout<<"1 SAT"<<endl;
	cout<<"2 Sudoku"<<endl;
	cout<<"0 Exit"<<endl;
	int a;
	cin >> a;
	while (!(a == 0 || a == 1 || a == 2)) {
		cout << "请输入合法数字" << endl;
		cin >> a;
	}
	char tmp;
	while ((tmp = getchar()) != EOF && tmp != '\n');//清空输入缓存区
	return a;
}

void SaveToRes(int value, double time, char* filename) {//将结果输入到res文件
	int len = strlen(filename);
	filename[len - 3] = 'r';
	filename[len - 2] = 'e';
	filename[len - 1] = 's';
	ofstream outFile;
	outFile.open(filename);
	if (!outFile.is_open()) {
		cout << "结果保存为res文件失败" << endl;
		exit(EXIT_FAILURE);
	}
	if (value == 1) {
		outFile << "s 1" << endl << 'v';
		for (int i = 1; i <= n_vars; i++) {
			if (result[i].value == TRUE) outFile << ' ' << i;
			else outFile << ' ' << -1 * i;
		}
		outFile << endl << "t " << time << ' ' << dpll_called << endl;
	}
	else if (value == 0) outFile << "s 0" << endl << "t " << time << ' ' << dpll_called << endl;
	outFile.close();
}

void Free() {//释放分配的内存
	for (int i = 1; i <= n_vars; i++) {
		if (literal_info[i][0].literal_clause) {
			free(literal_info[i][0].literal_clause);
			literal_info[i][0].literal_clause = NULL;
		}
		if (literal_info[i][0].literal_clause_pos) {
			free(literal_info[i][0].literal_clause_pos);
			literal_info[i][0].literal_clause_pos = NULL;
		}
		if (literal_info[i][1].literal_clause) {
			free(literal_info[i][0].literal_clause);
			literal_info[i][0].literal_clause_pos = NULL;
		}
		if (literal_info[i][1].literal_clause_pos) {
			free(literal_info[i][0].literal_clause_pos);
			literal_info[i][0].literal_clause_pos = NULL;
		}
	}
	for (int i = 0; i < original_formula_length; i++) {
		if (clauses[i].literals) {
			free(clauses[i].literals);
		}
		if (clauses[i].literals_is_assigned) {
			free(clauses[i].literals_is_assigned);
		}
	}
	free(clauses);
}

void sat() {
	char filename[FILENAMESIZE];
	cout << "请输入文件名" << endl;
	cin.getline(filename, FILENAMESIZE);
	int cnf = CNFparser(filename);									//创建新的内部结构
	if (cnf == 0) {
		cout << "文件格式不正确" << endl;
		exit(EXIT_FAILURE);
	}
	else cout << "文件读取成功" << endl;
	cout << "请选择算法，输入 1 或 2" << endl << "1.普通算法   2.优化算法" << endl;
	cin >> op;
	while (!(op == 1 || op == 2)) {
		cout << "请输入合法数字" << endl;
		cin >> op;
	}
	clock_t starttime, endtime;
	cout << "开始求解" << endl << "Load···" << endl;
	starttime = clock();
	Preprocesser();													//预处理，将单子句加入全局栈
	int value = dpll(op);											//value表示求解是否成功
	endtime = clock();
	t = (double)(endtime - starttime) * 1000 / CLOCKS_PER_SEC;		//时间以ms为单位
	SaveToRes(value, t, filename);									//将结果输出到文件
	if (value == 1)cout << "sat问题解存在，用时" << t << "ms" << endl << "已将结果保存为res文件" << endl;
	else if (value == 0)cout << "sat问题解不存在，用时" << t << "ms" << endl << "已将结果保存为res文件" << endl;
	Free();															//释放malloc分配的空间
}