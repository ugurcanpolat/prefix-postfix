/* @Author
 * Student Name: Ugurcan Polat
 */

#include "functions.h"

using namespace std;

struct Queue q;
struct Stack s;

void emptyString (char **string) {
	int length = strlen(*string);
	
	for(int counter = 0; counter <= length; counter++) 
		(*string)[counter] = '\0';	
}

void copyString(char **dest, char *src) {
	if (*dest != NULL) {
		emptyString(dest);
		delete [] *dest;
	}
	
	int new_length = strlen(src) + 1;
	
	*dest = new char[new_length];
	
	strcpy(*dest, src);
}

void Queue::createQueue() {
	front = back = NULL;
}

void Queue::closeQueue() {
	Node *temp;

	while(front) {
		temp = front;
		front = front->next;
		delete [] temp->data;
		delete temp;
	}
}

void Queue::enqueue(char *newdata) {
	Node *newnode = new Node;
	newnode->data = NULL;
	copyString(&(newnode->data), newdata);
	newnode->next = NULL;

	if(!front) 
		front = back = newnode;

	else {
		back->next = newnode;
		back = newnode;
	}
}

char* Queue::dequeue() {
	if (front == NULL)
		return NULL;
	
	Node *topnode = front;
	char *temp = NULL;
	copyString(&temp, topnode->data);
	
	if (front == back)
		front = back = NULL;
	else
		front = front->next;

	delete [] topnode->data;
	delete topnode;
	return temp;
}

void Stack::createStack() {
	head = NULL;
}

void Stack::closeStack() {
	Node *temp;

	while(head) {
		temp = head;
		head = head->next;
		delete [] temp->data;
		delete temp;
	}
}

void Stack::push(char *newdata) {
	Node *newnode = new Node;
	newnode->data = NULL;
	
	copyString(&(newnode->data), newdata);
	newnode->next = head;
	head = newnode;	
}

char* Stack::pop() {
	if (head == NULL)
		return NULL;

	Node *topnode = head;
	char *temp = NULL;
	copyString(&temp, topnode->data);

	head = head->next;
	delete [] topnode->data;
	topnode->data = NULL;
	delete topnode;
	return temp;
}

void Functions::openInputFile(char *filename) {
	fp = fopen(filename, "r");
	
	if(!fp) {
		cout << endl << "Cannot open the input file.";
		return;
	}
}

void Functions::readFromInputFile() {
	char line[100];

	while(!feof(fp)) {
		char *postfix;
		fgets(line, 100, fp);
		
		// Get rid of carriage return and new line characters come 
		// from input file.
		for(int counter = 0; line[counter] != '\0'; counter++){
			if(line[counter] == '\r' || line[counter] == '\n')
				line[counter] = '\0';
		}

		postfix = convertToPostfix(line);
		
		// Only ecaluate postfix expression when there is one.
		if(postfix)
			evaluatePostfix(postfix);
		
		// Empty and delete postfix to get rid off waste information.	
		emptyString(&postfix);
		delete [] postfix;
	}
	
	// Close the file.
	fclose(fp);
}

int Functions::unaryBinaryPolynary(char *oprtr) {
	if (oprtr == NULL)
		return UNARY;
	
	if (!isOperator(oprtr) || strcmp(oprtr, "abs") == 0 ||
		strcmp(oprtr, "sqrt") == 0 ) {
		return UNARY;
	}
	
	else if (strcmp(oprtr, "sub") == 0 ||
			 strcmp(oprtr, "div") == 0) {
		return BINARY;
	}
	
	else
		return POLY;
}

void Functions::initialize() {
	s.createStack();
	q.createQueue();
}

void Functions::quit() {
	char *answer = q.dequeue();
	
	// If queue is not empty, print answers.
	if (answer != NULL)
		cout << endl << "Answer queue: " << answer;
	
	while(answer != NULL) {
		answer = q.dequeue();
		
		if (answer != NULL) {
			cout << " -> " << answer;
		}
	}
	
	cout << endl << endl;
	
	// Close stack and queue.
	s.closeStack();
	q.closeQueue();
}

