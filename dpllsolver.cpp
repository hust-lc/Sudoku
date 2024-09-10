#include "global.h"

void Value(int v) {//单字句处理
	register int i;
	register int p = abs(v), q = (v > 0) ? SATISFIED : SHRUNK;
	for (i = 0; i < literal_info[p][q].n_number; ++i) {
		register int j = literal_info[p][q].literal_clause[i];
		if (clauses[j].is_clause_satisfied) continue;//将j设为满足的
		clauses[j].is_clause_satisfied = YES;//设置为满足后，不改变句子长度，直到回溯后
		--current_formula_length;//更改子句的计数器
		changes_stack[changes_stack_index++].index_of_clause = j;//一个子句被设置为满足，那么记录这个子句的序号
		n_changes[depth][SATISFIED]++;//depth层的改变量++
	}
	q = !q;//对变元的负值进行处理
	for (i = 0; i < literal_info[p][q].n_number; ++i) {
		register int j = literal_info[p][q].literal_clause[i];//记录变元的字句序号
		if (clauses[j].is_clause_satisfied) continue;	//若该字句满足（已被标记），则跳过
		register int k = literal_info[p][q].literal_clause_pos[i];//记录文字在该字句中的位置
		--clauses[j].length_current;//改变字句长度
		clauses[j].literals_is_assigned[k] = ASSIGNED;//该文字已经被赋值
		changes_stack[changes_stack_index].index_of_clause = j;//记录改变的字句位置
		changes_stack[changes_stack_index++].index_of_literal = k;//改变的文字位置
		n_changes[depth][SHRUNK]++;	//该层该变量++
		//if (clauses[j].length_current == 0) is_empty_clause_conflict = 1;
		if (clauses[j].length_current == 1) {//检查字句长度，判断是否为单子句
			register int location = -1;
			for (int i_literal = 0; i_literal < clauses[j].length_original; i_literal++) {
				if (clauses[j].literals_is_assigned[i_literal] == UNASSIGNED) {//若该文字没有被标记
					location = i_literal;
					break;
				}
			}
			register int w = clauses[j].literals[location];//单子句中剩余的文字
			register int s = abs(w), t = (w > 0) ? SATISFIED : SHRUNK;	//标记该文字的正负
			if (literal_info[s][(!t)].is_in_unit_clause == 1) {//记录冲突
				is_contradicted = 1;			//标记冲突
				conflicting_literal = w;		//记录冲突的文字
			}
			else if (literal_info[s][t].is_in_unit_clause == 0) {//没有冲突,把这个单子句放入全局栈
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

void UnValue(int v) {//回溯，取消单字句处理
	register int i;
	register int p = abs(v), q = (v > 0) ? SATISFIED : SHRUNK;
	--depth;		//dpll中节点的深度自减
	while (n_changes[depth][SHRUNK]) {//还有没从stack中取出来的
		--n_changes[depth][SHRUNK];//
		register int j = changes_stack[--changes_stack_index].index_of_clause;	//栈中的字句
		register int k = changes_stack[changes_stack_index].index_of_literal;	//栈中的文字
		++clauses[j].length_current;//回复子句的长度
		if (clauses[j].length_current == 2) {//将被设置为在单子句中出现的文字设为未出现
			int s = abs(clauses[j].unit_cluase_literal);
			int t = (clauses[j].unit_cluase_literal > 0) ? SATISFIED : SHRUNK;
			literal_info[s][t].is_in_unit_clause = NO;	//没有在单子据中存在
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

inline int get_length_of() {		//寻找还未满足的字句中的最小长度（内联函数，提高效率）
	register int i, s, type, C, min = max_clause_len;
	if (min == 2) return min;//如果最长子句的长度为2，那么min同样为2
	for (i = 1; i <= n_vars; ++i) {
		if (result[i].value == UNASSIGNED) {	//若改变元没有赋值
			for (type = 0; type < 2; ++type) {	//type表示两种单字句的策略
				for (s = 0; s < literal_info[i][type].n_number; ++s) {
					C = literal_info[i][type].literal_clause[s];//每一个文字，在每一个句子中的位置
					if (!clauses[C].is_clause_satisfied &&			//若该字句还未满足，且长度小于min，则更新min
						clauses[C].length_current < min) {
						min = clauses[C].length_current;
						if (min == 2) return 2;						//有2则返回2，不用继续寻找
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
		c = literal_info[x][SATISFIED].literal_clause[j];//子句序号
		if (clauses[c].length_current == k)
			s += 1 - clauses[c].is_clause_satisfied;
	}
	for (j = 0; j < literal_info[x][SHRUNK].n_number; ++j) {// 子句序号
		c = literal_info[x][SHRUNK].literal_clause[j];
		if (clauses[c].length_current == k)
			t += 1 - clauses[c].is_clause_satisfied;
	}
}
inline int GetMinLenOfLiteral() {//优化策略			找出最短的字句，在其中找出一个合适的文字
	register unsigned int i, k;
	register unsigned int max = 0, r, s, t;
	register int u;
	k = get_length_of();//最短子句
	for (i = 1; i <= n_vars; ++i) {
		if (result[i].value == UNASSIGNED) {
			//k = get_length_of();//最短子句
			get_weight(i, k, s, t);//i 变元 k  长度
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

inline int GetFirstLiteral() {//优化前策略			找出第一个合法的文字
	for (int i = 0; i < current_formula_length; i++)
		if (clauses[i].is_clause_satisfied == 0 && clauses[i].length_current > 0) //找到第一个未被标记的字句
			for (int j = 0; j < clauses[i].length_current; j++)
				if (clauses[i].literals_is_assigned[j] == -1)		//找到该字句第一个未被赋值的文字
					return clauses[i].literals[j];
	return 0;
}

int dpll(int op) {//DPLL递归算法
	++dpll_called;//增加dpll调用次数
	int* local_stack = NULL;		//建立当前DPLL的栈
	register unsigned int n_local_stack = 0;		//当前dpll中栈的栈顶位置
	while (1) {		//处理单字句和存在的冲突
		if (is_contradicted) {//处理冲突
			int cl = abs(conflicting_literal);		//冲突的文字
			while (n_local_stack) {					//回溯，取消单字句设置
				UnValue(local_stack[--n_local_stack]);
				register int s = abs(local_stack[n_local_stack]);
				register int t = local_stack[n_local_stack] > 0 ? true : false;
				result[s].value = UNASSIGNED;
			}
			is_contradicted = false;	//处理完冲突，更新冲突标记
			free(local_stack);			//释放栈空间
			n_unit_clause_stack = 0;	//更新全局变量
			return 0;	//存在冲突，求解失败，返回0
		}
		else if (n_unit_clause_stack) {//处理单字句
			local_stack = (int*)realloc(local_stack, (n_local_stack + 1) * sizeof(int));
			register int implied_lit = unit_clause_stack[--n_unit_clause_stack];//将要进行单子句规则的文字
			local_stack[n_local_stack++] = implied_lit;		//将要进行单字句规则的文字放入当前dpll的栈
			result[abs(implied_lit)].value = implied_lit > 0 ? true : false;		//对结果进行赋值
			Value(implied_lit);
		}
		else break;
	}
	if (!current_formula_length) return 1;			//如果当前子句数为0,则返回1，求解成功
	register int v;									//变元选取策略
	if (op == 1)v = GetFirstLiteral();
	else v = GetMinLenOfLiteral();
	if (v == 0)return 0;
	result[abs(v)].value = v > 0 ? true : false;	//更新结果
	Value(v);										//对选取的变元v进行单字句处理
	if (dpll(op)) return 1;							//处理完后若满足，则返回1表示求解成功
	UnValue(v);										//回溯，取消单字句设置
	register int i, j, k, m;
	result[abs(v)].value = !result[abs(v)].value;	//对选取的变元v取反，更新result
	Value(-v);										//对-v进行尝试
	if (dpll(op)) return 1;							//若满足，返回1
	UnValue(-v);									//回溯
	result[abs(v)].value = UNASSIGNED;				//将该值标记为还未解决
	while (n_local_stack) {							//回溯，将该过程中的单字句处理都取消
		int z = local_stack[--n_local_stack];
		UnValue(z);
		result[abs(z)].value = UNASSIGNED;
	}
	free(local_stack);								//释放空间
	is_contradicted = false;						//更新标记冲突
	return 0;										//返回0表示求解失败
}
