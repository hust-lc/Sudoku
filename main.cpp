#include "global.h"

int dpll_called;									//统计DPLL调用次数的全局变量
int sudokuterminal[10][10];							//数独棋盘
literal_infomation literal_info[MAX_VARS][2];		//文字存储数组
clause_infomation* clauses;							//子句存储数组++++
int original_formula_length, current_formula_length;//原长度，现长度
changes_infomation changes_stack[8 * MAX_CLAUSES];	//记录该变量
unsigned int changes_stack_index;					//数组changes的下标
unsigned int n_changes[MAX_VARS][2];				//某一层满足或不满足的句子的个数
assign_infomation result[MAX_VARS];					//用于回溯操作的记录
int is_contradicted;								//是否用冲突的单子句
int conflicting_literal;							//用于记录冲突的文字
int unit_clause_stack[MAX_CLAUSES];					//全局栈
int n_unit_clause_stack;							//用于记录栈顶位置
int depth;											//DPLL树中，结点的深度
int n_vars;											//变元总个数
int max_clause_len;									//最长单子句
double t;											//记录时间
int op = 2;											//算法选择策略

int main() {
	int option;
	while (1) {
		option = Display();
		if (option == 1)sat();
		else if (option == 2)sudoku();
		else if (option == 0) break;
		else cout << "请输入合法数字" << endl;
		if (option != 3) system("pause");
	}
	return 0;
}