char* Functions::convertToPostfix(char *prefix) {
	char *input[100], *oprnd, *expression, *cat, *postfix;
	int counter, check; 
	bool doesHaveParanthesis = false;
	
	// Split string into tokens (delimiter is space charachter).
	input[0] = strtok(prefix," ");
	for (counter = 0; input[counter] != NULL;) {
		counter++;
		input[counter] = strtok(NULL, " ");
	}
	
	// If the first token is not operator or the last token is not operand,
	// there is a syntax error.
	if(!isOperator(input[0]) || isOperator(input[counter-1])) {
		s.closeStack();
		cout << endl << "Syntax error." << endl;
		return NULL;
	} 
	
	// Begin reading from right to left.
	for (--counter; counter >= 0; counter--) {	
		// If the token comes after opening paranthesis is not operator,
		// there is a syntax error.
		if(strcmp(input[counter], "(") == 0) {
			if(!isOperator(input[counter+1])) {
				s.closeStack();
				cout << endl << "Syntax error." << endl;
				return NULL;
			}
		}
		
		// If the token is operator.
		else if(isOperator(input[counter])) {
	
			if (counter != 0) { // Avoid segmentation fault.
				if(strcmp(input[counter-1], "(") == 0)
					doesHaveParanthesis = true;
			}
			
			check = unaryBinaryPolynary(input[counter]);
			expression = new char[1000];
			// Get rid of waste informations.
			expression[0] = '\0';
			while (check > 0) {
				oprnd = s.pop();
				
				if (!oprnd)
					break;
						
				// If it reaches to closing paranthesis, the operation
				// is already done.
				if (strcmp(oprnd, ")") == 0)
					break;
				
				// Concatenate the expression and current operand.
				strcat(expression, oprnd);
				strcat(expression, " ");
				check--;
			}
			// Concatenate the expression and operator.
			strcat(expression, input[counter]);
			
			// If the operator have parantheses.
			if (doesHaveParanthesis) {
				// Pop the remaining paranthesis in the stack.
				if (check == 0)
					s.pop();
				
				// Concatenate the parantheses of the operation.
				cat = new char[100];
				strcpy(cat, "( ");
				strcat(cat, expression);
				strcpy(expression, cat);
				emptyString(&cat);
				delete [] cat;
				strcat(expression, " )");
				doesHaveParanthesis = false;
			}
			
			// Push expression to the stack.
			s.push(expression);
			// Since the operation is done, empty and delete the string.
			emptyString(&expression);
			delete [] expression;
		}
		
		// If it is operand, push it to the stack.
		else if (!isOperator(input[counter])) 
			s.push(input[counter]);
	}
	
	postfix = s.pop();
	
	char* pops;
	while ((pops = s.pop()) != NULL) {
		if (strcmp(pops, ")") != 0) {
			s.closeStack();
			cout << endl << "Syntax error." << endl;
			return NULL;			
		}
	}
	
	return postfix;	
}

