#include "global.h"

int CNFparser(char* filename) {
	//����ȫ�ֱ���
	dpll_called = 0;
	max_clause_len = 0;
	for (int i = 0; i < MAX_VARS; i++) {
		literal_info[i][SATISFIED].is_assigned = UNASSIGNED;	//û�б���ֵ 
		literal_info[i][SATISFIED].is_in_unit_clause = NO;		//û���ڵ��Ӿ��г���
		literal_info[i][SATISFIED].literal_clause = NULL;		//�ڹ�ʽ�е�λ��
		literal_info[i][SATISFIED].literal_clause_pos = NULL;	//���Ӿ��е�λ��
		literal_info[i][SATISFIED].n_number = 0;				//���ָ���Ϊ0
		literal_info[i][SHRUNK].is_assigned = UNASSIGNED;
		literal_info[i][SHRUNK].is_in_unit_clause = NO;
		literal_info[i][SHRUNK].literal_clause = NULL;
		literal_info[i][SHRUNK].literal_clause_pos = NULL;
		literal_info[i][SHRUNK].n_number = 0;
	}
	for (int i = 0; i < 8 * MAX_CLAUSES; i++) {			//�ı�����Ϊ0��δ����DPLL��
		changes_stack[i].index_of_clause = 0;
		changes_stack[i].index_of_literal = 0;
	}
	for (int i = 0; i < MAX_VARS; i++) {			//DPLLĳһ�����������ľ��Ӹ���
		n_changes[i][0] = 0;
		n_changes[i][1] = 0;
	}
	changes_stack_index = 0;
	for (int i = 0; i < MAX_VARS; i++) {			//��Ԫ��δ��ֵ
		result[i].value = UNASSIGNED;			
	}
	is_contradicted = 0;//�Ƿ��ó�ͻ�ĵ��Ӿ�
	conflicting_literal = 0;//���ڼ�¼��ͻ������
	n_unit_clause_stack = 0;//ȫ��ջ�����ڼ�¼ջ��λ��
	depth = 0;//DPLL���У��������
	ifstream inFile;			//���ļ�
	inFile.open(filename);
	if (!inFile.is_open()) {
		cout << "�ļ���ʧ��" << endl;
		exit(EXIT_FAILURE);
	}
	char c;
	char cnf[5];
	int x = 1;
	while (c = inFile.get() != 'p');	//����ע�Ͳ���
	inFile.get();	//�����з�
	inFile >> cnf >> n_vars >> original_formula_length;	//�����Ԫ�����־���
	inFile.get();	//�����з�
	if (strcmp(cnf, "cnf"))return 0;	//�ж��ļ���ʽ
	current_formula_length = original_formula_length;	//��ʼ��
	clauses = (clause_infomation*)malloc(sizeof(clause_infomation) * original_formula_length);//�����־伯
	for (int i = 0; i < original_formula_length; i++) {
		clauses[i].literals = NULL;				//�־����ĸΪ��
		clauses[i].length_current = 0;			//�־���ĸ������Ϊ0
		clauses[i].is_clause_satisfied = 0;	//��ʼΪ������
		clauses[i].literals_is_assigned = NULL;	//��ʼû�б�����
		clauses[i].unit_cluase_literal = 0;		//��ʼû���ڵ��־����
		while (1) {
			//if(!(inFile >> x))goto file_is_finish;
			inFile >> x;
			if (x == 0) break;
			clauses[i].literals = (int*)realloc(clauses[i].literals, (++clauses[i].length_current) * (sizeof(int))); //����ÿ���־������
			clauses[i].literals[clauses[i].length_current - 1] = x;		//��ʼ������
			clauses[i].literals_is_assigned = (int*)realloc(clauses[i].literals_is_assigned, (clauses[i].length_current) * (sizeof(int)));;		//��������ռ�
			clauses[i].literals_is_assigned[clauses[i].length_current - 1] = UNASSIGNED;		//��Ԫ�Ĳ���ֵδ֪,��ʼ��Ϊ-1
			int t = x > 0 ? SATISFIED : SHRUNK;		//��ֵ/��ֵ���Խ���һ�����ֽṹ
			int s = abs(x);

			++literal_info[s][t].n_number;					//��Ԫx������++
			literal_info[s][t].literal_clause = (int*)realloc(literal_info[s][t].literal_clause, literal_info[s][t].n_number * (sizeof(int)));			//�����ڹ�ʽ�еĵ�i���־�
			literal_info[s][t].literal_clause[literal_info[s][t].n_number - 1] = i;
			literal_info[s][t].literal_clause_pos = (int*)realloc(literal_info[s][t].literal_clause_pos, literal_info[s][t].n_number * (sizeof(int)));	//�������Ӿ��е�λ��
			literal_info[s][t].literal_clause_pos[literal_info[s][t].n_number - 1] = clauses[i].length_current - 1;										//�����ڸ��־��еĵڼ���λ��
		}
		clauses[i].length_original = clauses[i].length_current;		//��ǰ���ȵ��ڳ�ʼ����
		if (clauses[i].length_original > max_clause_len) {
			max_clause_len = clauses[i].length_original;		//����ȫ�ֱ���
		}
	}
	//file_is_finish:
	//fclose(fp);
	inFile.close();
	return 1;
}

