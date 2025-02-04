# MuEnvSubst

Substitutes environment variables using one of the templating engines, as 
[envsubst](https://www.gnu.org/software/gettext/manual/html_node/envsubst-Invocation.html) does. 

This is a list of supported template engines:

- `inja` - [Inja templates](https://pantor.github.io/inja/)

One of the advantages of this utility is that it is built in a small static binary without any dependencies.
Architecture of binary is Linux x86 (32 bit). It can be run in docker from scratch.

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

Additional functions:

- Split text by delimiter:

  ```
  split(text: string, delimiter: string): string
  ```
  
- Convenient convert boolean variable to boolean type:
  
  ```
  varToBool(varName: string): boolean
  ```
  
  This function supports strings "true", "yes", "on", "1" for `true` value, other values supposed to be a `false` value. 
  
  Example: `set DO_SMTH = varToBool("DO_SMTH")`
  
- Throw error:
  
  ```
  error(message: string)
  ```

## Mstch examples

Simple variable substitution:

```sh
echo "Hello, {{ USER }}!" | muenvsubst
```

then output will be: 

```
Hello, John!
```

## Inja examples

- Simple variable substitution:
  ```sh
  echo "Hello, {{ USER }}!" | muenvsubst
  ```

  then output will be: 

  ```
  Hello, John!
  ```

- Using variable and function:

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
  
- Render conditional block:

  ```sh
  USE_GREETER=no USE_GOODBYER=yes muenvsubst << EOF
  {%- if USE_GREETER=="yes" -%}
  Hello, {{ USER }}!
  {%- endif -%}
  {%- if USE_GOODBYER=="yes" -%}
  Goodbye, {{ USER }}!
  {%- endif -%}
  EOF
  ```

  then output will be: 
  
  ```
  Goodbye, John!
  ```

- Using split and loop:
  
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
