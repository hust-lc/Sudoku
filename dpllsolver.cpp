#include "global.h"

void Value(int v) {//���־䴦��
	register int i;
	register int p = abs(v), q = (v > 0) ? SATISFIED : SHRUNK;
	for (i = 0; i < literal_info[p][q].n_number; ++i) {
		register int j = literal_info[p][q].literal_clause[i];
		if (clauses[j].is_clause_satisfied) continue;//��j��Ϊ�����
		clauses[j].is_clause_satisfied = YES;//����Ϊ����󣬲��ı���ӳ��ȣ�ֱ�����ݺ�
		--current_formula_length;//�����Ӿ�ļ�����
		changes_stack[changes_stack_index++].index_of_clause = j;//һ���Ӿ䱻����Ϊ���㣬��ô��¼����Ӿ�����
		n_changes[depth][SATISFIED]++;//depth��ĸı���++
	}
	q = !q;//�Ա�Ԫ�ĸ�ֵ���д���
	for (i = 0; i < literal_info[p][q].n_number; ++i) {
		register int j = literal_info[p][q].literal_clause[i];//��¼��Ԫ���־����
		if (clauses[j].is_clause_satisfied) continue;	//�����־����㣨�ѱ���ǣ���������
		register int k = literal_info[p][q].literal_clause_pos[i];//��¼�����ڸ��־��е�λ��
		--clauses[j].length_current;//�ı��־䳤��
		clauses[j].literals_is_assigned[k] = ASSIGNED;//�������Ѿ�����ֵ
		changes_stack[changes_stack_index].index_of_clause = j;//��¼�ı���־�λ��
		changes_stack[changes_stack_index++].index_of_literal = k;//�ı������λ��
		n_changes[depth][SHRUNK]++;	//�ò�ñ���++
		//if (clauses[j].length_current == 0) is_empty_clause_conflict = 1;
		if (clauses[j].length_current == 1) {//����־䳤�ȣ��ж��Ƿ�Ϊ���Ӿ�
			register int location = -1;
			for (int i_literal = 0; i_literal < clauses[j].length_original; i_literal++) {
				if (clauses[j].literals_is_assigned[i_literal] == UNASSIGNED) {//��������û�б����
					location = i_literal;
					break;
				}
			}
			register int w = clauses[j].literals[location];//���Ӿ���ʣ�������
			register int s = abs(w), t = (w > 0) ? SATISFIED : SHRUNK;	//��Ǹ����ֵ�����
			if (literal_info[s][(!t)].is_in_unit_clause == 1) {//��¼��ͻ
				is_contradicted = 1;			//��ǳ�ͻ
				conflicting_literal = w;		//��¼��ͻ������
			}
			else if (literal_info[s][t].is_in_unit_clause == 0) {//û�г�ͻ,��������Ӿ����ȫ��ջ
				unit_clause_stack[n_unit_clause_stack] = clauses[j].unit_cluase_literal = w;
				literal_info[s][t].is_in_unit_clause = YES;
				++n_unit_clause_stack;
			}
		}
	}
	literal_info[p][SHRUNK].is_assigned = YES;
	literal_info[p][SATISFIED].is_assigned = YES;
	++depth;
}

void UnValue(int v) {//���ݣ�ȡ�����־䴦��
	register int i;
	register int p = abs(v), q = (v > 0) ? SATISFIED : SHRUNK;
	--depth;		//dpll�нڵ������Լ�
	while (n_changes[depth][SHRUNK]) {//����û��stack��ȡ������
		--n_changes[depth][SHRUNK];//
		register int j = changes_stack[--changes_stack_index].index_of_clause;	//ջ�е��־�
		register int k = changes_stack[changes_stack_index].index_of_literal;	//ջ�е�����
		++clauses[j].length_current;//�ظ��Ӿ�ĳ���
		if (clauses[j].length_current == 2) {//��������Ϊ�ڵ��Ӿ��г��ֵ�������Ϊδ����
			int s = abs(clauses[j].unit_cluase_literal);
			int t = (clauses[j].unit_cluase_literal > 0) ? SATISFIED : SHRUNK;
			literal_info[s][t].is_in_unit_clause = NO;	//û���ڵ��Ӿ��д���
			clauses[j].unit_cluase_literal = 0;
		}
		clauses[j].literals_is_assigned[k] = UNASSIGNED;////
	}
	while (n_changes[depth][SATISFIED]) {
		--n_changes[depth][SATISFIED];
		register int j = changes_stack[--changes_stack_index].index_of_clause;
		clauses[j].is_clause_satisfied = NO;
		++current_formula_length;
	}
	literal_info[p][SATISFIED].is_assigned = NO;
	literal_info[p][SHRUNK].is_assigned = NO;
}

