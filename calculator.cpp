#include "std_lib_facilities.h"

class Token {	// define a type called Token
public:	
	char kind;	// what kind of token
	double value;	// used for numbers

	// make a Token from a char
	Token(char ch){ kind=ch; value=0 ; }
	Token(char ch, double val){kind=ch; value=val;}
};



class Token_stream {
public:
	Token_stream();
	Token get();		// get a Token
	void putback(Token t); // put a Token back into the Token_stream
private:
	// representation: not directly accessible to users:
	bool full;	// is there a Token in the buffer?
	Token buffer;       // here is where we keep a Token put back using putback()

};
Token_stream::Token_stream()
:full(false), buffer(0)    // no Token in buffer
{
}

void Token_stream::putback(Token t)
{
	if (full) error("putback() into a full buffer");
	buffer=t;
	full=true;
}

Token Token_stream::get()	 // read a Token from the Token_stream
{
	if (full) { 
	full=false; 
	return buffer; }  // check if we already have a Token ready

	
    char ch;
    cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)


	switch (ch) {
	case '=':    // for "print"
        case 'x':    // for "quit"
        case '(':
	case ')': 
	case '+': 
	case '-': 
	case '*': 
	case '/': 
            return Token{ch};        // let each character represent itself
	case '.':
	case '0': case '1': case '2': case '3': case '4': 
	case '5': case '6': case '7': case '8': case '9':
	    {	cin.putback(ch);		// put digit back into the input stream
		double val;
		cin >> val;		// read a floating-point number
		return Token{'8',val};	// let ‘8’ represent “a number”
	    }
	default:
		error("Bad token");
	}

}
Token_stream ts;

double expression();
double primary()	// Number or ‘(‘ Expression ‘)’
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':			       // handle ‘(’expression ‘)’
	     {	double d = expression();
		t = ts.get();
		if (t.kind != ')') error("')' expected");
		return d;
	     }
	case '8':		// we use ‘8’ to represent the “kind” of a number
		return t.value;	// return the number’s value
	default:
		error("primary expected");
	}
}
double term()	// deal with * and  /
{
	double left = primary(); 
Token t = ts.get();	// get the next Token from input		
	while (true) {
		
		switch (t.kind) {	
		case '*':
			left *=primary();
			t= ts.get();
			break;
		case '/':
		{
			double d=primary();
			if(d==0) error("divide by zero");
			left /=d;
			t=ts.get();
			break;
		}
		default:
			ts.putback(t);	// put unused token back into input stream
			return left;
		}
	}
}

double expression()	// deal with + and -
{
	double left = term(); 	
Token t = ts.get();	     // get the next token from a “token stream”
	while (true) {
	
		switch (t.kind) {		
		case '+':
			left += term();
			t=ts.get();
			break;
		case '-':
			left -= term();
			t=ts.get();
			break;
		default:
			ts.putback(t);  // put the unused token back
			    return left;
		}
	}
}









int main()
try
{
    cout << "Welcome to our simple calculator.\n"
         << "Please enter expressions using floating-point numbers.\n"
         << "Operators available: {+, -, *, /}\n"
         << "Use '=' to print results.\n" 
         << "Press 'x' to quit.\n";

    double val = 0;

    while (cin) {
        Token t = ts.get();

        if (t.kind == 'x') {
            break; // 'x' for quit
        }

        if (t.kind == '=') {       // '=' for "print now" 
            cout << "=" << val << "\n";
        }

        else {
            ts.putback(t);
        }

        val = expression();
    }
	keep_window_open();
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n'; 
	keep_window_open();
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n"; 
	keep_window_open();
    return 2;
}
