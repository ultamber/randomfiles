#include <stdio.h>
#include <ctype.h>

int expr(char *);
int number(char *);
int term(char *);
int factor(char *);

enum error_enum {OK,INVALID_CHARACTER,MISSING_PARENTHESIS,UNEXPECTED_END_OF_INPUT}error; //different errors that might occur

int par_count=0; //counts the parentheses in the expression

int main(){
	int result,counter=0;
	char ch;
	while (ch!=EOF){
		error=OK;
		ch=getchar();
		if(ch=='\n'){//if there were no characters read
			printf("Error!No numbers read\n");
			continue;
		}
		result=expr(&ch);//if there are characters to be read
		if(par_count!=0&&error==OK)
			error=MISSING_PARENTHESIS;
		if (error==OK){ //After returning a result , according to the error's value , print either the result or the error message
			counter++;
			printf("Result %d is %d\n",counter,result );
		}
		else if(error==INVALID_CHARACTER)
			printf("Invalid character input\n");
		else if (error==MISSING_PARENTHESIS)
			printf("Missing parenthesis\n");
		else if (error==UNEXPECTED_END_OF_INPUT)
			printf("Invalid end of expression\n");
		while(ch!='\n') //ignore the rest of the line , in case there was an error
			ch=getchar();
	  par_count=0;
	}
	return 0;
}

int expr(char * ch){
	int val=0, sign=1;
	while(*ch!='\n'&&error==OK||error==UNEXPECTED_END_OF_INPUT){
	  while(isblank(*ch))//ignores all white space
      		*ch=getchar();
	  if(*ch=='\n')
		return val;
	  val+=sign*term(ch);
       	  if(error!=OK) //stops the expression in case an error is found
		return 0;
	  while(isblank(*ch))//ignores all white space after a term
		*ch=getchar();
	  if(*ch=='+'){
	    error=UNEXPECTED_END_OF_INPUT;//if the expression doesn't end unexpectedly , the error will change to OK
  	    sign=1;
    	    *ch=getchar();
	  }
	  else if(*ch=='-'){
	    error=UNEXPECTED_END_OF_INPUT;//if the expression doesn't end unexpectedly , the error will change to OK
    	    sign=-1;
	    *ch=getchar();
	  }
	  else if (*ch==')'){ //returns the result of the expression inside the parentheses
		par_count--;
		break;
  	  }
	  else if (*ch=='\n') //goes to main
		break;
	  else
	    error=INVALID_CHARACTER; //detected an invalid character
	}
  return val;
}

int term(char * ch){
  int val;
  val=1;
  if(*ch==')'){
    error=INVALID_CHARACTER;
    par_count=0;
    return 0;
  }
  val=factor(ch);
  while(isblank(*ch))//ignores all white space
    *ch=getchar();
  if(*ch=='*')
	while(*ch=='*'&&error==OK){ //multiplies all terms till another operator is found
	  *ch=getchar();
  	  val*=factor(ch);
	}
  return val;
}

int factor(char * ch){
	int val=0;
	while(isblank(*ch))
		*ch=getchar();
	if(*ch=='('){
		par_count++;
		while(isblank(*ch))//ignores all white space
			*ch=getchar();
		*ch=getchar();
		val=expr(ch); //calculates the value of the expression inside the parentheses
	  if(*ch==')'&&error==OK){//found a closing parenthesis and the expression was calculated without errors
	    error=OK;
	    *ch=getchar();
	    return val;
	  }
	  else{
    	   if(error==OK)
      	     error=MISSING_PARENTHESIS;//didnt found a closing parenthesis
	   return 0;
	  }
	}
	else
	  val=number(ch);
	if (error!=OK) //error checking , in case something went wrong in the number function
	  return 0;
	return val;
}

int number(char * ch) //function for (un)signed numbers
{
	int n,num=0,read_digit,sign=1;
	read_digit=0;
	error=OK;
	while(isblank(*ch))//ignores all white space
		*ch=getchar();
	if (*ch=='+'){//if the number is positive
		sign=sign;
		*ch=getchar();
	}
	else if (*ch=='-'){//if the number is negative
		sign=-1;
		*ch=getchar();
	}
	while(isdigit(*ch)){ //takes all digits and makes the number
		read_digit = 1;
		int n=*ch-'0';
		num=num*10+n;
		*ch=getchar();
	}
	if(read_digit)//returns the signed number
		return sign*num;
	else{//if there wasnt an number after the signs ,returns an error
		error=INVALID_CHARACTER;
		return 0;
	}
}
