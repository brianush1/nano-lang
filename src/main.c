#include <stdio.h>
#include <stdlib.h>

#define PROG_NAME "nanoc"

#include "tokens.h"

int main(int argc, char** argv)
{
	FILE* source;
	FILE* output;

	if (argc < 2)
	{
		fprintf(stderr, "%s: error: no input file specified\n", PROG_NAME);
		return 1;
	}
	else if (argc < 3)
	{
		fprintf(stderr, "%s: error: no output file specified\n", PROG_NAME);
		return 1;
	}
	
	source = fopen(argv[1], "r");
	output = fopen(argv[2], "w");

	fseek(source, 0, SEEK_END);
	size_t src_size = ftell(source);
	rewind(source);

	char* content = (char*)malloc(src_size + 1);
	fread(content, 1, src_size, source);
	content[src_size] = 0;

	lexer_t lexer = new_lexer();
	lexer->code = content;
	lexer->index = 0;

	token_t t = next(lexer);
	printf("%s (%d)\n", t.data, t.type);

	t = next(lexer);
	printf("%s (%d)\n", t.data, t.type);

	fclose(source);
	fclose(output);

	return 0;
}