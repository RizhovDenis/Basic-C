# Descriptions

## Compile code
I used libraries for format data so need to launch them.<br />
Just write it in terminal:
```
clang main.c -ljson-c -lyaml -o main
```
## Download library for parsing
* Json parser:
```
sudo apt install libjson-c-dev
```
* Xml parser: <br />
I solved make parser myself. <br />
Also we can dowload library:
```
sudo apt-get install libxml2-dev
```
If you received error "file not found", write next commands:
```
find /usr -name parser.h
```
```
sudo ln -s /usr/include/libxml2/libxml /usr/include/libxml
```

* Yaml parser:
```
sudo apt-get install libyaml-dev
```
