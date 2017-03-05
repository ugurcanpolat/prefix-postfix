/* @Author
 * Student Name: Ugurcan Polat
 */

#include "functions.h"
#include "functions.cpp"

#define INPUT_SIZE 100

void reallocateString(char **);

struct Functions f;

int main(int argc, char **argv) {
	char *input_expression;
	
	// Initialize stack and queue.
	f.initialize();
	
	// If the program invoked with a file name paramater, run batch mode.
	if (argc > 1) {
		f.openInputFile(*(argv+1));
		if(f.fp != NULL) 
			f.readFromInputFile();
			
		f.quit();
		return 0;
	}
	
	// Run console mode if the program invoked without parameters.
	else
		cout << endl << "Initializing the interpreter..." << endl;

	do {
		char *postfix;
		input_expression = new char[INPUT_SIZE];
		cout << endl << ">> ";
		scanf("%[^\n]s", input_expression);

		if (strcmp(input_expression, "exit") == 0)
			break;

		// Dynamically allocate memory for input expression.
		reallocateString(&input_expression);
		// Convert to postfix.
		postfix = f.convertToPostfix(input_expression);
		
		// Only evaluate postfix string when there is one.
		if(postfix)
			f.evaluatePostfix(postfix);
		
		// Get rid of '\n' character from input buffer.
		while (getchar() != '\n');
		
		// Empty and delete string to get rid off waste information.
		emptyString(&input_expression);
		delete [] input_expression;
	} while (1);

	cout << endl << "Terminating the interpreter..." << endl;
	f.quit();
	return 0;
}

void reallocateString(char **string) {
	char *temp = new char[strlen(*string)+1];
	strcpy(temp, *string);
	delete [] *string;
	*string = new char[strlen(temp)+1];
	strcpy(*string, temp);
	delete [] temp;
}
