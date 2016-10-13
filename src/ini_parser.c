#include "ini_parser.h"

/*
    QUESTIONS:
        - what to do with square brackets?????
        - set error flag in every function to 0???
        - !feof(file) or feof(file) == 0??????
        - what to do with ascii code 127??? ('')
        - which whitespace characters can occur????
        - use int or char???
        - ignore non existant escaped characters?
        - unify returns???
        - which order of arguments with error???
        - how to handle : = and ; #
        - return error in int functions???

        - replace spaces with tabs?????
        - how to handle comma seperated values???

    TIPS:
        - begin with ret and return it at the end
        - a function has a task and a subject

    TODO:
        - fix search for end of line after quotes
        - refactor ini_read_file
        - fix newline
        - include unallow utf-8
        - switch between python syle and ini style
        - implement details from https://cloanto.com/specs/ini/

        - support writing files

    BUGS:
        - line numbers are broken
        - linebreaks allowed in section
        - problems with quoted values

    TO TEST:
        - newline combinations

    TESTED:
        - first character missing after empty line
        - unescape string with multiline
        - test eof
        - optimal condition tested
        - \ tested
        - quoted values tested
        - # and ; tested
        - test line without =, : or ]
        - linenumber not correct (after multiline!?)

    FIXED:
        - problems with / for linebreak
        - sections is missing the first letter
        - quoted values are not working anymore (--> read value from stream)
        - refactor unescape string
        - empty value doesn't work after multiline
        - use seperate string in unescape string
        - better error handling in unescape string
        - use realloc instead of malloc?????    --> realloc or use big buffers
        - strict implementation???              --> strict
        - stop on first error?                  --> yes
        - check ferror if eof is read???        --> yes
        - free string in unescape string?       --> no (free at same level as malloc)
        - fix bitshift!!    --> use unsigned char
        - handle BOM
        - support \x0000 for unicode characters
        - comment as first character of the line doesn't work
        - extend to ISO/IEC 8859-1 aka latin1
        - don't remove manually inserted whitespaces
        - don't remove whitespaces if value was quoted
        - ; / # directly behind = still displayed
        - empty values create empty line
        - error on some escaped values (', ", =, :, \ + \n)
*/

