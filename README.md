# tsviewer

view tsv file line by line with header in json/dict mode

This project have several program in Python3, C++ and Java. And there is a demo tsv file in ./data directory.

Usage of Python3 version:

    ✗ python3 python/tsviewer.py -h
    usage: tsviewer.py [-h] -f FILE [-l LINE] [-c COLUMN]

    optional arguments:
      -h, --help            show this help message and exit
      -f FILE, --file FILE  file to read
      -l LINE, --line LINE  show X line
      -c COLUMN, --column COLUMN
                            only show column x,y,z


    ✗ python3 python/tsviewer.py -f data/data.tsv -l 2 -c "Study_set,Genome_type"
    {'Genome_type': 'Isolate', 'Study_set': 'HBC'}
    {'Genome_type': 'Isolate', 'Study_set': 'HBC'}

Usage of Java version:

     ✗ java -jar java/tsviewer/target/tsviewer-1.0.jar
    usage: TSVViewer [-c <arg>] [-f <arg>] [-l <arg>]
     -c,--column <arg>   only show column x,y,z
     -f,--file <arg>     file to read
     -l,--line <arg>     show X line



    ✗ java -jar java/tsviewer/target/tsviewer-1.0.jar -l 2 -f data/data.tsv -c "Study_set,Genome_type"
    {
      "Study_set" : "HBC",
      "Genome_type" : "Isolate"
    }
    {
      "Study_set" : "HBC",
      "Genome_type" : "Isolate"
    }

Usage of C++ version:

    ✗ xmake # build the source
    ✗ ./build/macosx/x86_64/release/tsviewer -l 2 -f data/data.tsv -c "Study_set,Genome_type"
