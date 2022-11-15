
#include "INI\INI.h"





FINIFile::FINIFile(const std::string& FileName)
{
	ParseIni(FileName);
}

FINIFile::FINIFile(FILE* File)
{
	ParseIniFile(File);
}




namespace INI_Private
{

#define INI_MAX_LINE 2000
#define INI_INITIAL_ALLOC 200
#define MAX_SECTION 50
#define MAX_NAME 50
#define INI_START_COMMENT_PREFIXES ";#"
#define INI_INLINE_COMMENT_PREFIXES ";"

/* Strip whitespace chars off end of given string, in place. Return s. */
FORCEINLINE static char* rstrip(char* s)
{
	char* p = s + strlen(s);
	while( p > s && isspace((unsigned char)(*--p)) )
	{
		*p = '\0';
	}
	return s;
}

/* Return pointer to first non-whitespace char in given string. */
FORCEINLINE static char* lskip(const char* s)
{
	while( *s && isspace((unsigned char)(*s)) )
	{
		s++;
	}
	return (char*)s;
}

/* Return pointer to first char (of chars) or inline comment in given string,
   or pointer to null at end of string if neither found. Inline comment must
   be prefixed by a whitespace character to register as a comment. */
FORCEINLINE static char* find_chars_or_comment(const char* s, const char* chars)
{
	int was_space = 0;
	while( *s && (!chars || !strchr(chars, *s)) && !(was_space && strchr(INI_INLINE_COMMENT_PREFIXES, *s)) )
	{
		was_space = isspace((unsigned char)(*s));
		s++;
	}
	return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
FORCEINLINE static char* strncpy0(char* dest, const char* src, size_t size)
{
	strncpy_s(dest, size, src, size - 1);
	dest[size - 1] = '\0';
	return dest;
}

} // namespace INI_Private



int FINIFile::ParseIni(const std::string& FileName)
{
	FILE* LFile;
	int LError;

	fopen_s(&LFile, FileName.c_str(), "r");
	if( LFile == nullptr ) return -1;

	LError = ParseIniFile(LFile);
	fclose(LFile);

	return LError;
}

int FINIFile::ParseIniFile(FILE* File)
{
	/* Uses a fair bit of stack (use heap instead if you need to) */
	char* line;
	size_t max_line = INI_INITIAL_ALLOC;
	char* new_line;
	size_t offset;
	char section[MAX_SECTION] = "";
	char prev_name[MAX_NAME] = "";

	char* start;
	char* end;
	char* name;
	char* value;
	int lineno = 0;
	int error = 0;

	line = (char*)malloc(INI_INITIAL_ALLOC);
	if( !line )
	{
		return -2;
	}


	/* Scan through stream line by line */
	while( fgets(line, (int)max_line, File) != NULL )
	{
		offset = strlen(line);
		while( offset == max_line - 1 && line[offset - 1] != '\n' )
		{
			max_line *= 2;
			if( max_line > INI_MAX_LINE ) max_line = INI_MAX_LINE;
			new_line = (char*)realloc(line, max_line);
			if( !new_line )
			{
				free(line);
				return -2;
			}
			line = new_line;
			if( fgets(line + offset, (int)(max_line - offset), File) == NULL ) break;
			if( max_line >= INI_MAX_LINE ) break;
			offset += strlen(line + offset);
		}

		lineno++;

		start = line;
		if( lineno == 1 && (unsigned char)start[0] == 0xEF && (unsigned char)start[1] == 0xBB && (unsigned char)start[2] == 0xBF )
		{
			start += 3;
		}
		start = INI_Private::lskip(INI_Private::rstrip(start));

		if( strchr(INI_START_COMMENT_PREFIXES, *start) )
		{
			/* Start-of-line comment */
		}
		else if( *start == '[' )
		{
			/* A "[section]" line */
			end = INI_Private::find_chars_or_comment(start + 1, "]");
			if( *end == ']' )
			{
				*end = '\0';
				INI_Private::strncpy0(section, start + 1, sizeof(section));
				*prev_name = '\0';
			}
			else if( !error )
			{
				/* No ']' found on section line */
				error = lineno;
			}
		}
		else if( *start )
		{
			/* Not a comment, must be a name[=:]value pair */
			end = INI_Private::find_chars_or_comment(start, "=:");
			if( *end == '=' || *end == ':' )
			{
				*end = '\0';
				name = INI_Private::rstrip(start);
				value = end + 1;
				end = INI_Private::find_chars_or_comment(value, NULL);
				if( *end ) *end = '\0';
				value = INI_Private::lskip(value);
				INI_Private::rstrip(value);

				/* Valid name[=:]value pair found, call handler */
				INI_Private::strncpy0(prev_name, name, sizeof(prev_name));
				Values[section][name] = value;
			}
			else if( !error )
			{
				/* No '=' or ':' found on name[=:]value line */
				error = lineno;
			}
		}

		if( error ) break;
	}

	free(line);

	return error;
}