#pragma once	//����ͷ�ļ��ظ�����
#define _CRT_SECURE_NO_WARNINGS 1	//����VS��ȫ���
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include<iostream>
#include <ctime>
#include<fstream>
using namespace std;

#define MAX_VARS 60000 //����Ԫ��
#define MAX_CLAUSES 230000 //����Ӿ���
#define FILENAMESIZE 400//�ļ�������
#define ASSIGNED -2// ��ʾ�Ѹ�ֵ��ֵ
#define UNASSIGNED -1//��ʾ��û�и�ֵ
#define TRUE 1 
#define FALSE 0 
#define YES 1
#define NO 0
#define SATISFIED 1
#define SHRUNK 0	
#define SAT 1
#define UNSAT 0

typedef int status;
typedef struct literal_infomation {//���ֽṹ
	int* literal_clause;//�����ڹ�ʽ�е�λ��
	int* literal_clause_pos;//�������Ӿ��е�λ��
	int n_number;//����
	int is_assigned;//�����Ƿ񱻸�ֵ
	int is_in_unit_clause;//��¼�Ƿ��ڵ��Ӿ��г��� NO or YES
}literal_infomation;
extern literal_infomation literal_info[MAX_VARS][2];//���ִ洢����
typedef struct clause_infomation {//�Ӿ�ṹ
	int* literals;//�Ӿ��е�����
	int* literals_is_assigned;//1 for unassigned 0 for false
	int length_current;//���ڵ��־䳤��
	int length_original;//����Ӿ䳤��
	int is_clause_satisfied;//�Ƿ�����
	int unit_cluase_literal;//�洢���Ӿ������У�δ��ֵ����
}clause_infomation;
typedef struct changes_infomation {//������Ϣ
	int index_of_clause; //�Ӿ����
	int index_of_literal;//�������
}changes_infomation;//�Ӿ�����ʱ����¼�Ӿ���š����ֱ���Ϊ��ʱ����¼���ֺ��־����
typedef struct assign_infomation {//��¼��ֵ����
	int value; //��ֵ����UNASSIGNED TRUE FALSE
}assign_infomation;
typedef struct Sudoku {//�����ṹ
	int s[9][9];
	int n_current;
}Sudoku;


extern clause_infomation* clauses;//�Ӿ�洢����
extern int original_formula_length;//ԭ����
extern int current_formula_length;//�ֳ���
extern changes_infomation changes_stack[8 * MAX_CLAUSES];//��¼
extern unsigned int changes_stack_index;//����changes���±�
extern unsigned int n_changes[MAX_VARS][2];//ĳһ�����������ľ��ӵĸ���
extern assign_infomation result[MAX_VARS];//���ڻ��ݲ����ļ�¼
extern int is_contradicted;//�Ƿ��ó�ͻ�ĵ��Ӿ�
extern int conflicting_literal;//���ڼ�¼��ͻ������
extern int unit_clause_stack[MAX_CLAUSES];//���Ӿ����
extern int n_unit_clause_stack;//ȫ��ջ�����ڼ�¼ջ��λ��
extern int depth;//DPLL���У��������
extern int n_vars;//��Ԫ�ܸ���
extern int dpll_called;//ͳ��DPLLִ�д���
extern int sudokuterminal[10][10];//������������
extern int max_clause_len;//����־䳤��
extern int op;//ѡ�����
extern double t;//ʱ��



int Display();//չʾѡ��
void printSudoku(Sudoku* s);//��ӡ����
int CNFparser(char* filename);//��ȡ��ʽ
void Preprocesser();//Ԥ������
int dpll(int op);//dpll�㷨
void Value(int v);//������v�趨Ϊ�Ѹ�ֵ
void UnValue(int v);//�������趨Ϊδ��ֵ
inline int GetMinLenOfLiteral();//
inline int get_length_of();//��������־䳤��
void get_weight(int x, int k, unsigned int& s, unsigned int& t);//���Ȩ��
void SaveToRes(int value, double time, char* filename);//����������.res�ļ�
void Free();//�ͷŶ�̬����Ŀռ�
int createSudoku(int i, int j);//��������
Sudoku* DigHoleSudoku(Sudoku* S, int up_times, int purpose_num);//������������
int startsolveSudoku(Sudoku* s, int* count);//��ʼ�������
int solveSudoku(Sudoku* s, int x, int y, int* count);//��������ĵݹ����
int checkkeyword(Sudoku* s, int x, int y);//���x��y���ϵ�����
inline int Location(int x, int y, int z);//��άѰַ
int ToCNFFile(Sudoku* s, char* filename);//����������ΪCNF�ļ�
Sudoku* ResultToSudoku();//��������������resתΪ���Ƚ��
void game(Sudoku* s, Sudoku* final_patern);//��ʼ������Ϸ
inline int GetFirstLiteral();//��õ�һ������
void sat();
void sudoku();
