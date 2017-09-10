# The .INI format

The .INI format has no clear specification. Most of the .INI files are ASCII encoded and allow comments at the beginnings of the line. Some readers also support more fancy features like UTF-8 or multi line values.

The here implemented .INI format allow comments anywhere in the file (as long as it doesn't invalidate other parts of the file).

Empty lines are ignored but counted.

Further component specific details are described in the [Components](#Components) section.

##Components

The .INI format has 3 key components:
* [sections](#Sections)
* [key-value-pairs](#Key Value pairs)
* [comments](#Comments)

### Sections

Sections always begin with a ```[``` and end with a ```]``` with the section name in between. The section name mustn't contain neither [whitespaces](whitespace characters) nor [Special characters](Special characters) and UTF-8 sequences.
Furthermore the whole section name has to be on one line. 

It is allowed to insert non-newline whitespaces before and after the ```[``` and ```]```.

A comment on the same line is only allowed if it is placed behind the ```]```.

### Key Value pairs

Key value pairs are seperated with a user defined equals character (default is ```=```) in the style of ```key=value```. Before and after the key, the value and the seperator [spacing characters](#spacing characters) are allowed.

#### Key

The key mustn't contain neither [whitespaces](whitespace characters) nor [Special characters](Special characters) and UTF-8 sequences.

#### Value

The whitespaces before and after the value are removed by the parser.

The value may be enclosed in ```'``` or ```"``` (whitespaces inside are not removed). Values may be split over multiple lines by puttin ```\``` followed by a whitespace at the end of a line and may contain all characters although [special characters](#Special characters) need to be [escaped](#Escaping).

### Comments

Comments start with a user defined comment character (default is ```;```). After this any character is allowed.

## Characters

### Special character

Are:
* ```[```
* ```]```
* ```\```
* ```'```
* ```"```
* the comment character
* the equals character

### Newline characters

Are either:
* ```\n```
* ```\r``` or
* a combination of these.

```\n``` and ```\r``` are countes as a newline but the combinations ```\n\r``` and ```\r\n``` are only counted as a single newline.

### Spacing characters

These are either:
* spaces or
* tabs.

### Whitespace characters

Are either:
* [Newline characters](#Newline characters) or
* [Whitespace characters](#Whitespace characters)

### Escaping

See how to escape certain characters in the table below:

Character | Escaped character
----------|------------------
```\```|```\\```
```[```|```\[```
```]```|```\]```
```'```|```\'```
```"```|```\"```
```0x7``` (Bell)|```\a```
```0x8``` (Backspace)|```\b```
```0x9``` (Horizontal Tab)|```\t```
```0xA``` (Line Feed)|```\n```
```0xB``` (Vertical Tab)|```\v```
```0xC``` (Form Feed)|```\f```
```0xD``` (Carriage Return)|```\r```
Unicode|```\x??????``` or ```\X??????```
Equals character|```\Equals character```
Comment character|```\Comment character```
