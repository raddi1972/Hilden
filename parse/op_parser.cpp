#include "op_parser.h"
static std::stack<Environment>envList; 
int curr_env_index = 0; 
int get_precedence(std::string tokval){
	if(tokval=="+" || tokval=="-"){
		return ADD_PREC; 
	}else if(tokval=="*" || tokval=="/"){
		return MUL_PREC;
	}else{
		return EQ_PREC;
	}
	return -1; 
}

float get_ans(float lhs , std::string op , float rhs){
	if(op=="+"){
		return lhs + rhs; 
	}else if(op=="*"){
		return lhs * rhs;
	}else if(op=="/"){
		return lhs / rhs; 
	}else if(op=="-"){
		return lhs - rhs;
	}else if(op ==">="){
		return lhs >= rhs;
	}else if(op =="<="){
		return lhs <= rhs; 
	}else if(op == "!="){
		return lhs != rhs; 
	}else if(op == "=="){
		return lhs == rhs; 
	}
	return -1.0; 
}

void create_environment(){
	Environment newenv; 
	newenv.level = envList.top().level + 1; 
	envList.push(newenv); 
}

void exit_environment(){
	envList.pop();
}

void init_variable(TokenType tok , std::string val){
	if(envList.top().var_map.find(tok.token_val)==envList.top().var_map.end()){
		perror("Variable already exists!"); 
		exit(-1); 
	}
	Variable *newvar = new Variable(); 
	newvar->name = tok.token_val; 
	newvar->val = val; 
	newvar->type = "float"; 
	envList.top().var_map.insert(std::pair<std::string , Variable *>(tok.token_val , newvar)); 
}

//Uses Shunting Yard Algorithm
float parse_binary(std::vector<TokenType>tList){
	std::vector<TokenType>output_q; 
	std::stack<TokenType>op_stack; 

	int prev_tok_number = 1000;
	for(int i = 0; i<tList.size(); i++){
		TokenType tmp = tList[i]; 
		int token_number = tmp.token_number; 
		std::string token_value = tmp.token_val;
		if(token_number==tok_hfloat || token_number==tok_id){
			if(token_number==tok_hfloat){
				output_q.push_back(tmp);
			}else if(token_number==tok_id){
				Environment currenv = envList.top(); 
				if(currenv.var_map.find(token_value)==currenv.var_map.end()){
					perror("Variable not initialized!"); 
					exit(-1); 
				}
				Variable *var = currenv.var_map.at(token_value); 
				TokenType literal; 
				literal.token_number = tok_hfloat; 
				literal.token_val = var->val; 
				output_q.push_back(literal); 
			}
		}else if(token_number==tok_operator){
			while(!op_stack.empty()){
				int prec_top = get_precedence(op_stack.top().token_val);
				int prec_curr_tok = get_precedence(token_value); 
				if(prec_curr_tok >= prec_top){
					break; 
				}
				TokenType top = op_stack.top(); 
				op_stack.pop(); 
				output_q.push_back(top);
			}
			op_stack.push(tmp); 
		}else if(token_number==tok_open_p){
			op_stack.push(tmp);
		}else if(token_number==tok_close_p){
			while(!op_stack.empty()){
				if(op_stack.top().token_val=="("){
					op_stack.pop(); 
					break; 
				}
				TokenType top = op_stack.top(); 
				op_stack.pop(); 
				output_q.push_back(top);
			}
		}
		if((prev_tok_number==tok_hfloat || prev_tok_number==tok_id) && (token_number==tok_hfloat || token_number==tok_id)){
			perror("Syntax Error!");
			exit(0);
		}else if(prev_tok_number==tok_operator && token_number==tok_operator){
			perror("Syntax Error!");
			exit(0);
		}else if(prev_tok_number==tok_open_p && token_number==tok_operator){
			perror("Syntax Error!");
			exit(0);
		}
		prev_tok_number = token_number; 
	}
	while(!op_stack.empty()){
		if(op_stack.top().token_number!=tok_open_p){
			output_q.push_back(op_stack.top());
			op_stack.pop();
		}
	}
	
	std::stack<float>parse_stack;
	for(int i = 0; i<output_q.size(); i++){
		TokenType tok = output_q[i]; 
		if(tok.token_number==tok_hfloat || tok.token_number==tok_id){
			parse_stack.push(std::stof(tok.token_val));
		}else if(tok.token_number == tok_operator){
			float rhs = parse_stack.top();
			parse_stack.pop();
			if(parse_stack.empty()){
				return rhs; 
			}
			float lhs = parse_stack.top();
			parse_stack.pop();
			float ans = get_ans(lhs , tok.token_val, rhs);
			parse_stack.push(ans); 
		}
	}
	return parse_stack.top();
}

