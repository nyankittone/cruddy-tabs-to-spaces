# What is this?

This is a simple little C program that converts tabs to spaces in a file, and prints the result
to stdout. That's about it. Not even that well-written, tbh, but whatever.

# How do I use it?
 
Invoke the program on the command line, with the files you want to expand as parameters. It will
then print the result out for each file in the order you typed the files in.

The program supports a few command options. They do the following:
+ `-t`: convert spaces to tabs
+ `-s`: convert tabs to spaces
+ `-n <number>`: set how many spaces long a tab should be
+ `--`: interpret any following arguments in the program invokation as positional arguments

# How do I build it?

+ download the git repo
+ `cd` into the directory for it  
+ run `make`