void Preprocesser() {//Ԥ���������Ӿ����ȫ��ջ
	for (int i = 0; i < original_formula_length; i++)
		if (clauses[i].length_original == 1) //������Ϊһ�ı�������ȫ��ջ  Ҳ���ǽ����Ӿ����ȫ��ջ
			unit_clause_stack[n_unit_clause_stack++] = clauses[i].literals[0];		//���ֵ�ֵ
}

int Display() {//����ѡ�������ʾ
	system("cls");
	cout<<"ѡ��һ��ѡ��"<<endl;
	cout<<"1 SAT"<<endl;
	cout<<"2 Sudoku"<<endl;
	cout<<"0 Exit"<<endl;
	int a;
	cin >> a;
	while (!(a == 0 || a == 1 || a == 2)) {
		cout << "������Ϸ�����" << endl;
		cin >> a;
	}
	char tmp;
	while ((tmp = getchar()) != EOF && tmp != '\n');//������뻺����
	return a;
}

void SaveToRes(int value, double time, char* filename) {//��������뵽res�ļ�
	int len = strlen(filename);
	filename[len - 3] = 'r';
	filename[len - 2] = 'e';
	filename[len - 1] = 's';
	ofstream outFile;
	outFile.open(filename);
	if (!outFile.is_open()) {
		cout << "�������Ϊres�ļ�ʧ��" << endl;
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

void Free() {//�ͷŷ�����ڴ�
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
	cout << "�������ļ���" << endl;
	cin.getline(filename, FILENAMESIZE);
	int cnf = CNFparser(filename);									//�����µ��ڲ��ṹ
	if (cnf == 0) {
		cout << "�ļ���ʽ����ȷ" << endl;
		exit(EXIT_FAILURE);
	}
	else cout << "�ļ���ȡ�ɹ�" << endl;
	cout << "��ѡ���㷨������ 1 �� 2" << endl << "1.��ͨ�㷨   2.�Ż��㷨" << endl;
	cin >> op;
	while (!(op == 1 || op == 2)) {
		cout << "������Ϸ�����" << endl;
		cin >> op;
	}
	clock_t starttime, endtime;
	cout << "��ʼ���" << endl << "Load������" << endl;
	starttime = clock();
	Preprocesser();													//Ԥ���������Ӿ����ȫ��ջ
	int value = dpll(op);											//value��ʾ����Ƿ�ɹ�
	endtime = clock();
	t = (double)(endtime - starttime) * 1000 / CLOCKS_PER_SEC;		//ʱ����msΪ��λ
	SaveToRes(value, t, filename);									//�����������ļ�
	if (value == 1)cout << "sat�������ڣ���ʱ" << t << "ms" << endl << "�ѽ��������Ϊres�ļ�" << endl;
	else if (value == 0)cout << "sat����ⲻ���ڣ���ʱ" << t << "ms" << endl << "�ѽ��������Ϊres�ļ�" << endl;
	Free();															//�ͷ�malloc����Ŀռ�
}