void Functions::evaluatePostfix(char *postfix) {
	char *expression[100], *temp_postfix = NULL, *answer = NULL, *control;
	int counter, check, oprtr, oprtr_type;
	long oprnd1, oprnd2;
	bool doesHaveParanthesis = false;
	
	// Copy postfix before operations to print it later.
	copyString(&temp_postfix, postfix);
	// Begin splitting string into tokens (delimiter is space character).
	expression[0] = strtok(postfix," ");
	
	for(counter = 0; expression[counter] != NULL;) {
		counter++;
		expression[counter] = strtok(NULL, " ");
	}
	
	for(counter = 0; expression[counter] != NULL; counter++) {
		if(strcmp(expression[counter], ")") == 0){} // Skip ')' character
		
		// If it is operator.
		else if(isOperator(expression[counter])) {		
			   
			oprtr_type = unaryBinaryPolynary(expression[counter]);
			oprtr = whichOperator(expression[counter]);
			oprnd1 = 0;
			
			// If operator is product, initial value must be 1. 
			if(oprtr == PRODUCT)
				oprnd1 = 1;
			
			if (oprtr_type == UNARY || oprtr_type == BINARY)
				check = 1;
			else 
				check = POLY;
			
			if(expression[counter+1] != NULL) {	
				if(strcmp(expression[counter+1], ")") == 0)
					doesHaveParanthesis = true;
			}
			
			while (check > 0) {
				if(s.head != NULL)
					control = s.pop();
				else 
					break;
				
				// If it reaches to opening paranthesis, the calculation is already done.
				if(strcmp(control, "(") == 0) {
					doesHaveParanthesis = false;
					break;	
				}
				
				// Get operand.
				oprnd2 = checkOperand(control);
				
				// If operator is min or max, initial value must be the first operand.
				if((oprtr == MIN || oprtr == MAX) && check == POLY)
					oprnd1 = oprnd2;
				
				switch(oprtr) {
					case ABS: 
						// Special case due to infinity.
						if (oprnd2 == LONG_MIN)
							oprnd1 = LONG_MAX;
						else
							oprnd1 = labs(oprnd2); 
						break;
					case SQRT: 
						if (oprnd2 == LONG_MAX)
							oprnd1 = LONG_MAX;
						else if (oprnd2 < 0) {
							cout << endl << "Square root of a negative integer is undefined!" << endl;
							while (s.pop() != NULL);
							return;
						}
						else
							oprnd1 = lround(sqrtl(oprnd2));
						break;
					case SUB: 
						oprnd1 = checkOperand(s.pop());
						
						// Special cases due to infinity.
						if (oprnd1 == LONG_MAX && oprnd2 == LONG_MIN)
							oprnd1 = LONG_MAX;
						else if (oprnd1 == LONG_MIN && oprnd2 == LONG_MAX)
							oprnd1 = LONG_MIN;
						else if (oprnd1 == LONG_MAX && oprnd2 != LONG_MAX)
							oprnd1 = LONG_MAX;
						else if (oprnd1 == LONG_MIN && oprnd2 != LONG_MIN)
							oprnd1 = LONG_MIN;
						else if (oprnd1 != LONG_MAX && oprnd2 == LONG_MAX)
							oprnd1 = LONG_MIN;
						else if (oprnd1 != LONG_MIN && oprnd2 == LONG_MIN)
							oprnd1 = LONG_MAX;	
						// Subtraction.					
						else
							oprnd1 = oprnd1 - oprnd2;
						break;
					case DIV: 
						if (oprnd2 == 0) {
							cout << endl << "Dividing by 0 is undefined!" << endl;
							// Empty stack.
							while (s.pop() != NULL);
							return;
						}	
						
						oprnd1 = checkOperand(s.pop());
						
						// Special cases due to infinity.
						if (oprnd1 == LONG_MAX && oprnd2 == LONG_MAX)
							oprnd1 = 1;
						else if (oprnd1 == LONG_MAX && oprnd2 == LONG_MIN)
							oprnd1 = -1;
						else if (oprnd1 == LONG_MIN && oprnd2 == LONG_MAX)
							oprnd1 = -1;
						else if (oprnd1 == LONG_MIN && oprnd2 == LONG_MIN)
							oprnd1 = 1;
						else if (oprnd1 == LONG_MAX && oprnd2 > 0)
							oprnd1 = LONG_MAX;
						else if (oprnd1 == LONG_MAX && oprnd2 < 0)
							oprnd1 = LONG_MIN;
						else if (oprnd1 == LONG_MIN && oprnd2 > 0)
							oprnd1 = LONG_MIN;
						else if (oprnd1 == LONG_MIN && oprnd2 < 0)
							oprnd1 = LONG_MAX;		
						else if (oprnd2 == LONG_MAX || oprnd2 == LONG_MIN)
							oprnd1 = 0;
						// Division. Round the result if it is fractional.
						else
							oprnd1 = lround((double)oprnd1 / (double)oprnd2);
						break;
					case EXP: 
						// Since it is unknown if the operator is unary
						// or binary, pop one more data to check if 
						// it is operand or not. If it is operand, 
						// operator should be binary unless the next
						// operator is not unary.  
						control = s.pop();
						if (control != NULL) {
							if (!isOperator(control) && strcmp(control, "(") != 0) {
								oprtr_type = BINARY;
								oprnd1 = checkOperand(control);
							}
							else {
								if (strcmp(control, "(") != 0)
									s.push(control);
								oprtr_type = UNARY;
							}
							
							if(oprtr_type == UNARY) {
								// Special cases due to infinity.
								if (oprnd2 == LONG_MAX)
									oprnd1 = LONG_MAX;
								else if (oprnd2 == LONG_MIN)
									oprnd1 = 0;
								else
									oprnd1 = lround(exp(oprnd2));
							}
							
							// The operator is binary.
							else { 
								// Special cases due to infinity.
								if (oprnd1 == LONG_MAX && oprnd2 > 0) 
									oprnd1 = LONG_MAX;
								else if (oprnd1 == LONG_MAX && oprnd2 < 0)
									oprnd1 = 0;
								else if (oprnd2 == LONG_MIN)
									oprnd1 = 0;
								else if (oprnd2 == LONG_MAX)
									oprnd1 = LONG_MAX;
								else
									oprnd1 = pow(oprnd1, oprnd2);
							}
						}
							
						else {
							if (oprnd2 == LONG_MAX)
								oprnd1 = LONG_MAX;
							else if (oprnd2 == LONG_MIN)
								oprnd1 = 0;
							else
								oprnd1 = lround(exp(oprnd2));
						}
						break;
					case LOG:
						if (oprnd2 < 0) {
							cout << endl << "Logarithm of a negative integer is undefined!" << endl;
							while (s.pop() != NULL);
							return;
						}	
						
						// Since it is unknown if the operator is unary
						// or binary, pop one more data to check if 
						// it is operand or not. If it is operand, 
						// operator should be binary unless the next
						// operator is not unary.  
						control = s.pop();
						if (control != NULL) {
							if (!isOperator(control) && strcmp(control, "(") != 0) {
								oprtr_type = BINARY;
								oprnd1 = checkOperand(control);
							}
							else {
								if (strcmp(control, "(") != 0)
									s.push(control);
								oprtr_type = UNARY;
							}
							
							if (oprnd1 < 0 || oprnd2 == 0) {
								cout << endl << "For log x n; x must be greater than 0 and n cannot be 0" << endl;
								while (s.pop() != NULL);
								return;
							}
							
							if(oprtr_type == UNARY) {
								if (oprnd2 == LONG_MAX)
									oprnd1 = LONG_MAX;
								else
									oprnd1 = lround(log(oprnd2));
							}
							else {
								if (oprnd2 != LONG_MAX && oprnd1 == LONG_MAX)
									oprnd1 = 0;
								else if (oprnd2 == LONG_MAX && oprnd1 != LONG_MAX)
									oprnd1 = LONG_MAX;
								else
									oprnd1 = lround(log(oprnd2) / log(oprnd1)); 
							}
						}
						
						else {
							if (oprnd2 == LONG_MAX)
								oprnd1 = LONG_MAX;
							else
								oprnd1 = lround(log(oprnd2));
						}
		
						break;
					case MOD: 
						// Since it is unknown if the operator is unary
						// or binary, pop one more data to check if 
						// it is operand or not. If it is operand, 
						// operator should be binary unless the next
						// operator is not unary.  					
						control = s.pop();
						if (control != NULL) {
							if (!isOperator(control) && strcmp(control, "(") != 0) {
								oprtr_type = BINARY;
								oprnd1 = checkOperand(control);
							}
							else {
								if (strcmp(control, "(") != 0)
									s.push(control);
								oprtr_type = UNARY;
							}
										
							if(oprtr_type == UNARY) 
								oprnd1 = oprnd2 % 10;
							else {
								if (oprnd1 == 0) {
									cout << endl << "Modulo by 0 is undefined!" << endl;
									while (s.pop() != NULL);
									return;
								}								
								oprnd1 = oprnd2 % oprnd1;
							}
						}
						
						else 
							oprnd1 = oprnd2 % 10; 
				
						break;
					case SUM: 
						// Special cases due to infinity.
						if (oprnd1 == LONG_MAX && oprnd2 == LONG_MIN)
							oprnd1 = 0;
						else if (oprnd1 == LONG_MIN && oprnd2 == LONG_MAX)
							oprnd1 = 0;
						else if (oprnd1 == LONG_MAX && oprnd2 != LONG_MIN)
							oprnd1 = LONG_MAX;
						else if (oprnd1 == LONG_MIN && oprnd2 != LONG_MAX)
							oprnd1 = LONG_MIN;
						else if (oprnd2 == LONG_MAX)
							oprnd1 = LONG_MAX;
						else if(oprnd2 == LONG_MIN)
							oprnd1 = LONG_MIN;
						// Sum.
						else 
							oprnd1 += oprnd2;	
							
						break;
					case PRODUCT: 
						// Special cases due to infitiy.
						if (oprnd1 == LONG_MAX && oprnd2 < 0)
							oprnd1 = LONG_MIN;
						else if (oprnd1 == LONG_MIN && oprnd2 < 0)
							oprnd1 = LONG_MAX;
						else if (oprnd1 == LONG_MAX && oprnd2 > 0)
							oprnd1 = LONG_MAX;
						else if (oprnd1 == LONG_MIN && oprnd2 > 0)
							oprnd1 = LONG_MIN;
						else if (oprnd1 < 0 && oprnd2 == LONG_MAX)
							oprnd1 = LONG_MIN;
						else if (oprnd1 < 0 && oprnd2 == LONG_MIN)
							oprnd1 = LONG_MAX;
						else if (oprnd1 > 0 && oprnd2 == LONG_MAX)
							oprnd1 = LONG_MAX;
						else if (oprnd1 > 0 && oprnd2 == LONG_MIN)
							oprnd1 = LONG_MIN;	
						// Multiplication.	
						else
							oprnd1 *= oprnd2;
						break;
					case MIN: 
						if (oprnd2 < oprnd1)
							oprnd1 = oprnd2;
						break;
					case MAX: 
						if (oprnd2 > oprnd1)
							oprnd1 = oprnd2;					
						break;
					default: break;
				}  
				check--;
			}
			
			// If the operator have parentheses, pop the paranthesis.
			if(doesHaveParanthesis)
				s.pop();
			
			// If the result is inf or -inf push them to stack with string.
			if (oprnd1 == LONG_MAX) {
				char ans[4];
				strcpy(ans, "inf");
				s.push(ans); 
			}
			
			else if (oprnd1 == LONG_MIN) {
				char ans[5];
				strcpy(ans, "-inf");
				s.push(ans); 
			}
			
			// Push answer of the current operator to the stack.
			else 
				s.push(integerToString(oprnd1)); 
		}
		
		// If it is operand, push it to the stack.
		else if (!isOperator(expression[counter]))
			s.push(expression[counter]);
		
	}
	
	// The stack should have only one node, and it is the answer.
	answer = s.pop();
	
	// If the program is not in the batch mode, print answer and expression.
	if(fp == NULL) {
		cout << endl << "Postfix string: ";
		printPostfix(temp_postfix);
		cout << endl;
		cout << "Answer: " << answer << endl;
	}
	// Enqueue answer to queue.
	q.enqueue(answer);
}

