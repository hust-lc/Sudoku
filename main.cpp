#include "global.h"

int dpll_called;									//ͳ��DPLL���ô�����ȫ�ֱ���
int sudokuterminal[10][10];							//��������
literal_infomation literal_info[MAX_VARS][2];		//���ִ洢����
clause_infomation* clauses;							//�Ӿ�洢����++++
int original_formula_length, current_formula_length;//ԭ���ȣ��ֳ���
changes_infomation changes_stack[8 * MAX_CLAUSES];	//��¼�ñ���
unsigned int changes_stack_index;					//����changes���±�
unsigned int n_changes[MAX_VARS][2];				//ĳһ�����������ľ��ӵĸ���
assign_infomation result[MAX_VARS];					//���ڻ��ݲ����ļ�¼
int is_contradicted;								//�Ƿ��ó�ͻ�ĵ��Ӿ�
int conflicting_literal;							//���ڼ�¼��ͻ������
int unit_clause_stack[MAX_CLAUSES];					//ȫ��ջ
int n_unit_clause_stack;							//���ڼ�¼ջ��λ��
int depth;											//DPLL���У��������
int n_vars;											//��Ԫ�ܸ���
int max_clause_len;									//����Ӿ�
double t;											//��¼ʱ��
int op = 2;											//�㷨ѡ�����

int main() {
	int option;
	while (1) {
		option = Display();
		if (option == 1)sat();
		else if (option == 2)sudoku();
		else if (option == 0) break;
		else cout << "������Ϸ�����" << endl;
		if (option != 3) system("pause");
	}
	return 0;
}