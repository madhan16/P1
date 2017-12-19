# Semantisk komprimering af tweets
af gruppe A310b, Aalborg Universitet - Software 2017
## Kort vejledning til kompilering af programmet
### Java
Det er påkrævet at have jdk eller jre installeret og systemmiljøvariablen `JAVA_HOME` sat korrekt. Det kan på Windows være påkrævet at bruge forkortelsen `Progra~1` for `Program Files` eller `Progra~2` for `Program Files (x86)`.
### Files path
Hvis MAC, ændre 3 stiger i `includes/StructsAndConsts.h` til `files/MAC` i stedet for `files/WIN`

## Makefile
Programmet indeholder en makefile, som kaldes i den yderste mappe P1, og genererer en exe fil i mappen twitie-tagger, som skal køres inde i denne mappe for twitie-tagger fungerer. 