char* Functions::integerToString(long integer) {
	long temp = integer;
	int num_digits = 0, counter;
	int is_negative = 0;
	char digit;
	
	// If the integer is negative.
	if (integer < 0) {
		is_negative = 1;
		integer *= -1; // Absolute.
		num_digits = 1;
	}
	
	// If the integer is 0.
	else if (integer == 0) {
		char *string = new char[2];
		string[0] = '0';
		string[1] = '\0';
		return string;
	}
		
	// Find how many digits the integer have.
	while (integer >= 1) {
		integer /= 10;
		num_digits++;
	}

	char *string = new char[num_digits+1];
	double del;
	for(counter = is_negative; counter < num_digits; counter++){
		// Get absolute.
		integer = labs(temp);
		del = integer / pow(10, num_digits-counter);
		integer = integer / pow(10, num_digits-counter);
		del -= integer;
		del *= 10;
		integer = del;
		// Convert digit to character.
		digit = (char)integer + '0';
		// Put that character to the string.
		string[counter] = digit;
	}
	
	// If it is a negative integer, put its sign.
	if (is_negative)
		string[0] = '-';
	
	// End string with null terminator.
	string[num_digits] = '\0';

	return string;
}

void Functions::printPostfix(char *postfix) {
	char *expression[100];
	
	// Print postfix expression with paranthesis.
	//cout << postfix;
	
	// Print without paranthesis.
	expression[0] = strtok(postfix," ()");
	
	for(int counter = 0; expression[counter] != NULL;) {
		cout << expression[counter] << " ";
		counter++;
		expression[counter] = strtok(NULL, " ()");
	}	
}

