#include "../include/ini_parser.h"

/**
 * Converts a ASCII hex digit (0 - f or 0 - F) to a binary value.
 * @param ASCII character
 * @return converted charecter; -1 on failure
 */
char hex_digit_to_binary(char c) {
	//convert ASCII to hex and return -1 on failure
	if(c >= '0' && c <= '9')
		return c - '0';
	else if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return -1;
}

/**
 * Converts unicode characters to UTF-8 characters.
 * Note: This uses the UTF-8 standard since 2003.
 * @param unicode character
 * @return UTF-8 character; -1 on failure
 */
uint32_t uctoutf8(uint32_t code) {
	//NOTE: this uses the UTF-8 standard since 2003
	unsigned char c[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	if(code <= 0x007F) {
		c[0] = (0x7f & code);
		c[1] = 0;
		c[2] = 0;
		c[3] = 0;
	}  else if(code <= 0x07FF) {
		c[0] = (0x80 | (0x3F & code));
		c[1] = (0xC0 | (0x1F & (code >> 6)));
		c[2] = 0;
		c[3] = 0;
	} else if(code <= 0xFFFF) {
		c[0] = (0x80 | (0x3F & code));
		c[1] = (0x80 | (0x3F & (code >> 6)));
		c[2] = (0xE0 | (0x0F & (code >> 12)));
		c[3] = 0;
	} else if(code <= 0x10FFFF) {
		c[0] = (0x80 | (0x3F & code));
		c[1] = (0x80 | (0x3F & (code >> 6)));
		c[2] = (0x80 | (0x3F & (code >> 12)));
		c[3] = (0xF0 | (0x07 & (code >> 18)));
	}
	return *((uint32_t*)c);
}

/**
 * Checks if a character is a spacing character (tab or space).
 * @param character
 * @return 1, if the character is a spacing character (0 otherwise)
 */
int is_spacing(char c) {
	if(c == '\t' || c == ' ')
		return 1;
	return 0;
}

/**
 * Checks if a character is a newline character (\r or \n).
 * @param character
 * @return 1, if the character is a newline character (0 otherwise)
 */
int is_newline(char c) {
	if(c == '\r' || c == '\n')
		return 1;
	return 0;
}

/**
 * Checks if a character is a whitespace character (spacing or newline).
 * @param character
 * @return 1, if the character is a whitespace character (0 otherwise)
 */
int is_whitespace(char c) {
	if(is_spacing(c) || is_newline(c))
		return 1;
	return 0;
}

/**
 * Checks if a character is a special character.
 * @param character
 * @param comment character
 * @param equals character
 * @return 1, if the character is a special character (0 otherwise)
 */
int is_special_character(char c, char comment_char, char equals_char) {
	if(c == '[' || c == ']' || c == '\\' || c == '\'' || c == '"' || c == comment_char || c == equals_char)
		return 1;
	return 0;
}

/**
 * Checks if a character is part of an UTF-8 sequence.
 * @param character
 * @return 1, if this is the case (0 otherwise)
 */
int is_utf8_part(char c) {
	if(c & 0x80)
		return 1;
	return 0;
}

/**
 * Checks if a character a printable character.
 * @deprecated
 * @param character
 * @return 1, if this is the case (0 otherwise)
 */
int is_printable_char(char c) {
	if(c > 32 || is_utf8_part(c) || is_whitespace(c))
		return 1;
	return 0;
}

/**
 * Returns the number of whitespaces on the left side of a string.
 * The input string must not be NULL!
 * @param string
 * @return number of whitespaces
 */
size_t lskipws(const char* str) {
	size_t i = 0;
	//skip whitespaces
	while(is_whitespace(str[i++]));
	//return index of the last whitespace character at the front
	return --i;
}

/**
 * Returns the number of whitespaces on the right side of a string.
 * The input string must not be NULL!
 * @param string
 * @return number of whitespaces
 */
size_t rskipws(const char* str) {
	size_t len = strlen(str);
	if(len == 0)
		return 0;
    int i = (int)len;
	//skip whitespaces from back to front
	while((is_whitespace(str[i]) || str[i] == '\0') && i >= 0)
		--i;
	//return index of first whitespace character at the end
	return ++i;
}

/**
 * Returns maximum of two size_t value.
 * @deprecated
 * @param first value
 * @param second value
 * @return maximum
 */
size_t max_size_t(size_t f, size_t s) {
	return ((f > s) ? f : s);
}

/*
  / **
  * Reads a hex character (e.g. 0xFE) from a string.
  * The input string must not be NULL and the position offset is expected to be in range!
  * @deprecated DO NOT USE THIS FUNCTION ANY MORE! THIS WILL BE REMOVED IN LATER RELEASES!
  * @param string
  * @param optional pointer to an offset
  * @return hex character; -1 on failure
  * /
  int read_hex_from_string(const char* str, size_t* pos) {
  size_t local_pos = 0;
  if(pos)
  local_pos = *pos;

  char ret = 0;
  size_t len = strlen(str);
  char c1, c2;

  if(len == local_pos + 1) {
  //only one character present
  c1 = str[local_pos++];

  //convert value to hex
  c1 = hex_digit_to_binary(c1);

  if(c1 < 0)
  return -1;

  ret = c1;
  } else if(len > local_pos + 1) {
  //at least two characters present
  c1 = str[local_pos++];
  c2 = str[local_pos++];
		
  //convert both values to hex
  c1 = hex_digit_to_binary(c1);
  c2 = hex_digit_to_binary(c2);

  if(c1 >= 0 && c2 >= 0) {
  //combine values if it is valid
  c1 = (c1 << 4);
  ret = (unsigned char)c1 | (unsigned char)c2;
  } else if(c1 >= 0) {
  //only first character is valid
  ret = (unsigned char)c1;
  } else {
  //exit if there isn't a hex value at given position
  return -1;
  }
  } else {
  //abort if the string is too short
  return -1;
  }

  if(pos) {
  *pos = local_pos;
  }
	
  return ret;
  }
*/
/**
 * Reads an unicode character from a string and converts it to an UTF-8 character.
 * The input string must not be NULL and the position offset is expected to be in range!
 * @param string
 * @param optional pointer to an offset
 * @return UTF-8 character; -1 on failure
 */
uint32_t read_unicode_from_string(const char* str, size_t* pos) {
	size_t local_pos = 0;
    if(pos)
		local_pos = *pos;
	
    uint32_t ret = 0;
    size_t len = strlen(str);
    unsigned int hex_length;

	if(local_pos + 6 > len)
		//string too short
		return -1;
	
    for(hex_length = 0; hex_length < 6 && hex_length < len; ++hex_length) {
		char current_char = str[local_pos + hex_length];
		char val = hex_digit_to_binary(current_char);
        if(val == -1)
			//invalid hex character and therfore invalid hex code
			return -1;
			
        ret |= ((unsigned char)val << ((5 - hex_length) * 4));
    }

	//add number of read digits
	local_pos += hex_length;

    ret = uctoutf8(ret);
    if(ret == (uint32_t)-1) {
		//invalid code
        return -1;
	}

	if(pos)
		*pos = local_pos;
	
	return ret;
}

/**
 * Checks if a string has a whitespace character before a newline.
 * The optional offset points to either the first non-whitespace character or the newline.
 * The input string must not be NULL and the position offset is expected to be in range!
 * @param string
 * @param optional pointer to offset
 * @return 1, if the string contains whitespaces before a newline (0 otherwise)
 */
int has_non_whitespace_before_newline(const char* str, size_t* pos) {
	size_t local_pos = 0;
	if(pos)
		local_pos = *pos;
    
    while(!is_newline(str[local_pos]) && str[local_pos] != '\0') {
        if(!is_whitespace(str[local_pos++])) {
			--local_pos;
			if(pos)
				*pos = local_pos;
            return 1;
        }
    }
	
	if(pos)
		*pos = local_pos;
    return 0;
}

/**
 * Replaces an escaped character with its counter part.
 * The comment and equals character are intended for user defined escaped characters
 * and return themselves.
 * Possible escaped characters: 'a', 'b', 'f', '[', ']', 'n', 'r', 't', 'v', ''', '"', '\', ' ', '\t', '\n', '\r', the comment character and the equals character
 * Notes: 
 *  - a whitespace after an '\' creates a newline
 *  - 'x' returns x (which is used to enter Unicode characters)
 * @param character
 * @param comment character
 * @param equals character
 * @return the unescaped counterpart or -1 on failure
 */
char unescape_char(char c, char comment, char equals) {
    char ret = -1;
    if(c == comment)
        return comment;
    else if(c == equals)
        return equals;
    switch(c) {
	case 'a':
		ret = '\a';
		break;
	case 'b':
		ret = '\b';
		break;
	case 'f':
		ret = '\f';
		break;
	case '[':
		ret = '[';
		break;
	case ']':
		ret = ']';
		break;
	case 'n':
		ret = '\n';
		break;
	case 'r':
		ret = '\r';
		break;
	case 't':
		ret = '\t';
		break;
	case 'v':
		ret = '\v';
		break;
	case '\'':
		ret = '\'';
		break;
	case '"':
		ret = '\"';
		break;
	case '\\':
		ret = '\\';
		break;
	case 'x':
		ret = 'x';
		break;
	case 'X':
		ret = 'x';
		break;
	case ' ':
	case '\t':
	case '\n':
	case '\r':
		ret = '\n';
		break;
    };
    return ret;
}

/**
 * Removes the whitespaces from the front and the back of a string. 
 * The input string must not be NULL!
 * @param string
 * @return string without whitespaces at the front or the back.
 */
char* stripws(char* str) {
    size_t offset_front = lskipws(str);
    size_t offset_back = rskipws(str);
    size_t len = offset_back - offset_front;
	if(offset_front >= offset_back)
		len = 0;
    memmove(str, str + offset_front, len);
    str[len] = '\0';
    return str;
}

/**
 * Converts a string with all its escaped sequences to an UTF-8 compatible string.
 * The string must not be NULL and the number of bytes to be copied is expected to be in range!
 * @param string
 * @param number of bytes to copy (has to include the zero-terminator!)
 * @param comment charcter
 * @param equals character
 * @return 1 on success; 0 on failure or if reserved characters are encountered
 */
int unescape_string(char* str, size_t len, char comment, char equals) {
	size_t src_index, dst_index = 0;
	
	//iterate through all characters
    for(src_index = 0; src_index < len; ++src_index) {
        char c = str[src_index];

		//check for escaped characters
        if(c == '\\') {
			if(++src_index >= len)
				//escaped sequence out of range
				return 0;
			
			//skip the '\'
			c = str[src_index];

			//convert to UTF-8
			char unesc = unescape_char(c, comment, equals);
			if(unesc == -1) {
				//illegal escaped character
				return 0;
			} else if(unesc == 'x') {
				//convert Unicode
				//skip 'x'
				++src_index;
				
				uint32_t hex_val = read_unicode_from_string(str, &src_index);

				//check if the src_index is still in range and if the character is valid
				if(src_index > len || hex_val == (uint32_t)-1)
					return 0;
				
				//copy only non-null bytes
				for(int k = 3; k >= 0; --k) {
					char current_byte = (0xFF & (hex_val >> (k * 8)));
					if(current_byte != 0)
						str[dst_index++] = current_byte;
				}
			} else if(unesc == '\n' && c != 'n') {
#ifndef INI_FORBID_MULTILINE
				if(has_non_whitespace_before_newline(str, &src_index))
					//"\\ "has to be followed by a newline
					return 0;
				
				//add newline to the string
				str[dst_index++] = '\n';
#else
				//newlines not allowed
				return 0;
#endif //INI_FORBID_MULTILINE
			} else {
				//copy converted character
				str[dst_index++] = unesc;
			}
        } else if(is_special_character(str[src_index], comment, equals)) {
			//invalid character
			return 0;
#ifndef INI_FORBID_MULTILINE
		} else if(is_newline(str[src_index])) {
			//newlines need to be escaped
			return 0;
#endif //INI_FORBID_MULTILINE
		} else {
			//copy normal string
            str[dst_index++] = str[src_index];
        }
    }

	//zero-terminate the string
	str[dst_index] = '\0';
	
    return 1;
}

/**
 * Returns the size of a file in bytes.
 * The stream must not be NULL!
 * @param file
 * @param optional pointe to error variable
 * @return file size
 */
size_t fsize(FILE* stream, int* error) {
    size_t ret = 0;

	//iterate through file and count the size
	//NOTE: this method was chosen to assure compatibillity
    while(fgetc(stream) != EOF)
        ++ret;
    if(error && ferror(stream) != 0) {
        *error = ferror(stream);
        return 0;
    }

	//reset file
    if(error)
        *error = fseek(stream, 0, SEEK_SET);
    else
        fseek(stream, 0, SEEK_SET);
	
    return ret;
}

/**
 * Takes three pointers and frees them if they are not NULL.
 * @param first pointer
 * @param second pointer
 * @param third pointer
 */
void free3(void* data0, void* data1, void* data2) {
    if(data0)
        free(data0);
    if(data1)
        free(data1);
    if(data2)
        free(data2);
}

/**
 * Counts the number of newlines at the given position in a string.
 * '\n' and '\r' are seperately counted as a single newline but the 
 * combinations '\n\r' and '\r\n' are also counted as a single newline
 * to assure compatibillity.
 * The input string must not be NULL and the position offset is expected to be in range!
 * @param string
 * @param optional pointer to position offset
 * @return number of newlines
 */
size_t count_newlines(const char* str, size_t* pos) {
	size_t local_pos = 0;
	if(pos)
		local_pos = *pos;
	
    unsigned int ret = 0;
    char prev_newline = 0;
    while(is_newline(str[local_pos])) {
        if(prev_newline == str[local_pos]) {
            ++ret;
			prev_newline = str[local_pos];
		} else if(prev_newline == 0) {
            ++ret;
			prev_newline = str[local_pos];
		} else {
			prev_newline = 0;
		}
		
		++local_pos;
    }
	
	if(pos != NULL)
		*pos = local_pos;
	
    return ret;
}

/**
 * Skips the whitespaces at the given position at a string.
 * The input string must not be NULL and the position offset is expected to be in range!
 * @param string
 * @param optional pointer to offset
 * @param optional pointer to line number counter
 * @return number of skipped whitespaces
 */
size_t skip_whitespaces(const char* str, size_t* pos, unsigned int* line_number) {
	size_t local_pos = 0;
	if(pos)
		local_pos = *pos;

	//save the starting position
	size_t start_pos = local_pos;
	
    while(is_whitespace(str[local_pos])) {
		//count spacing characters
        while(is_spacing(str[local_pos]))
            ++local_pos;

		//count newlines
		if(line_number)
			*line_number += count_newlines(str, &local_pos);
		else
			count_newlines(str, &local_pos);
    }

	if(pos)
		*pos = local_pos;

	//return the number of skipped characters
    return local_pos - start_pos;
}

/**
 * Skips all characters until a newline is reached.
 * The input string must not be NULL and the position offset is expected to be in range!
 * @param string
 * @param optional pointer to offset
 * @return number of skipped characters
 */
size_t skip_comment(const char* str, size_t* pos) {
	size_t local_pos =  0;
	if(pos)
		local_pos = *pos;

	size_t start_pos = local_pos;
	
	while(!is_newline(str[local_pos]) && str[local_pos] != '\0')
		++local_pos;

	if(pos)
		*pos = local_pos;
	
    return local_pos - start_pos;
}

/**
 * @deprecated
 */
int validate(const char* data, int allow_spacing, int allow_newline, char comment, char equals)
{
    unsigned int pos = 0;
    while(data[pos] != '\0')
		{
			//check for non printable characters
			if(!is_printable_char(data[pos]))
				return 0;
			if(is_special_character(data[pos], comment, equals) && data[pos] != '\\')
				{   //terminate on illegal character
					if(pos == 0)
						return 0;
					else if(data[pos - 1] != '\\')
						return 0;
				}
			//check for spacing (if enabled)
			if(!allow_spacing && is_spacing(data[pos]))
				return 0;
			//check for newline
			if(is_newline(data[pos]))
				{   //check for allowance
					if(!allow_newline)
						{
							return 0;
						}
					else
						{   //check for escape sequence
							if(pos == 0)
								return 0;
							else if(data[pos - 1] != '\\' && is_whitespace(data[pos - 1]))
								return 0;
						}
				}
			++pos;
		}
    return 1;
}

/**
 * Checks if a string contains UTF-8 characters.
 * The input string musn't be NULL!
 * @param string
 * @return 1 if the string contains UTF-8 characters; 0 otherwise
 */
int contains_utf8(const char* str) {
	size_t len = strlen(str);
	size_t i;
	
	for(i = 0; i < len; ++i) {
		if(is_utf8_part(str[i]))
			return 1;
	}
	
	return 0;
}

/**
 * Checks if a string contains characters which is not allowed in a name or a section.
 * The input string musn't be NULL!
 * @param string
 * @param comment character
 * @param equals character
 * @return 1 if the string contains an illegal character; 0 otherwise
 */
int contains_escape_characters(const char* str, char comment_char, char equals_char) {
	size_t len = strlen(str);
	size_t i;
	char c;
	
	for(i = 0; i < len; ++i) {
		c = str[i];
		if(is_special_character(c, comment_char, equals_char) || is_whitespace(c))
			return 1;
	}
	
	return 0;
}

/**
 * Reads either a key name or a section name from an input string.
 * The input string and the output string musn't be NULL and the start and end position
 * are expected to be in range!
 * @param name
 * @param input string
 * @param start position
 * @param end position
 * @param comment character
 * @param equals character
 * @param allow UTF-8 flag
 * @return 1 on success; 0 otherwise
 */
int read_name(char* name, const char* str, size_t start, size_t end, char comment_char, char equals_char, int allow_utf8) {
    size_t len = end - start;

	//copy new name
    name = (char*)memcpy(name, str + start, len);
	name[len] = '\0';
			
	//strip whitespaces form the front and the back of the string
    name = stripws(name);

	//check against illegal characters
	if(contains_escape_characters(name, comment_char, equals_char))
		return 0;

	//chech if UTF-8 characters are allowed
	if(!allow_utf8) {
		if(contains_utf8(name))
			return 0;
	}
	
	return 1;
}

/**
 * Removes quotes directly from the front and the back of the string.
 * The input string musn't be NULL!
 * @param string
 * @return 1 on success, 0 if the quotes don't match
 */
int remove_quotes(char* str) {
	size_t len = strlen(str);
	
	if(len) {
		char first = str[0];
		char last = str[len - 1];
		if(first == '"' || first == '\'') {
			if(len < 2)
				//string too short for quotes
				return 0;

			if(first != last)
				//quotes not closed correctly
				return 0;

			//remove quotes
			len -= 2;
			memcpy(str, str + 1, len);
		    str[len] = '\0';
		}
	}

	return 1;
}

/**
 * Reads value from an input string.
 * The input string and the output string musn't be NULL and the start and end position
 * are expected to be in range!
 * @param value
 * @param input string
 * @param start position
 * @param end position
 * @param comment character
 * @param equals character
 * @param optional pointer to line number counter
 * @param allow UTF-8 flag
 * @return 1 on success; 0 otherwise
 */
int read_value(char* value, const char* str, size_t start, size_t end, char comment_char, char equals_char, unsigned int* line_number, int allow_utf8) {
    size_t len = end - start;
	
	//copy new value
    value = (char*)memcpy(value, str + start, len);
	value[len] = '\0';

	//count newlines
	if(line_number) {
		size_t i;
		for(i = 0; i < len; ++i) {
			while(value[i] && !is_newline(value[i]))
				++i;
			*line_number += count_newlines(value, &i);
		}
	}
			
	//strip whitespaces form the front and the back of the string
    value = stripws(value);

	if(!remove_quotes(value))
		return 0;
	
	//check against illegal characters
	if(!unescape_string(value, strlen(value), comment_char, equals_char))
		return 0;

	//chech if UTF-8 characters are allowed
	if(!allow_utf8) {
		if(contains_utf8(value))
			return 0;
	}
	
	return 1;
}

/**
 * Parses a string with a given file size and calls a handler function when a key value pair is read.
 * The handler should return 0 on failure and a non 0 value on success.
 * @param string
 * @param file size
 * @param handler function
 * @param pointer to user data structure
 * @param comment char
 * @param equals char
 * @param allow UTF-8 flag
 * @return 0 on success; -1 on memory error and the currently parsed line number on a parsing error 
 */
int ini_read(const char* data, size_t file_size, ini_event handler, void* data_structure, char comment_char, char equals_char, int allow_utf8) {
    char* current_section = (char*)calloc(file_size, sizeof(char));
    char* current_name = (char*)calloc(file_size, sizeof(char));
    char* current_value = (char*)calloc(file_size, sizeof(char));
    size_t pos = 0;
    unsigned int line_number = 1;

	if(!data || !file_size || !current_section || !current_name || !current_value) {
		free3(current_section, current_name, current_value);
		return -1;
	}

	if(is_special_character(comment_char, '\0', '\0') || is_special_character(equals_char, '\0', '\0')) {
		//invalid equals char or invalid comment char
		free3(current_section, current_name, current_value);
		return -1;
	}

    while(pos < file_size) {
        skip_whitespaces(data, &pos, &line_number);
		
        if(data[pos] == '[') {
            //skip '['
            ++pos;
            unsigned int start = pos;
			
            //search for the first special character
			while(!is_special_character(data[pos], comment_char, equals_char) && !is_newline(data[pos]) && data[pos])
				++pos;

			if(!read_name(current_section, data, start, pos, comment_char, equals_char, allow_utf8)) {
				free3(current_section, current_name, current_value);
				return line_number;
			}

            if(data[pos++] != ']') {
				//section never ended
                free3(current_section, current_name, current_value);
                return line_number;
            } else if(has_non_whitespace_before_newline(data, &pos)) {
				if(data[pos] != comment_char) {
					//section is not the only thing on this line (besides whitespaces and comments)
					free3(current_section, current_name, current_value);
					return line_number;
				} else {
					//skip comment
					skip_comment(data, &pos);
				}
			}
        } else if(data[pos] == comment_char) {
            //skip the comment char
            ++pos;
            skip_comment(data, &pos);
			
        } else if(is_special_character(data[pos], comment_char, equals_char)) {
			//illegal character
            free3(current_section, current_name, current_value);
            return line_number;
			
        } else if(data[pos] == '\0') {
			//reached end of string
            break;
			
		} else {
            unsigned int start = pos;
			
            //search for the first special character
			while(!is_special_character(data[pos], comment_char, equals_char) && !is_newline(data[pos]) && data[pos])
				++pos;

			if(!read_name(current_name, data, start, pos, comment_char, equals_char, allow_utf8)) {
				free3(current_section, current_name, current_value);
				return line_number;
			}

            if(data[pos] == equals_char) {
				//read value
                start = ++pos;
				
                //search for newline
				int escaped = 0;
				while((!is_newline(data[pos]) || escaped) && data[pos]) {
					if(data[pos] == '\\' && !escaped)
						escaped = 1;
					else if(!is_whitespace(data[pos]))
						escaped = 0;

					//check for comment
					if(data[pos] == comment_char && data[pos-1] != '\\')
						break;
					
					++pos;
				}

				//read value
				if(!read_value(current_value, data, start, pos, comment_char, equals_char, &line_number,  allow_utf8)) {
                    free3(current_section, current_name, current_value);
                    return line_number;
				}

				//skip comments
				if(data[pos] == comment_char)
					skip_comment(data, &pos);

			} else if(data[pos] == comment_char) {
				//skip comment
				skip_comment(data, &pos);
				current_value[0] = '\0';
            } else if(is_newline(data[pos]) || !data[pos]) {
				//handle empty value
                current_value[0] = '\0';
            } else {
				//invalid character in name
                free3(current_section, current_name, current_value);
                return line_number;
			}
			
            //call user
            if(!handler(current_section, current_name, current_value, data_structure)) {
                free3(current_section, current_name, current_value);
                return line_number;
            }
        }
    }

	//done parsing
    free3(current_section, current_name, current_value);
    return 0;
}

/**
 * Checks if a given file has a byte order marker and skips it.
 * The file stream mustn't be invalid!
 * @param file stream
 * @return 1 if it has a BOM; 0 if this is not the case; -1 on failure
 */
int check_for_BOM(FILE* stream) {
	//look for byte order mark
    char BOM[3] = {0, 0, 0};
	
    if(fread(BOM, 1, 3, stream) == 3) {
        if(BOM[0] != (char)0xEF || BOM[1] != (char)0xBB || BOM[2] != (char)0xBF)
            rewind(stream);
        else
            return 1;
		
    } else if(ferror(stream)) {
        return -1;
    } else {
        rewind(stream);
    }
	
    return 0;
}

/**
 * Reads an .ini file and parses it using the user defined handler function and the data structure.
 * The handler should return 0 on failure and a non 0 value on success.
 * @param string
 * @param file size
 * @param handler function
 * @param pointer to user data structure
 * @param comment char
 * @param equals char
 * @param allow UTF-8 flag
 * @return 0 on success; -1 on memory error and the currently parsed line number on a parsing error
 */
int ini_read_file(const char* path, ini_event handler, void* data_structure, char comment_char, char equals_char, int allow_utf8) {
	int ret = 0;
	int error = 0;
    FILE* file = fopen(path, "rb");
	
    if(file == NULL)
        return -1;

    //get file size
    size_t file_size = fsize(file, &error);
    if(error || !file_size)
        return -1;

	//check for byte order mark
    int parse_utf8 = check_for_BOM(file);
    if(parse_utf8 == -1) {
        fclose(file);
        return -1;
    } else if(parse_utf8 && allow_utf8) {
		file_size -= 3;
	}

	//override BOM if UTF-8 is completely allowed
	if(allow_utf8 == INI_ALLOW_UTF8)
		parse_utf8 = 1;
	
    //allocate memory
    char* file_content = (char*)malloc(file_size + 1);
    if(!file_content)
        return -1;
	
    //read content
    size_t read_size = fread(file_content, 1, file_size, file);
    if(read_size != file_size) {
        free(file_content);
        return -1;
    }
	
    file_content[file_size] = '\0';
	
    //close file
    if(fclose(file) == EOF) {
        free(file_content);
        return -1;
    }
	
    ret = ini_read(file_content, file_size + 1, handler, data_structure, comment_char, equals_char, parse_utf8);
    free(file_content);
	
    return ret;
}