inline int get_length_of() {		//Ѱ�һ�δ������־��е���С���ȣ��������������Ч�ʣ�
	register int i, s, type, C, min = max_clause_len;
	if (min == 2) return min;//�����Ӿ�ĳ���Ϊ2����ôminͬ��Ϊ2
	for (i = 1; i <= n_vars; ++i) {
		if (result[i].value == UNASSIGNED) {	//���ı�Ԫû�и�ֵ
			for (type = 0; type < 2; ++type) {	//type��ʾ���ֵ��־�Ĳ���
				for (s = 0; s < literal_info[i][type].n_number; ++s) {
					C = literal_info[i][type].literal_clause[s];//ÿһ�����֣���ÿһ�������е�λ��
					if (!clauses[C].is_clause_satisfied &&			//�����־仹δ���㣬�ҳ���С��min�������min
						clauses[C].length_current < min) {
						min = clauses[C].length_current;
						if (min == 2) return 2;						//��2�򷵻�2�����ü���Ѱ��
					}
				}
			}
		}
	}
	return min;
}
void get_weight(int x, int k, unsigned int& s, unsigned int& t) {
	register int j, c;
	s = t = 0;
	for (j = 0; j < literal_info[x][SATISFIED].n_number; ++j) {
		c = literal_info[x][SATISFIED].literal_clause[j];//�Ӿ����
		if (clauses[c].length_current == k)
			s += 1 - clauses[c].is_clause_satisfied;
	}
	for (j = 0; j < literal_info[x][SHRUNK].n_number; ++j) {// �Ӿ����
		c = literal_info[x][SHRUNK].literal_clause[j];
		if (clauses[c].length_current == k)
			t += 1 - clauses[c].is_clause_satisfied;
	}
}
inline int GetMinLenOfLiteral() {//�Ż�����			�ҳ���̵��־䣬�������ҳ�һ�����ʵ�����
	register unsigned int i, k;
	register unsigned int max = 0, r, s, t;
	register int u;
	k = get_length_of();//����Ӿ�
	for (i = 1; i <= n_vars; ++i) {
		if (result[i].value == UNASSIGNED) {
			//k = get_length_of();//����Ӿ�
			get_weight(i, k, s, t);//i ��Ԫ k  ����
			r = (s + 1) * (t + 1);
			if (r > max) {
				max = r;
				if (s >= t) u = i;
				else u = -(int)i;
			}
		}
	}
	return u;
}

inline int GetFirstLiteral() {//�Ż�ǰ����			�ҳ���һ���Ϸ�������
	for (int i = 0; i < current_formula_length; i++)
		if (clauses[i].is_clause_satisfied == 0 && clauses[i].length_current > 0) //�ҵ���һ��δ����ǵ��־�
			for (int j = 0; j < clauses[i].length_current; j++)
				if (clauses[i].literals_is_assigned[j] == -1)		//�ҵ����־��һ��δ����ֵ������
					return clauses[i].literals[j];
	return 0;
}

int dpll(int op) {//DPLL�ݹ��㷨
	++dpll_called;//����dpll���ô���
	int* local_stack = NULL;		//������ǰDPLL��ջ
	register unsigned int n_local_stack = 0;		//��ǰdpll��ջ��ջ��λ��
	while (1) {		//�����־�ʹ��ڵĳ�ͻ
		if (is_contradicted) {//�����ͻ
			int cl = abs(conflicting_literal);		//��ͻ������
			while (n_local_stack) {					//���ݣ�ȡ�����־�����
				UnValue(local_stack[--n_local_stack]);
				register int s = abs(local_stack[n_local_stack]);
				register int t = local_stack[n_local_stack] > 0 ? true : false;
				result[s].value = UNASSIGNED;
			}
			is_contradicted = false;	//�������ͻ�����³�ͻ���
			free(local_stack);			//�ͷ�ջ�ռ�
			n_unit_clause_stack = 0;	//����ȫ�ֱ���
			return 0;	//���ڳ�ͻ�����ʧ�ܣ�����0
		}
		else if (n_unit_clause_stack) {//�����־�
			local_stack = (int*)realloc(local_stack, (n_local_stack + 1) * sizeof(int));
			register int implied_lit = unit_clause_stack[--n_unit_clause_stack];//��Ҫ���е��Ӿ���������
			local_stack[n_local_stack++] = implied_lit;		//��Ҫ���е��־��������ַ��뵱ǰdpll��ջ
			result[abs(implied_lit)].value = implied_lit > 0 ? true : false;		//�Խ�����и�ֵ
			Value(implied_lit);
		}
		else break;
	}
	if (!current_formula_length) return 1;			//�����ǰ�Ӿ���Ϊ0,�򷵻�1�����ɹ�
	register int v;									//��Ԫѡȡ����
	if (op == 1)v = GetFirstLiteral();
	else v = GetMinLenOfLiteral();
	if (v == 0)return 0;
	result[abs(v)].value = v > 0 ? true : false;	//���½��
	Value(v);										//��ѡȡ�ı�Ԫv���е��־䴦��
	if (dpll(op)) return 1;							//������������㣬�򷵻�1��ʾ���ɹ�
	UnValue(v);										//���ݣ�ȡ�����־�����
	register int i, j, k, m;
	result[abs(v)].value = !result[abs(v)].value;	//��ѡȡ�ı�Ԫvȡ��������result
	Value(-v);										//��-v���г���
	if (dpll(op)) return 1;							//�����㣬����1
	UnValue(-v);									//����
	result[abs(v)].value = UNASSIGNED;				//����ֵ���Ϊ��δ���
	while (n_local_stack) {							//���ݣ����ù����еĵ��־䴦��ȡ��
		int z = local_stack[--n_local_stack];
		UnValue(z);
		result[abs(z)].value = UNASSIGNED;
	}
	free(local_stack);								//�ͷſռ�
	is_contradicted = false;						//���±�ǳ�ͻ
	return 0;										//����0��ʾ���ʧ��
}
