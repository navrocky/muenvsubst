## Additional Inja functions

### error

Throws an error.
  
```
error(message: string)
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

### trim

Trims text. Removes spaces and new lines from begin and end of text.
  
```
trim(text: string): string
```

Example usage:
```
{{ "  some text  " | trim }} 
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
