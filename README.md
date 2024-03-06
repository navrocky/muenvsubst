# MuEnvSubst

Substitutes environment variables using [Mustache template](https://mustache.github.io/) syntax like 
[envsubst](https://www.gnu.org/software/gettext/manual/html_node/envsubst-Invocation.html) do.

One of the advantages of this utility is that it is built in a small static binary file without any dependencies and 
can be run in a docker from scratch.

Installation:

```sh
curl https://github.com/navrocky/muenvsubst/releases/download/1.0.1/muenvsubst -Lo /usr/local/bin/muenvsubst
chmod +x /usr/local/bin/muenvsubst
```

Usage:

```sh
echo "Hello, {{ USER }}!" | muenvsubst
```

then output will be: 

```
Hello, John!
```
