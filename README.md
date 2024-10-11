<h1>MIPS core instruction disassembler using C++</h1>

<i>Project 1 for my Fall 2024 Electrical & Computer Engineering (ECE) Principles of Computer Architecture class</i>
<br>
<p>The program reads an *.obj file where each line contains a 32-bit instruction encoded as 8 hexadecimal digits without a leading “0x”.
Each instruction is turned into it's MIPS equivalent. Once all the instructions are decoded, the program the creates a *.s file with
assembly instructions.</p>

<h2>Specifications</h2>

-  Decode core instructions excluding: j, jal, jr
-  Uses registers $t0 to $t9 and $s0 to $s7 for temporary and saved respectively

<h2>Running Program on Windows</h2>

The program is a command line app, so the program requires the files to be place in command.

```
cd \path\to\program
myDisassembler.exe <filename>.obj
```

<h3>Multiple files</h3>

```
cd \path\to\program
myDisassembler.exe <filename1>.obj <filename2>.obj <filename3>.obj 
```