int Functions::whichOperator(char *string) {
	if (strcmp(string, "abs") == 0)
		return ABS;
	else if (strcmp(string, "sqrt") == 0)
		return SQRT;
	else if (strcmp(string, "sub") == 0)
		return SUB;
	else if (strcmp(string, "div") == 0)
		return DIV;
	else if (strcmp(string, "exp") == 0)
		return EXP;
	else if (strcmp(string, "log") == 0)
		return LOG;
	else if (strcmp(string, "mod") == 0)
		return MOD;
	else if (strcmp(string, "sum") == 0)
		return SUM;
	else if (strcmp(string, "product") == 0)
		return PRODUCT;
	else if (strcmp(string, "min") == 0)
		return MIN;
	else if (strcmp(string, "max") == 0)
		return MAX;
	else
		return ERROR;
}

long Functions::checkOperand(char *string) {
	long operand;
	
	if (strcmp(string, "ans") == 0) {
		if (q.back != NULL) 
			operand = checkOperand((q.back)->data);
		else 
			operand = 0;
	}
	
	else if (strcmp(string, "inf") == 0) 
		operand = LONG_MAX;
		
	else if (strcmp(string, "-inf") == 0)
		operand = LONG_MIN;
	
	else 
		operand = atol(string);
		
	return operand;
}

bool Functions::isOperator(char *check) {
	// They are strings, but they are operands.
	if(strcmp(check, "inf") == 0)
		return false;
	if(strcmp(check, "-inf") == 0)
		return false;
	if(strcmp(check, "ans") == 0)
		return false;
	if(strcmp(check, "(") == 0) 
		return false;
	if(strcmp(check, ")") == 0)
		return false;

	char *p;
	
	// Try to convert string to long integer. p will store end pointer.
	strtol(check, &p, 10);
		
	// If p is not NULL, then string should be operator.
	if (*p) 
		return true;	

	return false;
}
