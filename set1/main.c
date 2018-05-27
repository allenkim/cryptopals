#include <stdio.h>
#include <string.h>
#include "xor.h"
#include "challenge.h"

int main(int argc, char* argv[]){
	init_letter_freq();
	/*
	char* str = "Letter frequencies, like word frequencies, tend to vary, both by writer and by subject. One cannot write an essay about x-rays without using frequent Xs, and the essay will have an idiosyncratic letter frequency if the essay is about the frequent use of x-rays to treat zebras in Qatar. Different authors have habits which can be reflected in their use of letters. Hemingway's writing style, for example, is visibly different from Faulkner's. Letter, bigram, trigram, word frequencies, word length, and sentence length can be calculated for specific authors, and used to prove or disprove authorship of texts, even for authors whose styles are not so divergent.";
	size_t len = strlen(str);
	float s = score_plaintext((unsigned char*)str, len);
	printf("%f\n\n",s);
	str = "qeffeqfewhflwjeknrgwkmevpberbkdlbxchoxhbgqeiwtwyuohfdncjznvbpxbmpm";
	len = strlen(str);
	s = score_plaintext((unsigned char*)str, len);
	printf("%f\n\n",s);
	
	challenge_1();
	challenge_2();
	challenge_3();
	challenge_4();
	challenge_5();
	*/
	challenge_6();
	return 0;
}