static char hex_digit_to_binary(char c)
{   //convert ASCII to hex and return -1 on failure
    if(c >= '0' && c <= '9')
        return c - '0';
    else if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else if(c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

static uint32_t utf8touc(uint32_t code)
{   //NOTE: this uses the UTF-8 standard since 2003
    unsigned char c[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    if(code <= 0x007F)
    {
        c[0] = (0b01111111 & code);
        c[1] = 0;
        c[2] = 0;
        c[3] = 0;
    }
    else if(code <= 0x07FF)
    {
        c[0] = (0b10000000 | (0b00111111 & code));
        c[1] = (0b11000000 | (0b00011111 & (code >> 6)));
        c[2] = 0;
        c[3] = 0;
    }
    else if(code <= 0xFFFF)
    {
        c[0] = (0b10000000 | (0b00111111 & code));
        c[1] = (0b10000000 | (0b00111111 & (code >> 6)));
        c[2] = (0b11100000 | (0b00001111 & (code >> 12)));
        c[3] = 0;
    }
    else if(code <= 0x10FFFF)
    {
        c[0] = (0b10000000 | (0b00111111 & code));
        c[1] = (0b10000000 | (0b00111111 & (code >> 6)));
        c[2] = (0b10000000 | (0b00111111 & (code >> 12)));
        c[3] = (0b11110000 | (0b00000111 & (code >> 18)));
    }
    return (c[3] << 24) | (c[2] << 16) | (c[1] << 8) | c[0];
}

static int is_spacing(char c)
{
    if(c == '\t' || c == ' ')
        return 1;
    return 0;
}

static int is_newline(char c)
{
    if(c == '\r' || c == '\n')
        return 1;
    return 0;
}

static int is_whitespace(char c)
{
    if(is_spacing(c) || is_newline(c))
        return 1;
    return 0;
}

static int is_special_character(char c, char comment_char, char equals_char)
{
    if(c == '[' || c == ']' || c == '\\' || c == '\'' || c == '"' || c == comment_char || c == equals_char)
        return 1;
    return 0;
}

static int is_utf_8_upper_part(char c)
{
    if(c < 0)
        return 1;
    return 0;
}

static int is_printable_char(char c)
{
    if((c > 32 && c < 128) || is_utf_8_upper_part(c) || is_whitespace(c))
        return 1;
    return 0;
}

static size_t lskipws(char* str)
{
    size_t len = strlen(str);
    size_t i = 0;
    //skip whitespaces
    while(i < len && is_whitespace(str[i++]));
    //return index of the last whitespace character at the front
    return --i;
}

static size_t rskipws(char* str)
{
    size_t len = strlen(str);
    size_t i = len;
    //skip whitespaces from back to front
    while(i >= 0 && (is_whitespace(str[i]) || str[i] == '\0'))
    {
        --i;
    }
    //return index of first whitespace character at the end
    return ++i;
}

static size_t max_size_t(size_t f, size_t s)
{
    return ((f > s) ? f : s);
}

static char read_hex_from_string(char* str, size_t pos, int* error)
{
    char ret = 0;
    size_t len = strlen(str);
    char c1, c2;
    size_t i = pos;
    if(len < i + 2)
    {   //abort if the string is too short
        if(error != NULL)
            *error = INI_PARSE_ERROR;
        return 0;
    }
    //read characters
    c1 = str[i];
    c2 = str[++i];
    //convert both values to hex
    c1 = hex_digit_to_binary(c1);
    c2 = hex_digit_to_binary(c2);
    if(c1 >= 0 && c2 >= 0)
    {   //combine values if it is valid
        c1 = (c1 << 4);
        ret = (unsigned char)c1 | (unsigned char)c2;
    }
    else
    {   //exit if there isn't a hex value at given position
        if(error != NULL)
            *error = INI_PARSE_ERROR;
        return 0;
    }
    return ret;
}

static uint32_t read_unicode_from_string(char* str, size_t pos, int* error)
{
    uint32_t ret = 0;
    size_t len = strlen(str);
    size_t i = pos;
    unsigned int hex_length;
    for(hex_length = 0; i + 2 < len && hex_length < 2; ++hex_length)
    {
        int hex_err = 0;
        char val = read_hex_from_string(str, ++i, &hex_err);
        if(hex_err != 0)
        {
            if(error != NULL)
                *error = INI_PARSE_ERROR;
            return 0;
        }
        ++i;
        ret |= ((unsigned char)val << ((3 - hex_length) * 8));
    }
    if(hex_length < 1 || hex_length > 3)
    {   //invalid format
        if(error != NULL)
            *error = INI_PARSE_ERROR;
        return 0;
    }
    //remove 0 bytes from the end
    ret = (ret >> (4 - hex_length) * 8);
    ret = utf8touc(ret);
    if(ret == 0xFFFFFFFF)
    {   //invalid code
        if(error != NULL)
            *error = INI_PARSE_ERROR;
        return 0;
    }
    return ret;
}

static int has_non_whitespace_before_newline_string(const char* str, size_t* pos)
{
    if(pos == NULL)
        return INI_RETURN_UNKNOWN_ERROR;
    ++(*pos);
    while(!is_newline(str[*pos]) && str[*pos] != '\0')
    {
        if(!is_whitespace(str[*pos++]))
        {
            return 0;
        }
    }
    return 1;
}

/*
static char has_non_whitespace_before_newline_stream(char c, FILE* stream, int* error = NULL)
{   //check for invalid characters before newline
    while(feof(stream) == 0 && c != '\n')
    {
        c = fgetc(stream);
        if(!is_whitespace(c))
        {
            return -1;
        }
    }
    if(feof(stream) != 0 && ferror(stream) != 0)
    {
        if(error != NULL)
            *error = INI_RETURN_FILE_ERROR;
        return -1;
    }
    return c;
}
*/

static char unescape_char(char c, char comment, char equals, int* error)
{
    char ret = -1;
    if(c == comment)
        return comment;
    else if(c == equals)
        return equals;
    switch(c)
    {
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
            break;
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            ret = '\n';
            break;
        default:
#ifndef INI_IGNORE_INVALID_ESCAPED_CHARACTERS
            if(error != NULL)
                *error = INI_PARSE_ERROR;
            return -1;
#endif // INI_IGNORE_INVALID_ESCAPED_CHARACTERS
            break;
    };
    return ret;
}

static int unescape_sequence(char* src, char* dest, size_t len, size_t* last_inserted_whitespace, size_t src_offset, size_t dest_offset, int allow_spacing, char comment, char equals)
{
    if(last_inserted_whitespace == NULL)
        return INI_RETURN_UNKNOWN_ERROR;
    *last_inserted_whitespace = 0;
    size_t i = src_offset, j = dest_offset;
    for(j = 0; i < len; ++i)
    {
        char c = src[i];
        if(c == '\\')
        {
            if(++i == len)
            {   //non-existent escaped character
                return INI_PARSE_ERROR;
            }
            else
            {
                c = src[i];
                int unesc_error = 0;
                char unesc = unescape_char(c, comment, equals, &unesc_error);
                if(unesc_error != 0)
                {
                    return unesc_error;
                }
                else if(is_spacing(c))
                {   // \ only allowed at end of the line
                    if(has_non_whitespace_before_newline_string(src, &i))
                    {
                        return INI_PARSE_ERROR;
                    }
                }
                else if(c == 'x' || c == 'X')
                {
                    int uc_error = 0;
                    uint32_t hex_val = read_unicode_from_string(src, i, &uc_error);
                    if(uc_error != 0)
                    {
                        return INI_PARSE_ERROR;
                    }
                    //skip escaped code
                    i += 4;
                    //copy only non-null bytes
                    for(int k = 3; k >= 0; --k)
                    {
                        char current_byte = (0xFF & (hex_val >> (k * 8)));
                        if(current_byte != 0)
                            dest[j++] = current_byte;
                    }
                }
                else
                {
                    dest[j++] = unesc;
                }
            }
        }
        else
        {
//TODO: remove allow_spacing later
            if(is_spacing(src[i]) && !allow_spacing)
            {
                return INI_PARSE_ERROR;
            }
            dest[j++] = src[i];
        }
    }
    return 0;
}

static char* unescape_string(char* str, int allow_spacing, int quoted, int *error, char comment, char equals)
{
    size_t len = strlen(str) + 1;
    char* new_str = (char*) malloc(len);
    if(new_str == NULL)
    {
        if(error != NULL)
            *error = INI_RETURN_MEMORY_ERROR;
        return NULL;
    }
    size_t i = 0, j = 0;
    //don't copy whitespaces
    if(!quoted)
            i = lskipws(str);
    size_t last_inserted_whitespace = 0;
//TODO: remove allow_spacing later
    int unesc_error = unescape_sequence(str, new_str, len - i, &last_inserted_whitespace, i, j, allow_spacing, comment, equals);
    if(unesc_error != 0)
    {
        if(error != NULL)
            *error = unesc_error;
        return NULL;
    }
    //remove whitespaces from the end and zero terminate the string
    if(!quoted)
        j = max_size_t(rskipws(new_str), last_inserted_whitespace);
    else
        j = len - 1;
    new_str[j] = '\0';
    //free unnecessery bytes
    len = strlen(new_str) + 1;
    char* new_str_shortened = (char*)realloc(str, len);
    if(new_str_shortened == NULL)
    {
        if(error != NULL)
            *error = INI_RETURN_MEMORY_ERROR;
        return NULL;
    }
    //fill the old string with data
    str = new_str_shortened;
    memcpy(str, new_str, len);
    free(new_str);
    return str;
}

/*
static int clean_up_with_2(FILE* file, char* first, char* second)
{
    free(first);
    free(second);
    return fclose(file);
}


static int clean_up(FILE* file, char* section, char* name, char* value)
{
    free(section);
    return clean_up_with_2(file, name, value);
}

static int move_stream_pos(FILE* stream, int num)
{   //go num steps back
    int pos = ftell(stream) + num;
    if(pos < 0)
    {   //position not found
        return INI_RETURN_UNKNOWN_ERROR;
    }
    if(fseek(stream, pos, SEEK_SET) != 0)
    {   //reverting failed
        return INI_RETURN_UNKNOWN_ERROR;
    }
    return 0;
}

static char jump_to_first_non_whitespace_in_stream(FILE* stream, int* comment, int* error = NULL, char comment_char = ';')
{
    char c;
    do
    {
        c = fgetc(stream);
        if(is_spacing(c))
        {
            continue;
        }
        else if(c == comment_char)
        {
            *comment = 1;
            break;
        }
        else
        {
            break;
        }
    }
    while(feof(stream) == 0);
    if(feof(stream) != 0 && ferror(stream) != 0)
    {
        if(error != NULL)
        {
            *error = INI_RETURN_FILE_ERROR;
        }
        return 0;
    }
    return c;
}
*/

static size_t fsize(FILE* stream, int* error)
{
    size_t ret = 0;
    while(fgetc(stream) != EOF)
        ++ret;
    if((error != NULL) && ferror(stream) != 0)
    {
        *error = ferror(stream);
        return 0;
    }
    if(error != NULL)
        *error = fseek(stream, 0, SEEK_SET);
    else
        fseek(stream, 0, SEEK_SET);
    return ret;
}
/*
char* read_string_from_stream(FILE* stream, size_t begin_pos, size_t end_pos, char* str, size_t s, int *error = NULL, int quoted = 0, int allow_spacing = 1)
{   //read string
    if(fseek(stream, begin_pos, SEEK_SET) != 0)
    {
        if(error != NULL)
            *error = INI_RETURN_UNKNOWN_ERROR;
        return NULL;
    }
    if (fread(str, 1, s, stream) != s)
    {   //reading failed
        if(error != NULL)
            *error = INI_RETURN_UNKNOWN_ERROR;
        return NULL;
    }
    str[s] = '\0';
    char*  new_str = (char*)malloc(s + 1);
    if((new_str = unescape_string(str, allow_spacing, quoted, error)) == NULL)
    {   //string conversion failed
        return NULL;
    }
    str = new_str;
    return str;
}
*/
static int check_for_BOM(FILE* stream, int* error)
{   //look for byte order mark
    int ret = 0;
    char BOM[3] = {0, 0, 0};
    if(fread(BOM, 1, 3, stream) == 3)
    {
        char BOM_code[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
        if(strcmp(BOM, BOM_code) != 0)
        {
            rewind(stream);
        }
        else
        {
            ret = 1;
        }
    }
    else if(ferror(stream) != 0)
    {
        if(error == NULL)
        {
            *error = INI_RETURN_FILE_ERROR;
        }
        return 0;
    }
    else
    {
        rewind(stream);
    }
    return ret;
}
/*
static int count_newlines(char c, FILE* stream)
{   //manage cr and lf combinations
    int ret = 0;
    int newline_flag;
    char old_char = c;
    while(is_newline(c))
    {
        c = fgetc(stream);
        if(c != old_char)
        {
            if(newline_flag)
            {
                newline_flag = 0;
                ++ret;
            }
            else
            {
                newline_flag = 1;
            }
        }
        else
        {
            ++ret;
        }
        old_char = c;
    }
    if(ferror(stream) != 0)
    {
        return INI_RETURN_FILE_ERROR;
    }
    int error = move_stream_pos(stream, -1);
    if(error < 0)
    {
        return error;
    }
    return ret;
}
*/

static void free3(void* data0, void* data1, void* data2)
{
    if(data0 != NULL)
        free(data0);
    if(data1 != NULL)
        free(data1);
    if(data2 != NULL)
        free(data2);
}

static int count_newlines(const char* data, size_t* pos)
{
    if(pos == NULL)
        return INI_RETURN_UNKNOWN_ERROR;
    unsigned int ret = 0;
    char prev_newline = 0;
    while(is_newline(data[*pos]))
    {
        if(prev_newline == data[*pos])
            ++ret;
        else if(prev_newline == 0)
            ++ret;
        ++(*pos);
    }
    return ret;
}

static int skip_whitespaces(const char* data, size_t* pos, unsigned int* line_number)
{
    if(line_number == NULL || pos == NULL)
        return INI_RETURN_UNKNOWN_ERROR;
    while(is_whitespace(data[*pos]))
    {
        while(is_spacing(data[*pos]))
            ++(*pos);
        *line_number += count_newlines(data, pos);
    }
    return INI_RETURN_SUCCESS;
}

static int skip_comment(const char* data, size_t* pos)
{
    if(pos == NULL)
        return INI_RETURN_UNKNOWN_ERROR;
    while(!is_newline(data[*pos]) && data[*pos] != '\0')
        ++(*pos);
    return INI_RETURN_SUCCESS;
}

static int validate(const char* data, int allow_spacing, int allow_newline, char comment, char equals)
{
    unsigned int pos = 0;
    while(data[pos] != 0)
    {
        //check for non printable characters
        if(!is_printable_char(data[pos]))
            return 0;
        if(is_special_character(data[pos], comment, equals))
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

int ini_read_file(const char* path, ini_event handler, void* data_structure, char comment_char, char equals_char)
{
    FILE* file;
    file = fopen(path, "rb");
    if(file == NULL)
        return INI_RETURN_FILE_ERROR;

    int error = 0;
//TODO: use this?????
    int has_bom = check_for_BOM(file, &error);
    if(error != 0)
    {
        fclose(file);
        return error;
    }

    //get size
    size_t file_size = fsize(file, &error);
    if(error != 0)
        return INI_RETURN_FILE_ERROR;
    //allocate memory
    char* file_content = (char*)malloc(file_size + 1);
    if(file_content == NULL)
        return INI_RETURN_MEMORY_ERROR;
    //read content
    size_t read_size = fread(file_content, 1, file_size - (has_bom ? 3 : 0), file);
    if(read_size != file_size)
    {
        free(file_content);
        return INI_RETURN_FILE_ERROR;
    }
    file_content[file_size] = '\0';
    //close file
    if(fclose(file) == EOF)
    {
        free(file_content);
        return INI_RETURN_FILE_ERROR;
    }
    int ret = ini_read(file_content, file_size, handler, data_structure, comment_char, equals_char, has_bom);
    free(file_content);
    return ret;
}

int ini_read(const char* data, size_t file_size, ini_event handler, void* data_structure, char comment_char, char equals_char, int has_bom)
{
    char* current_section = NULL;
    char* current_name = NULL;
    char* current_value = NULL;
    size_t pos = 0;
    unsigned int line_number = 0;

    int error = 0;

    while(pos < file_size)
    {
        skip_whitespaces(data, &pos, &line_number);
        if(data[pos] == '[')
        {
            //skip '['
            ++pos;
            unsigned int start = pos;
            //search for the first ']'
            while(1)
            {
                while(data[pos] != ']' && is_newline(data[pos]) && data[pos] != '\0')
                    ++pos;
                //end search if it is not escaped
                if(data[pos-1] != '\\')
                    break;
            }
            if(data[pos] != ']')
            {   //the comment section never ended
                free3(current_section, current_name, current_value);
                return line_number;
            }
            //calculate size
            unsigned int len = pos - start;
            //allocate memory for the section name
            char* new_section = (char*)realloc(current_section, len + 1);
            if(new_section == NULL)
            {
                free3(current_section, current_name, current_value);
                return INI_RETURN_MEMORY_ERROR;
            }
            current_section = new_section;
            current_section = (char*)memcpy(current_section, data + start, len);
            current_section[len] = '\0';
            //validate the read value
            if(!validate(current_section, 0, 0, comment_char, equals_char))
            {
                free3(current_section, current_name, current_value);
                return line_number;
            }
            //unescape string
            new_section = unescape_string(current_section, 0, 0, &error, comment_char, equals_char);
            if(error != 0)
            {
                free3(current_section, current_name, current_value);
                if(error == INI_PARSE_ERROR)
                    return line_number;
                else
                    return error;
            }
            current_section = new_section;
            if(data[pos++] != ']' || has_non_whitespace_before_newline_string(data, &pos))
            {
                free3(current_section, current_name, current_value);
                return line_number;
            }
        }
        else if(data[pos] == comment_char)
        {
            //skip the comment char
            ++pos;
            skip_comment(data, &pos);
        }
        else if(is_special_character(data[pos], comment_char, equals_char))
        {   //illegal character
            free3(current_section, current_name, current_value);
            return line_number;
        }
        else if(data[pos] == '\0')
        {   //reached end of string
            break;
        }
        else
        {
//TODO:implement



            unsigned int start = pos;
            //search for the first '='
            while(1)
            {
                while(data[pos] != '=' && is_newline(data[pos]) && data[pos] != '\0')
                    ++pos;
                //end search if it is not escaped
                if(data[pos-1] != '\\')
                    break;
            }
            if(data[pos] != '=' && !is_newline(data[pos]))
            {   //the current name never ended
                free3(current_section, current_name, current_value);
                return line_number;
            }
            //calculate size
            unsigned int len = pos - start;
            //allocate memory for the name
            char* new_name = (char*)realloc(current_name, len + 1);
            if(new_name == NULL)
            {
                free3(current_section, current_name, current_value);
                return INI_RETURN_MEMORY_ERROR;
            }
            current_name = new_name;
            current_name = (char*)memcpy(current_name, data + start, len);
            current_name[len] = '\0';
            //validate the read value
            if(!validate(current_name, 0, 0, comment_char, equals_char))
            {
                free3(current_section, current_name, current_value);
                return line_number;
            }
            //unescape string
            new_name = unescape_string(current_name, 0, 0, &error, comment_char, equals_char);
            if(error != 0)
            {
                free3(current_section, current_name, current_value);
                if(error == INI_PARSE_ERROR)
                    return line_number;
                else
                    return error;
            }
            current_name = new_name;
            if(has_non_whitespace_before_newline_string(data, &pos))
            {
                free3(current_section, current_name, current_value);
                return line_number;
            }
            if(data[pos] == '=')
            {   //read value
                start = pos;
                //search for newline
                while(1)
                {
                    while(is_newline(data[pos]) && data[pos] != '\0')
                        ++pos;
                    //end search if it is not escaped
                    if(data[pos-1] != '\\')
                        break;
                }
                if(!is_newline(data[pos]))
                {   //the value never ended
                    free3(current_section, current_name, current_value);
                    return line_number;
                }
                //calculate size
                unsigned int len = pos - start;
                //allocate memory for the value
                char* new_value = (char*)realloc(current_value, len + 1);
                if(new_value == NULL)
                {
                    free3(current_section, current_name, current_value);
                    return INI_RETURN_MEMORY_ERROR;
                }
                current_value = new_value;
                current_value = (char*)memcpy(current_value, data + start, len);
                current_value[len] = '\0';
                //validate the read value
                if(!validate(current_value, 0, 0, comment_char, equals_char))
                {
                    free3(current_section, current_name, current_value);
                    return line_number;
                }
                //unescape string
//TODO: merge allow_spacing and allow-newline???
                new_value = unescape_string(current_value, 1, 1, &error, comment_char, equals_char);
                if(error != 0)
                {
                    free3(current_section, current_name, current_value);
                    if(error == INI_PARSE_ERROR)
                        return line_number;
                    else
                        return error;
                }
                current_value = new_value;
                if(has_non_whitespace_before_newline_string(data, &pos))
                {
                    free3(current_section, current_name, current_value);
                    return line_number;
                }
            }



//            read_key_value();
            //call user
            if(handler(current_section, current_name, current_value, data_structure) != 0)
            {
                free3(current_section, current_name, current_value);
                return line_number;
            }
        }
    }
//TODO: free only 3 here
    free3(current_section, current_name, current_value);
    return INI_RETURN_SUCCESS;
}






/*
    while(feof(file) == 0)
    {
        int comment = 0;

        //search for beginning
        while(feof(file) == 0)
        {
            c = fgetc(file);
            if(is_spacing(c))
            {
                continue;
            }
            else if(is_newline(c))
            {
                int ln_offset = count_newlines(c, file);
                if(ln_offset < 0)
                {
                    return ln_offset;
                }
                line_number += ln_offset;
            }
            else
            {
                break;
            }
        }
        if(feof(file) != 0 && ferror(file) != 0)
        {
            clean_up(file, current_section, current_name, current_value);
            return INI_RETURN_FILE_ERROR;
        }

        if(feof(file) == 0)
        {
            if(c == '[')
            {   //read section
                c = jump_to_first_non_whitespace_in_stream(file, &comment, &error, comment_char);
                if(error != 0)
                {
                    clean_up(file, current_section, current_name, current_value);
                    return error;
                }
                //read section name (which needs a pointer to the previous character)
                int begin_pos = ftell(file) - 1;
                if(begin_pos < 0)
                {   //error while reading
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_UNKNOWN_ERROR;
                }
                int end_pos = begin_pos;
                while(feof(file) == 0 && !comment)
                {   //don't allow [, ] (according to the spec)
                    if(is_printable_char(c) && c != '[' && c != ']')
                    {   //if allowed symbol is read, continue to read
                        if(is_special_character(c, ';', '='))
                        {
                            int error = move_stream_pos(file, -1);
                            if(error != 0)
                            {
                                clean_up(file, current_section, current_name, current_value);
                                return error;
                            }
                            if(fgetc(file) != '\\')
                            {   //abort the algorithm; invalid format
                                clean_up(file, current_section, current_name, current_value);
                                return line_number;
                            }
                        }
                        c = fgetc(file);
                        continue;
                    }
                    else if(c == ']')
                    {   //end section
                        break;
                    }
                    else
                    {   //abort the algorithm; invalid format
                        clean_up(file, current_section, current_name, current_value);
                        return line_number;
                    }
                    c = fgetc(file);
                }
                if(feof(file) != 0 && ferror(file) != 0)
                {
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_FILE_ERROR;
                }
                //decode section name
                end_pos = ftell(file) - 1;
                if(end_pos < 0)
                {   //error while reading
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_UNKNOWN_ERROR;
                }
                //check for invalid characters before newline
                while(feof(file) == 0 && is_newline(c))
                {
                    c = fgetc(file);
                    if(!is_whitespace(c))
                    {
                        clean_up(file, current_section, current_name, current_value);
                        return line_number;
                    }
                }
                if(feof(file) != 0 && ferror(file) != 0)
                {
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_FILE_ERROR;
                }
                size_t s = end_pos - begin_pos;
                //realloc data
                char* new_str = (char*)realloc(current_section, s + 1);
                if(new_str == NULL)
                {   //memory allocation failed
                    clean_up_with_2(file, current_name, current_value);
                    return INI_RETURN_MEMORY_ERROR;
                }
                current_section = new_str;
                //reading string
                current_section = read_string_from_stream(file, begin_pos, end_pos, current_section, s, &error);
                if(current_section == NULL)
                {   //clean up and exit on error
                    clean_up(file, current_section, current_name, current_value);
                    if(error == INI_PARSE_ERROR)
                        return line_number;
                    else
                        return error;
                }
                //skip the ']'
                c = fgetc(file);
            }
            else if(c == '!' || c == '%' || c == '&' || (c >= 40 && c <= 57) || c == '<' || (c >= 62 && c <= 90) || (c >= 94 && c <= 126) || c == '\\' || (c >= 0x80 && c <=0xFF))
            {   //read name
                int begin_pos = ftell(file) - 1;
                if(begin_pos < 0)
                {   //error while reading
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_UNKNOWN_ERROR;
                }
                int end_pos = begin_pos;
                while(feof(file) == 0)
                {
                    if(c == '!' || c == '%' || c == '&' || (c >= 40 && c <= 90) || (c >= 94 && c <= 126) || c == '\"' || c == '\'' || c == '\\' || c == ' ' || c == '\t' || c == '#' || (c >= 0x80 && c <=0xFF))
                    {   //if allowed symbol is read, continue to read
                        if(is_special_character(c, ';', '='))
                        {
                            int error = move_stream_pos(file, -1);
                            if(error != 0)
                            {
                                clean_up(file, current_section, current_name, current_value);
                                return error;
                            }
                            if(fgetc(file) != '\\')
                            {
                                if(c == ';' || c == '#')
                                {   //comment
                                    comment = 1;
                                    break;
                                }
                                else if(c == ':' || c == '=')
                                {   //end name
                                    break;
                                }
                                else
                                {   //abort the algorithm; invalid format
                                    clean_up(file, current_section, current_name, current_value);
                                    return line_number;
                                }
                            }
                        }
                        c = fgetc(file);
                        continue;
                    }
                    else if(c == ':' || c == '=')
                    {   //end section
                        break;
                    }
//TODO: handle newline
                    else if(is_newline(c))
                    {
                        comment = 1;
                        ++line_number;
                        break;
                    }
                    else
                    {   //abort the algorithm; invalid format
                        clean_up(file, current_section, current_name, current_value);
                        return line_number;
                    }
                }
                if(feof(file) != 0 && ferror(file) != 0)
                {
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_FILE_ERROR;
                }
                //decode name without the '=' or ':'
                end_pos = ftell(file) - 1;
                if(end_pos < 0)
                {   //error while reading
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_UNKNOWN_ERROR;
                }
                size_t s = end_pos - begin_pos;
                //realloc data
                char* new_str = (char*)realloc(current_name, s + 1);
                if(new_str == NULL)
                {   //memory allocation failed
                    clean_up_with_2(file, current_section, current_value);
                    return INI_RETURN_MEMORY_ERROR;
                }
                current_name = new_str;
                //reading string
                current_name = read_string_from_stream(file, begin_pos, end_pos, current_name, s, &error);
                if(current_name == NULL)
                {   //clean up and exit on error
                    clean_up(file, current_section, current_name, current_value);
                    if(error == INI_PARSE_ERROR)
                        return line_number;
                    else
                        return error;
                }
                //jump over '=' or ':'
                c = fgetc(file);
                if(comment)
                {   //clear value if it is inside of a comment
                    char* new_value;
                    new_value = (char*)realloc(current_value, 1);
                    if(new_value == NULL)
                    {
                        clean_up(file, current_section, current_name, current_value);
                        return INI_RETURN_MEMORY_ERROR;
                    }
                    current_value = new_value;
                    current_value[0] = '\0';
                }
                else
                {   //remove whitespaces
                    int error = 0;
                    c = jump_to_first_non_whitespace_in_stream(file, &comment, &error, comment_char);
                    if(error != 0)
                    {
                        clean_up(file, current_section, current_name, current_value);
                        return error;
                    }
                    //check for quotes
                    char quote = 0;
                    if(c == '\'')
                    {
                        quote = 1;
                        c = fgetc(file);
                    }
                    else if(c == '\"')
                    {
                        quote = 2;
                        c = fgetc(file);
                    }
                    else if(c == comment_char || is_newline(c))
                    {
                        comment = 1;
                    }
                    //read value
                    begin_pos = ftell(file) - (comment ? 0 : 1);
                    if(begin_pos < 0)
                    {   //error while reading
                        clean_up(file, current_section, current_name, current_value);
                        return INI_RETURN_UNKNOWN_ERROR;
                    }
                    while(feof(file) == 0 && !comment)
                    {
                        if(c == '\n' || c == '\r' ||c == '!' || c == '%' || c == '&' || (c >= 40 && c <= 90) || (c >= 94 && c <= 126) || c == '\"' || c == '\'' || c == '\\' || c == ' ' || c == '\t' || c == '#' || (c >= 0x80 && c <=0xFF))
                        {   //if allowed symbol is read, continue to read
                            if(c == '\\')
                            {
                                c = fgetc(file);
                                if(feof(file) != 0)
                                {
                                    if(ferror(file) != 0)
                                    {
                                        clean_up(file, current_section, current_name, current_value);
                                        return INI_RETURN_FILE_ERROR;
                                    }
                                    break;
                                }
                                if(is_whitespace(c))
                                {
                                    while(!is_newline(c) && feof(file) == 0)
                                    {
                                        c = fgetc(file);
                                        if(!is_whitespace(c))
                                        {   //no characters after \ allowed
                                            clean_up(file, current_section, current_name, current_value);
                                            return line_number;
                                        }
                                    }
                                    if(feof(file) != 0 && ferror(file) != 0)
                                    {
                                        clean_up(file, current_section, current_name, current_value);
                                        return INI_RETURN_FILE_ERROR;
                                    }
                                    ++line_number;
                                    c = fgetc(file);
                                }
                                else if(c == '\\' || c == '\'' || c == '"' || c == '#' || c == ';' || c == '=' || c == ':' || c == 'x' || c == 'X')
                                {   //skip eskaped characters
                                    c = fgetc(file);
                                }
                                continue;
                            }
                            else if(c == comment_char)
                            {   //comment
                                comment = 1;
                                //go one step back
                                int error = move_stream_pos(file, -1);
                                if(error != 0)
                                {
                                    clean_up(file, current_section, current_name, current_value);
                                    return error;
                                }
                                break;
                            }
                            else if(c == '\'')
                            {
                                if(quote == 1)
                                {   //treat the rest like a comment
                                    comment = 1;
                                    quote = 3;
                                    //go one step back
                                    int error = move_stream_pos(file, -1);
                                    if(error != 0)
                                    {
                                        clean_up(file, current_section, current_name, current_value);
                                        return error;
                                    }
                                }
                                else
                                {   //abort the algorithm; invalid format
                                    clean_up(file, current_section, current_name, current_value);
                                    return line_number;
                                }
                            }
                            else if(c == '\"')
                            {
                                if(quote == 2)
                                {   //treat the rest like a comment
                                    comment = 1;
                                    quote = 3;
                                    //go one step back
                                    int error = move_stream_pos(file, -1);
                                    if(error != 0)
                                    {
                                        clean_up(file, current_section, current_name, current_value);
                                        return error;
                                    }
                                }
                                else
                                {   //abort the algorithm; invalid format
                                    clean_up(file, current_section, current_name, current_value);
                                    return line_number;
                                }
                            }
                            else if(is_newline(c))
                            {
//TODO: do newline right
                                ++line_number;
                                break;
                            }
                            else if(c == equals_char)
                            {   //unescaped character
                                clean_up(file, current_section, current_name, current_value);
                                return line_number;
                            }
                            c = fgetc(file);
                        }
                        else
                        {   //abort the algorithm; invalid format
                            clean_up(file, current_section, current_name, current_value);
                            return line_number;
                        }
                    }
                    if(feof(file) != 0 && ferror(file) != 0)
                    {
                        clean_up(file, current_section, current_name, current_value);
                        return INI_RETURN_FILE_ERROR;
                    }
                    if(quote != 0 && quote != 3)
                    {   //quotes not closed
                        clean_up(file, current_section, current_name, current_value);
                        return line_number - ((quote == 3) ? 0 : 1);
                    }
                    //remove quote if it exists
                    end_pos = ftell(file);
                    if(quote == 3)
                    {
                        --end_pos;
                    }
                    //decode value
                    if(end_pos < 0)
                    {   //error while reading
                        clean_up(file, current_section, current_name, current_value);
                        return INI_RETURN_UNKNOWN_ERROR;
                    }
                    size_t s = end_pos - begin_pos;
                    //realloc data
                    char* new_str = (char*)realloc(current_value, s + 1);
                    if(new_str == NULL)
                    {   //memory allocation failed
                        clean_up_with_2(file, current_section, current_name);
                        return INI_RETURN_MEMORY_ERROR;
                    }
                    current_value = new_str;
                    //reading string
                    current_value = read_string_from_stream(file, begin_pos, end_pos, current_value, s, &error, quote == 3);
                    if(current_value == NULL)
                    {   //return correct line number on quoted values
                        clean_up(file, current_section, current_name, current_value);
                        if(error == INI_PARSE_ERROR)
                            return line_number - ((quote == 3) ? 0 : 1);
                        else
                            return error;
                    }
                    //jump over ' or "
                    if(c == '\'' || c == '\"')
                    {
                        c = fgetc(file);
                    }
                }
                //call user
                if(handler(current_section, current_name, current_value, data_structure) != 0)
                {
                    clean_up(file, current_section, current_name, current_value);
                    return line_number;
                }
            }
            else if(c == comment_char)
            {   //skip comment and newline
                comment = 1;
                while(feof(file) == 0 && !is_newline(c))
                {
                    c = fgetc(file);
                }
                if(feof(file) != 0 && ferror(file) != 0)
                {
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_FILE_ERROR;
                }
            }
            else
            {
                clean_up(file, current_section, current_name, current_value);
                return line_number;
            }
            if(comment)
            {
                while(feof(file) == 0 && is_newline(c))
                {
                    c = fgetc(file);
                }
                if(feof(file) != 0 && ferror(file) != 0)
                {
                    clean_up(file, current_section, current_name, current_value);
                    return INI_RETURN_FILE_ERROR;
                }
                error = move_stream_pos(file, -1);
                if(error != 0)
                {
                    clean_up(file, current_section, current_name, current_value);
                    return error;
                }
                ++line_number;
                comment = 0;
            }
        }
        if(feof(file) != 0 && ferror(file) != 0)
        {
            clean_up(file, current_section, current_name, current_value);
            return INI_RETURN_FILE_ERROR;
        }
    }
    if(feof(file) != 0 && ferror(file) != 0)
    {
        clean_up(file, current_section, current_name, current_value);
        return INI_RETURN_FILE_ERROR;
    }
    if(clean_up(file, current_section, current_name, current_value) != 0)
        return INI_RETURN_UNKNOWN_ERROR;
    return INI_RETURN_SUCCESS;
}
*/
