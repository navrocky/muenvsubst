# MuEnvSubst

Substitutes environment variables using one of the templating engines, as 
[envsubst](https://www.gnu.org/software/gettext/manual/html_node/envsubst-Invocation.html) does, but using a powerfull templating engine. 

This is a list of supported template engines:

- `inja` - [Inja templates](https://pantor.github.io/inja/)

One of the advantages of this utility is that it is built very small static binary without any dependencies.
Architecture of binary is Linux x86 (32 bit). It can be run in any Linux x86 and x86_64, both new and very old versions.

## Installation

Download and copy binary to any destination specified in `PATH` (~/bin, /usr/local/bin, /usr/bin). 
Do not forget to set execution flag on binary with `chmod`.

```sh
sudo curl https://github.com/navrocky/muenvsubst/releases/download/1.5.0/muenvsubst -Lo /usr/local/bin/muenvsubst
sudo chmod +x /usr/local/bin/muenvsubst
```

## Usage

```
Substitutes environment variables using Inja templating engine, as envsubst 
does. 

USAGE: ./muenvsubst [ -h, --help <arg> ] [ -i, --in <arg> ] [ -d, 
       --include-dir <arg> ] [ -o, --out <arg> ] [ -v, --version ] 

OPTIONAL:
 -h, --help <arg>        Print this help. 

 -i, --in <arg>          Input file 

 -d, --include-dir <arg> The directory where the included files are searched 
                         for. This argument can be specified more than once. 

 -o, --out <arg>         Output file 

 -v, --version           Output version information and exit 
```

## Inja syntax

[Inja](https://pantor.github.io/inja/) inspired by Python's Jinja and supports subset of original Jinja syntax. 

Inja syntax documented [here](https://pantor.github.io/inja/) and original Jinja syntax documented 
[here](https://jinja.palletsprojects.com/en/stable/templates/).

Also supported function [pipe](https://jinja.palletsprojects.com/en/stable/templates/#filters) calling syntax. 
These are equal expressions:

```
{{ split("A,B,C", ",") }}
{{ "A,B,C" | split(",") }}

{{ sh("198c126c-2691-463f-9708-1ee485ce4d68", "sed 's/-//g'") }}
{{ "198c126c-2691-463f-9708-1ee485ce4d68" | sh("sed 's/-//g'") }}
```

## Additional Inja functions

- [error](functions.md#error) - throws an error
- [fromBase64](functions.md#fromBase64) - decode base64 string
- [fromJson](functions.md#fromJson) - parse JSON string to object
- [sh](functions.md#sh) - execute shell script
- [split](functions.md#split) - splits text by delimiter
- [toBase64](functions.md#toBase64) - encode base64 string
- [toBool](functions.md#toBool) - convert any value to boolean
- [toJson](functions.md#toJson) - serialize value to JSON string
- [trim](functions.md#trim) - trims text

## More Inja examples

### Simple variable substitution

```sh
echo "Hello, {{ USER }}!" | muenvsubst
```

then output will be: 

```
Hello, John!
```

### Using variable and function

```sh
muenvsubst <<EOF
{%- set username = upper(USER) -%}
Hello, {{ username }}!
EOF
```

then output will be: 

```
Hello, JOHN!
```
  
### Render conditional block

```sh
USE_GREETER=no USE_GOODBYER=yes muenvsubst << EOF
## if USE_GREETER=="yes"
Hello, {{ USER }}!
## endif
## if USE_GOODBYER=="yes"
Goodbye, {{ USER }}!
## endif
EOF
```

then output will be: 

```
Goodbye, John!
```

### Safe check of a boolean variable

```sh
muenvsubst << EOF
## if default(USE_GREETER, null) | toBool
Hello, {{ USER }}!
## endif
EOF
```

### Using split and loop
  
```sh
USERS="John,Mark,Peter" muenvsubst << EOF
{%- for user in split(USERS,",") -%}
Hello, {{ user }}!
{%- endfor -%}
EOF
```

then output will be: 

```
Hello, John!
Hello, Mark!
Hello, Peter!
```

### Includes

Content of `greeter.j2` file:
```
Hello, {{ USER }}!
```

Render template:

```sh
muenvsubst << EOF
## include "greeter.j2"
EOF
```

then output will be: 

```
Hello, John!
```
