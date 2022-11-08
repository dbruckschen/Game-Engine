#include "common.h"

// returns the length of a string s
// \n is part of the length
long long StringLen(char *s) {
    char *c = s;

    while(*c++ != '\0')
        ;

    return c - s;
}

bool StringCmp(char *s1, char *s2) {
    int cnt = 0;

    while(*s1 == *s2 && *s1 != '\0' && *s2 != '\0' && *s1 != '\n' && *s2 != '\n') {
        cnt++;
        s1++;
        s2++;
    }

    if(cnt == 0) {
        return 0;
	}

    if(*s1 == *s2) {
        return 1;
	}
	else {
        return 0;
	}
}

void StringCpy(char *dst, char *src, size_t dst_len) {
	for(int i = 0; i < dst_len - 1; i++) {
		dst[i] = src[i];
	}

	dst[dst_len] = '\0';
}

bool CharBelongsToText(char c) {
	if((c >= 33) && (c <= 126)) {
		return true;
	}
	else {
		return false;
	}
}

bool CharBelongsToAlphabet(char c) {
	if(((c >= 97) && (c <= 122)) || ((c >= 65) && (c <= 90))) 
		return true;
	else
		return false;
}

int CharToInt(char *s) {
	int result = 0;

	long long str_len = StringLen(s) - 1;
	int base = (int)pow(10.0, (int)(str_len - 1));
	
	for(int i = 0; i < str_len; i++) {
		result += (s[i] - '0') * base;
		base /= 10;
	}

	return result;
}

void ReverseString(char *s, int len) {
	char *tmp_string = malloc(len);

	for(int i = 0; i < len; i++) {
		tmp_string[i] = s[i];
	}

	for(int i = 0; i < len; i++) {
		s[i] =  tmp_string[(len-1)-i];
	}

	free(tmp_string);
}

void IntToString(int i, char *buff, int buff_len) {
	int idx = 0;

	while(i != 0) {
		if(buff_len >= idx) {
			int n = i % 10;
			buff[idx] = n + '0';
		
			i = i/10;

			idx++;
		}
	}
	buff[idx] = '\0';

	int str_len = StringLen(buff);
	ReverseString(buff, str_len-1);
}
