#pragma once	//避免头文件重复引用
#define _CRT_SECURE_NO_WARNINGS 1	//忽略VS安全检查
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include<iostream>
#include <ctime>
#include<fstream>
using namespace std;

#define MAX_VARS 60000 //最大变元数
#define MAX_CLAUSES 230000 //最大子句数
#define FILENAMESIZE 400//文件名长度
#define ASSIGNED -2// 表示已赋值赋值
#define UNASSIGNED -1//表示还没有赋值
#define TRUE 1 
#define FALSE 0 
#define YES 1
#define NO 0
#define SATISFIED 1
#define SHRUNK 0	
#define SAT 1
#define UNSAT 0

typedef int status;
typedef struct literal_infomation {//文字结构
	int* literal_clause;//文字在公式中的位置
	int* literal_clause_pos;//文字在子句中的位置
	int n_number;//个数
	int is_assigned;//文字是否被赋值
	int is_in_unit_clause;//记录是否在单子句中出现 NO or YES
}literal_infomation;
extern literal_infomation literal_info[MAX_VARS][2];//文字存储数组
typedef struct clause_infomation {//子句结构
	int* literals;//子句中的文字
	int* literals_is_assigned;//1 for unassigned 0 for false
	int length_current;//现在的字句长度
	int length_original;//最初子句长度
	int is_clause_satisfied;//是否满足
	int unit_cluase_literal;//存储单子句文字中，未赋值文字
}clause_infomation;
typedef struct changes_infomation {//回溯信息
	int index_of_clause; //子句序号
	int index_of_literal;//文字序号
}changes_infomation;//子句满足时，记录子句序号。文字被设为假时，记录文字和字句序号
typedef struct assign_infomation {//记录赋值操作
	int value; //赋值类型UNASSIGNED TRUE FALSE
}assign_infomation;
typedef struct Sudoku {//数独结构
	int s[9][9];
	int n_current;
}Sudoku;


extern clause_infomation* clauses;//子句存储数组
extern int original_formula_length;//原长度
extern int current_formula_length;//现长度
extern changes_infomation changes_stack[8 * MAX_CLAUSES];//记录
extern unsigned int changes_stack_index;//数组changes的下标
extern unsigned int n_changes[MAX_VARS][2];//某一层满足或不满足的句子的个数
extern assign_infomation result[MAX_VARS];//用于回溯操作的记录
extern int is_contradicted;//是否用冲突的单子句
extern int conflicting_literal;//用于记录冲突的文字
extern int unit_clause_stack[MAX_CLAUSES];//单子句规则
extern int n_unit_clause_stack;//全局栈，用于记录栈顶位置
extern int depth;//DPLL树中，结点的深度
extern int n_vars;//变元总个数
extern int dpll_called;//统计DPLL执行次数
extern int sudokuterminal[10][10];//生成数独终盘
extern int max_clause_len;//最大字句长度
extern int op;//选择策略
extern double t;//时间



int Display();//展示选项
void printSudoku(Sudoku* s);//打印数独
int CNFparser(char* filename);//合取范式
void Preprocesser();//预处理器
int dpll(int op);//dpll算法
void Value(int v);//将文字v设定为已赋值
void UnValue(int v);//将文字设定为未赋值
inline int GetMinLenOfLiteral();//
inline int get_length_of();//计算最短字句长度
void get_weight(int x, int k, unsigned int& s, unsigned int& t);//获得权重
void SaveToRes(int value, double time, char* filename);//将结果输出到.res文件
void Free();//释放动态分配的空间
int createSudoku(int i, int j);//生成数组
Sudoku* DigHoleSudoku(Sudoku* S, int up_times, int purpose_num);//创建数独棋盘
int startsolveSudoku(Sudoku* s, int* count);//开始解决数独
int solveSudoku(Sudoku* s, int x, int y, int* count);//解决数独的递归过程
int checkkeyword(Sudoku* s, int x, int y);//检查x，y符合的数字
inline int Location(int x, int y, int z);//三维寻址
int ToCNFFile(Sudoku* s, char* filename);//将数独保存为CNF文件
Sudoku* ResultToSudoku();//检查数独结果，将res转为数度结果
void game(Sudoku* s, Sudoku* final_patern);//开始数独游戏
inline int GetFirstLiteral();//获得第一个文字
void sat();
void sudoku();
