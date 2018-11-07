#pragma once
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

const char* keywords[] = {
	"return"
};


typedef enum
{
	keyword,
	identifier,
	integer,
	number
} token_type_t;

typedef struct
{
	token_type_t type;
	char* data;
} token_t;

typedef struct
{
	size_t index;
	char* code;
} _lexer_struct, *lexer_t;

token_t _read_while(lexer_t state, bool (*predicate)(char, void*), void* pstate)
{
	token_t result;

	char* place = state->code + state->index;
	int size = 0;

	while (predicate(*place, pstate))
	{
		size++;
		place++;
	}

	result.data = (char*)malloc(size + 1);
	result.data[size] = 0;

	place = state->code + state->index;
	for (int i = 0; i < size; ++i)
	{
		result.data[i] = place[i];
	}

	state->index += size;
	return result;
}

bool _is_digit(char c, void* has_decimal)
{
	return (c >= '0' && c <= '9') || (!*(bool*)has_decimal && c == '.' && (*(bool*)has_decimal = true));
}

bool _is_identifier(char c, void* state)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
}

token_t next(lexer_t state)
{
	token_t result;

	char* place = state->code + state->index;

	while (*place == ' ' || *place == '\t' || *place == '\n' || *place == '\r')
	{
		place++;
		state->index++;
	}

	uint64_t NIL = 0;
	bool has_decimal = false;
	if (_is_digit(*place, &NIL))
	{
		result = _read_while(state, _is_digit, &has_decimal);
		result.type = has_decimal ? number : integer;
	}
	else if (_is_identifier(*place, NULL))
	{
		result = _read_while(state, _is_identifier, NULL);
		result.type = identifier;

		for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i)
		{
			if (strcmp(result.data, keywords[i]) == 0)
			{
				result.type = keyword;
				break;
			}
		}
	}
	else
	{
		fprintf(stderr, "%s: error: invalid character\n", PROG_NAME);
		exit(2);
	}

	return result;
}

lexer_t new_lexer()
{
	return (lexer_t)malloc(sizeof(_lexer_struct));
}