void parse_expression(std::vector<TokenType>tList){
	Environment globalenv; 
	globalenv.level = 0; 
	envList.push(globalenv); 
	int ctr = 0; 
	while(ctr<tList.size()){
		TokenType curtok = tList[ctr]; 
		if(curtok.token_val=="hprint"){
			ctr++;
			TokenType nextok = tList[ctr]; 
			std::vector<TokenType>expr; 
			while(true){
				nextok = tList[ctr]; 
				if(nextok.token_number == tok_sep){
					break;
				}
				expr.push_back(nextok); 
				ctr++;
			}
			if(expr.size()==0){
				std::cout<<std::endl;
				return ;
			}
			float ans = parse_binary(expr); 
			std::cout<<ans<<std::endl;
			ctr++;
		}else if(curtok.token_val=="hfloat"){
			ctr++; 
			TokenType nextok = tList[ctr];
			std::string var_name = nextok.token_val; 
			ctr++; 
			TokenType next_next_tok = tList[ctr];
			if(next_next_tok.token_val=="="){
				std::vector<TokenType>expr;
				ctr++;
				while(nextok.token_val!=";"){
					nextok = tList[ctr];
					expr.push_back(nextok);
					if(nextok.token_val==";"){
						break; 
					}
					ctr++;
				}
				if(expr.size()==0){
					perror("Syntax Error! nothing after '=' "); 
					exit(-1);
				}
				float ans = parse_binary(expr); 
				Variable *newvar = new Variable();  
				std::ostringstream ss;
				ss << ans; 
				std::string val(ss.str());
				newvar->val = val;
				newvar->type = "float"; 
				newvar->name = var_name; 
				envList.top().var_map.insert(std::pair<std::string , Variable*>(var_name , newvar));
				ctr++; 
			}
		}else if(curtok.token_number==tok_id){
			ctr++;
			TokenType nextok = tList[ctr];
			if(nextok.token_val=="="){
				std::vector<TokenType>expr; 
				ctr++; 
				while(nextok.token_val!=";"){
					nextok = tList[ctr];
					expr.push_back(nextok);
					if(nextok.token_val==";"){
						break; 
					}
					ctr++;
				}
				float ans = parse_binary(expr); 
				std::ostringstream ss;
				ss << ans; 
				std::string val(ss.str());
				envList.top().var_map[curtok.token_val]->val = val; 
				ctr++;
			}
		}else if(curtok.token_number==tok_open_b){
			//to be FIXED
			// std::cout<<"in"<<std::endl;
			std::stack<int>paran;
			paran.push(1); 
			bool flag_last = false; 
			std::vector<TokenType>expr; 
			create_environment(); 
			while(true){
				if(paran.empty()){
					break;
				}
				TokenType nextok = tList[++ctr];
				if(nextok.token_number==tok_open_b){
					paran.push(1); 
				}else if(nextok.token_number==tok_close_b){
					if(!paran.empty()){
						if(paran.size()==1){
							flag_last = true;
						}
						paran.pop();
					}
				}
				if(!flag_last){
					expr.push_back(nextok);
				}
			}
			parse_expression(expr); 
			exit_environment(); 
			ctr++; 
		}else{
			perror("Syntax error!"); 
			exit(-1); 
		}
	}
}

