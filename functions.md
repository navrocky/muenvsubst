## Additional Inja functions

### error

Throws an error.
  
```
error(message: string)
```

### fromBase64

Decode base64 string.

```
fromBase64(base64: string): string
```

### fromJson

Parse JSON string to object.
  
```
fromJson(json: string): json
```

Example usage: 

```
## set JSON = fromJson("{\"key\":\"value\"}")
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

### toBase64

Encode base64 string.

```
toBase64(text: string): string
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

### toJson

Serialize value to JSON string. 
JSON indented with N spaces if indent parameter provided.
  
```
toJson(value: any, indent: int?): string
```

Example usage: 

```
## set INDENTED_JSON = "{\"key\":\"value\"}" | fromJson | toJson(2)
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

> [!WARNING]
> `varToBool` is deprecated because it does not work with variables defined with the `set` instruction.
> Use `default(VAR_NAME, null) | toBool` instead.
  
```
varToBool(varName: string): boolean
```

This function supports strings "true", "yes", "on", "1" for `true` value, other values supposed to be a `false` value. 

Example usage: 

```
## set DO_SMTH = varToBool("DO_SMTH")
```
