#include <stdio.h>
#include <time.h>
#include <math.h>

#define MINNUM 3990000000
#define MAXNUM 4010000000

int modular_pow(unsigned long long base, unsigned long long exponent, unsigned long long modulus) //a^d mod n ófor  miller-rabin algorithm
    {
    unsigned long long result; // this function is the one Wikipedia provides , but modified in C
	if (modulus == 1)
		return 0;
    result = 1;
    base = base % modulus;
    while (exponent > 0){
        if (exponent % 2 == 1)
           result = (result * base)% modulus;
        exponent = exponent >> 1;
        base = (base * base)% modulus;}
    return result;
}


//START OF DETERMINISTIC ALGORITHM

int main ()
{
	unsigned int i, current ,count=0 , n ,tmp , prime , x , d ,a  ;
	int r, z , flag, composite;
	clock_t sttime1, endtime1 , sttime, endtime;
	sttime1 = clock(); //gets current time
	printf("\n");
	for (current=MINNUM+1 ;current<=MAXNUM; current+=2) //checks all odd numbers from MINNUM to MAXNUM
	{
		prime=1;
		if (current%3==0) //if the current number can be divided by 3 then it isn't prime
			prime=0;
		else  //if the current cannot be divided perfectly by 3 , it checks if all the other primes(smaller than the current's root) can divide the current
		{
			for(i=5;i*i<=current;i+=6)
			{
				if (current% i==0||current%(i+2)==0) //if the remainer of dividing the current number with one of the two "primes" that exist every six numbers is 0
				{
					prime = 0;//then it isn't prime
					break;//check next odd number
				}
			}
		}
		if (prime==1)
		{
			count++; //if there isn't a smaller odd that divides the current number perfetly , then it's prime
		}
	}
	printf ("Deterministic algorithm : %d primes \n",count);
	endtime1 = clock(); //gets current time for a second time
    	printf("\n");
    	printf("Time for algorithm 1: %.4f secs\n",((double)(endtime1-sttime1))/CLOCKS_PER_SEC); //prints the difference between starting and ending time

	sttime =  clock();

	//START OF MILLER-RABIN ALGORITHM
	count =0;
	for (n = MINNUM+1 ; n <= MAXNUM ; n+=2) //checks all odd numbers between MINNUM and MAXNUM
	{
		r=0;
		composite=0;//a flag for the "for" loop later
		tmp = n -1;
		while (tmp%2==0)
		{
			tmp=tmp/2;
			r++;//counts how many times tmp can be divided by 2
		}
		d=tmp; //sets d as the last value of tmp
		a=2;
		while (a) //for every a in {2,7,61)
		{
			x = modular_pow(a,d,n);//calls function to calculate pow(a,d) % n
			if ((x == 1) || (x == (n-1)))
			{
				if (a==2)
				{
					a=7;
					continue;//continues check with next a
				}
				else if (a==7)
				{
					a=61;
					continue;//continues check with next a
				}
				else
					break;//goes to next command
			}
			flag = 1;
			for(z = 1; z <= r-1; z++) //loop with r-1 repetitions unless (break)
			{
				x = modular_pow(x,2,n); //calls function to calculate pow(a,2)% n
				if (x==n-1)
					{
						if (a==2)
						{
							a=7;
							flag=2;
							break;//continues check with next a
						}
						else if (a==7)
						{
							a=61;
							flag=2;
							break;//continues check with next a
						}
						else if (a==61)
						{
							a=0;
							flag=2;
							break;//continues with next a ,otherwise count current as prime
						}
					}
			}
			if (flag==1)
			{
				composite=1;
				break;
			}
		}
		if (!composite)
			count++;
	}
	printf("\n");
	printf("Miller-Rabin :  %d primes \n", count);
	endtime = clock();
	printf("\n");
	printf("Time for algorithm 2: %.4f secs\n",((double)(endtime-sttime))/CLOCKS_PER_SEC);
	printf("\n");
	return 0;
}

//the way too many printf commands are only to make the program more comprehendable
