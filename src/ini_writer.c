#include "../include/ini_parser.h"

/**
 * Opens an .INI file for write and writes the byte order mark if needed.
 * The file pointer mustn't be NULL!
 * @param file pointer
 * @param path
 * @param equals character
 * @param comment character
 * @param UTF-8 mode (INI_UTF8_MODE_FORBID, INI_UTF8_MODE_ALLOW,
 * INI_UTF8_MODE_ALLOW_WITH_BOM, INI_UTF8_MODE_ESCAPE)
 * @return 1 on success; 0 on failure
 */
int ini_open_file(struct ini_file *file, const char *path, char equals,
                  char comment, int utf8_mode) {
  if (!file || is_special_character(equals, '\0', '\0') ||
      is_special_character(comment, '\0', '\0'))
    return 0;

  // open file
  file->file = fopen(path, "w");
  if (!file->file)
    return 0;

  // write byte order mark if needed
  if (utf8_mode == INI_UTF8_MODE_ALLOW_WITH_BOM) {
    const char BOM[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
    if (fwrite(BOM, 1, 3, file->file) != 3)
      return 0;
  }

  // set options
  file->utf8_mode = utf8_mode;
  file->equals = equals;
  file->comment = comment;

  return 1;
}

/**
 * Closes an .INI file. The file pointer must be valid!
 * @param file pointer
 * @return 1 on success; 0 on failure
 */
int ini_close_file(struct ini_file *file) {
  if (!file)
    return 0;

  if (fclose(file->file))
    return 0;

  file->file = NULL;

  return 1;
}

/**
 * Writes a section name to a file. The section and file pointers must be valid!
 * The section name mustn't include whitespace character, other escape
 * characters!
 * THe section name also mustn't be empty!
 * @param file pointer
 * @param section name
 * @return 1 on success; 0 on failure
 */
int ini_write_section(const struct ini_file *file, const char *section) {
  if (!file || !section)
    return 0;

  if (contains_escape_characters(section, file->comment, file->equals) ||
      contains_utf8(section))
    return 0;

  size_t len = strlen(section);

  if (!len)
    return 0;

  if (fwrite("[", 1, 1, file->file) != 1)
    return 0;
  if (fwrite(section, 1, len, file->file) != len)
    return 0;
  if (fwrite("]\n", 1, 2, file->file) != 2)
    return 0;

  return 1;
}

/**
 * Converts a binary value to two ASCII hex digits.
 * @param binary value
 * @return two ASCII digits
 */
uint16_t binary_to_hex_digits(char c) {
  char ret[2];
  int i;

  ret[0] = (c & 0x0F);
  ret[1] = ((c & 0xF0) >> 4);

  for (i = 0; i < 2; ++i) {
    if (ret[i] <= 9)
      ret[i] += '0';
    else
      ret[i] += 'A' - 10;
  }

  return *((uint16_t *)ret);
}

/**
 * Converts UTF-8 characters into an unicode character.
 * //NOTE: this uses the UTF-8 standard since 2003.
 * @param UTF-8 code
 * @return unicode character; -1 on failure
 */
uint32_t utf8touc(uint32_t code) {
  // NOTE: this uses the UTF-8 standard since 2003
  unsigned char c[4] = {0xFF, 0xFF, 0xFF, 0xFF};
  unsigned char *code_ch = (unsigned char *)&code;

  if (!(code & 0xFFFFFF80)) {
    c[0] = (0x7F & code_ch[0]);
    c[1] = 0;
    c[2] = 0;
    c[3] = 0;
  } else if (!((code ^ 0x0000DFBF) & 0xFFFFE0C0)) {
    c[0] = ((0x03 & code_ch[1]) << 6) | (0x3F & code_ch[0]);
    c[1] = ((0x1F & code_ch[1]) >> 2);
    c[2] = 0;
    c[3] = 0;
  } else if (!((code ^ 0x00EFBFBF) & 0xFFF0C0C0)) {
    c[0] = ((0x03 & code_ch[1]) << 6) | (0x3F & code_ch[0]);
    c[1] = ((0x0F & code_ch[2]) << 4) | ((0x3F & code_ch[1]) >> 2);
    c[2] = 0;
    c[3] = 0;
  } else if (!((code ^ 0xF7BFBFBF) & 0xF8C0C0C0)) {
    c[0] = ((0x03 & code_ch[1]) << 6) | (0x3F & code_ch[0]);
    c[1] = ((0x0F & code_ch[2]) << 4) | ((0x3F & code_ch[1]) >> 2);
    c[2] = ((0x07 & code_ch[3]) << 2) | ((0x3F & code_ch[2]) >> 4);
    c[3] = 0;
  }

  return *((uint32_t *)c);
}

/**
 * Returns the length of an UTF-8 sequence according to the first byte.
 * @param first byte
 * @return length (0 if the first byte is invalid)
 */
size_t get_utf8_len(char c) {
  if (!(c & 0x80))
    // 2 byte UTF-8 sequence
    return 1;
  else if (!((c ^ 0xDF) & 0xE0))
    // 2 byte UTF-8 sequence
    return 2;
  else if (!((c ^ 0xEF) & 0xF0))
    // 3 byte UTF-8 sequence
    return 3;
  else if (!((c ^ 0xF7) & 0xF8))
    // 3 byte UTF-8 sequence
    return 4;

  // invalid UTF-8 sequence
  return 0;
}

/**
 * Verifies that a given string consists only of valid UTF-8 sequences or
 * doesn't contain any UTF-8 sequence.
 * @param string
 * @param flag whether to allow UTF-8 or not
 * @return 1 if the string is valid; 0 otherwise
 */
int ini_verify_utf8(const char *str, int allow_utf8) {
  if (!str)
    return 0;

  size_t pos = 0;
  size_t len = strlen(str);
  while (pos < len) {
    if ((str[pos] & 0x7F) == str[pos]) {
      ++pos;
      continue;
    } else if (allow_utf8) {
      size_t utf8_len = get_utf8_len(str[pos]);
      if (!utf8_len || utf8_len + pos > len)
        // invalid sequence
        return 0;

      utf8_len += pos - 1;

      for (++pos; pos <= utf8_len; ++pos) {
        if ((str[pos] ^ 0x80) & 0xC0)
          return 0;
      }
    } else {
      // UTF-8 sequences not allowed
      return 0;
    }
  }

  return 1;
}

/**
 * Writes a value string to a file. Please note that the output file will
 * contain junk data if this method fails.
 * @param file struct (mustn't be NULL)
 * @param value string
 * @return 1 if the string is valid; 0 otherwise
 */
int write_value(const struct ini_file *file, const char *value) {
  if (!file || !value)
    return 0;

  size_t len = strlen(value);
  size_t i;

  for (i = 0; i < len; ++i) {
    char c = value[i];

    if (is_special_character(c, file->comment, file->equals) || is_newline(c) ||
        c == '\a' || c == '\b' || c == '\f' || c == '\v') {
      // write escaped characters
      char escaped_str[2] = "\\ ";
      escaped_str[1] = c;

      if (fwrite(escaped_str, 1, 2, file->file) != 2)
        return 0;

    } else if (is_whitespace(c) || c >= 32) {
      // normal character
      if (fwrite(&c, 1, 1, file->file) != 1)
        return 0;

    } else if (file->utf8_mode) {

      if (file->utf8_mode == INI_UTF8_MODE_ESCAPE) {
        // escape UTF-8
        char bin_str[4] = {0, 0, 0, 0};
        size_t j;
        size_t bin_len = get_utf8_len(c);

        if (!bin_len || bin_len + i > len)
          // invalid length
          return 0;

        // read UTF-8 character
        for (j = 0; j < bin_len; ++j)
          bin_str[bin_len - j - 1] = value[i + j];
        i += bin_len - 1;

        // convert character
        uint32_t hex = utf8touc(*((uint32_t *)bin_str));
        // abort if conversion failed
        if (hex == (uint32_t)-1)
          return 0;

        char *hex_str = (char *)&hex;

        char escaped_str[8] = "\\x000000";
        char *current;

        // convert binary to hex digits
        for (j = 0; j < 3; ++j) {
          uint16_t hex_digits = binary_to_hex_digits(hex_str[j]);
          current = (char *)&hex_digits;

          // store converted characters
          escaped_str[7 - j * 2] = current[0];
          escaped_str[7 - j * 2 - 1] = current[1];
        }

        // write everything into a file
        if (fwrite(escaped_str, 1, 8, file->file) != 8)
          return 0;
      } else {
        // INI_UTF8_MODE_ALLOW & INI_UTF8_MODE_ALLOW_WITH_BOM
        // UTF-8 sequences allowed; no need to escape anything
        size_t bin_len = get_utf8_len(c);

        if (!bin_len || bin_len + i > len)
          // invalid length
          return 0;

        if (fwrite(value + i, 1, bin_len, file->file) != bin_len)
          return 0;

        // skip already read characters
        i += bin_len - 1;
        continue;
      }
    } else {
      // invalid character
      return 0;
    }
  }
  return 1;
}

/**
 * Writes a name - value pair to a file. The name and file pointers mustn't be
 * NULL!
 * @param file pointer
 * @param name
 * @param value
 * @return 1 on success; 0 on failure
 */
int ini_write_name_value(const struct ini_file *file, const char *name,
                         const char *value) {
  if (!file || !name)
    return 0;

  if (contains_escape_characters(name, file->comment, file->equals) ||
      contains_utf8(name))
    return 0;

  size_t len = strlen(name);

  if (!len)
    return 0;

  // write name
  if (fwrite(name, 1, len, file->file) != len)
    return 0;

  if (value) {
    char equals_str[2] = " \"";
    equals_str[0] = file->equals;

    // if value exists, write equals character and write value
    if (fwrite(equals_str, 1, 2, file->file) != 2)
      return 0;

    if (!write_value(file, value))
      return 0;

    // end quotes
    if (fwrite("\"", 1, 1, file->file) != 1)
      return 0;
  }

  // write newline
  if (fwrite("\n", 1, 1, file->file) != 1)
    return 0;

  return 1;
}
