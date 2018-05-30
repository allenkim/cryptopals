#include <math.h>
#include "prime.h"

bool is_prime(size_t n){
	if (n < 2 || n % 2 == 0)
		return false;
	if (n == 2)
		return true;
	size_t bound = sqrt(n);	
	for (size_t i = 3; i <= bound; i += 2){
		if (n % i == 0)
			return false;
	}
	return true;
}

size_t next_prime(size_t n){
	while (!is_prime(n++));
	return n-1;
}


