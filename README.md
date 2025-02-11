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
sudo curl https://github.com/navrocky/muenvsubst/releases/download/1.3.0/muenvsubst -Lo /usr/local/bin/muenvsubst
sudo chmod +x /usr/local/bin/muenvsubst
```

## Usage

```
Substitutes environment variables using one of the templating engines, as 
envsubst does. 

USAGE: ./muenvsubst [ -h, --help <arg> ] [ -V, --version ] 

OPTIONAL:
 -h, --help <arg> Print this help. 

 -V, --version    Output version information and exit
```

## Inja syntax

[Inja](https://pantor.github.io/inja/) inspired by Python's Jinja and supports subset of original Jinja syntax. It 
much more powerfull then Mustache. 

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

### sh

Execute shell script with provided stdin and returned stdout.

```
sh(stdin: string?, command: string): string
```

Example usage:
```
ID={{ "198c126c-2691-463f-9708-1ee485ce4d68" | sh("sed 's/-//g'") }}
GUID={{ sh("uuidgen") | upper | trim }}
```

### split

Splits text by delimiter.

```
split(text: string, delimiter: string): string
```

### trim

Trims text. Removes spaces and new lines from begin and end of text.
  
```
trim(text: string): string
```

Example usage:
```
{{ "  some text  " | trim }} 
```

### toBool

Convenient convert any value to boolean type.
  
```
toBool(value: any): boolean
```

This function supports strings "true", "yes", "on", "1" for `true` value, other values supposed to be a `false` value. 

Example usage:
```
{{ "  Yes  " | toBool }}
```

### varToBool

Convenient convert boolean variable to boolean type.
  
```
varToBool(varName: string): boolean
```

This function supports strings "true", "yes", "on", "1" for `true` value, other values supposed to be a `false` value. 

Example usage: 

```
## set DO_SMTH = varToBool("DO_SMTH")
```

### parseJson

Parse JSON string to object.
  
```
parseJson(json: string): json
```

Example usage: 

```
## set JSON = parseJson("{\"key\":\"value\"}")
```

### error

Throws an error.
  
```
error(message: string)
```

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
