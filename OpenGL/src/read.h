#pragma once
#include <string>
#include <vector>
std::vector< std::string > get_words(FILE* fp, char** orig_line)
{
#define BIG_STRING 4096
	static char str[BIG_STRING];
	static char str_copy[BIG_STRING];
	std::vector< std::string > words;
	int max_words = 10;
	int num_words = 0;
	char* ptr, * ptr2;
	char* result;

	/* read in a line */
	result = fgets(str, BIG_STRING, fp);
	if (result == NULL)
	{
		*orig_line = NULL;
		return words;
	}
	/* convert line-feed and tabs into spaces */
	/* (this guarentees that there will be a space before the */
	/*  null character at the end of the string) */

	str[BIG_STRING - 2] = ' ';
	str[BIG_STRING - 1] = '\0';

	for (ptr = str, ptr2 = str_copy; *ptr != '\0'; ptr++, ptr2++)
	{
		*ptr2 = *ptr;
		// Added line here to manage carriage returns
		if (*ptr == '\t' || *ptr == '\r')
		{
			*ptr = ' ';
			*ptr2 = ' ';
		}
		else if (*ptr == '\n')
		{
			*ptr = ' ';
			*ptr2 = '\0';
			break;
		}
	}

	/* find the words in the line */

	ptr = str;
	while (*ptr != '\0')
	{
		/* jump over leading spaces */
		while (*ptr == ' ') ptr++;

		/* break if we reach the end */
		if (*ptr == '\0') break;

		char* _ptr = ptr;

		/* jump over non-spaces */
		while (*ptr != ' ') ptr++;

		/* place a null character here to mark the end of the word */
		*ptr++ = '\0';

		/* save pointer to beginning of word */
		words.push_back(_ptr);
	}

	/* return the list of words */
	*orig_line = str_copy;
	return words;
}