# Small Vim

VIM editor Project.

for C programming(CSE2035) 2016 Fall, Sogang Univ.

## Functions

* Basic functions of vim editor
	* Edit mode
	* Indirect Addressing Mode
	* command mode (i, ESC, dd, d, h, j, k, l)
	* ex-command mode (: w,: q,: wq,: set nu,: $ line_num, / $ pattern)
	* customizing for .vimrc
	* Save File

## Dependencies and Limitation

* Executable OS: Linux. (Not Support Windows, Mac)
* Fixed 80X24 Size Layout (Not Support Layout-Responsive Editor)
* Not Support ↑,←,↓,→. must use h,j,k,l

## Getting Started 

```shell
$ make
```

```shell
$ ./myvi your_file_name.txt
```
