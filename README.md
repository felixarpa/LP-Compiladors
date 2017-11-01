# LP-Compiladors
Compilers, Programming Languages (FIB)

## Mountains

First of all you'll need to intall **pccts** (`antlr` and `dlg`) as explanied in the [pccts/README](https://github.com/felixarpa/LP-Compiladors/blob/master/pccts/README) file.

Once you have **pccts** intalled run:

```bash
➜ antlr -gt mountains.g
➜ dlg -ci parser.dlg scan.c
➜ g++ -o mountains mountains.c scan.c err.c
```

This last command may generate a lot of *warnings*. Run the program doing:

```bash
➜ ./mountains
```

You can check the inputs inputs and the outputs in the files:

- [input1.txt](https://github.com/felixarpa/LP-Compiladors/blob/master/input1.txt)
- [output1.txt](https://github.com/felixarpa/LP-Compiladors/blob/master/output1.txt)
- [input2.txt](https://github.com/felixarpa/LP-Compiladors/blob/master/input2.txt)
- [output2.txt](https://github.com/felixarpa/LP-Compiladors/blob/master/output2.txt)


Enjoy ;)