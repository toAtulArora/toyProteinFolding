# toyProteinFolding

## Usage
1. Update the makefile according to your system (clang or whatever)
2. compile it with ```make```
3. To run for a sequence, try ```./main HAHHHAHAH```
4. To run for a batch saved in a text file,

(a) Install GNU parallel (in ubuntu, ```sudo apt install parallel```)

(b) Save your sequences in a text file called, say, ```testSequences.txt```

(c) Now use ```cat testSequences.txt | parallel -j+0 --eta './main {} >> output.log'``` and it will save the result in ```output.log``` or just run the script called ```./batchMain```

(d) Some explanation of what happened in the last step

* The bash script pipes the output of the text file into the parallel command.

* The ```-j+0``` tells parallel to use as many cores as in the CPU + 0 extra.

* The ```--eta``` will tell you how long it might take.

* The ```'./main``` is the command that it will run. ```{}``` will fetch one string at a time from the stream inputted to parallel. ```>> output.log``` would append the output from the program into the file.


## Sample Output
```


Protein Sequence:HHHAH
Best Score:1
Possible Configuration:
[
    H - A   
        |   
H - H - H   
            
]




Protein Sequence:HHHAHAH
Best Score:1
Possible Configuration:
[
    H   
    |   
    A   
    |   
A - H   
|       
H - H   
    |   
    H   
        
]




Protein Sequence:HHAHHAHHH
Best Score:3
Possible Configuration:
[
H - H - H   
        |   
H - H - A   
|           
A - H       
    |       
    H       
            
]




Protein Sequence:HAHHHAHAHA
Best Score:2
Possible Configuration:
[
    A       
    |       
    H       
    |       
    A       
    |       
    H - A   
        |   
H - H - H   
|           
A - H       
            
]



```