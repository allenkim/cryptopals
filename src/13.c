#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bytes.h"
#include "aes.h"
#include "oracles.h"
#include "hash_table.h"

int main(){
	char cookie[] = "foo=bar&baz=qux&zap=zazzle";
	ht_hash_table* ht = ht_new_parse(cookie);
	char* val = ht_search_str(ht, "foo");
	printf("%s\n", val);
	ht_del_hash_table(ht);
	return 0;
}

