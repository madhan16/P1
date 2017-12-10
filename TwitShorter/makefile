##The way this works is that you first write the name of the file before the : that you want to make.
##Then you write what the file needs after : to be made, and the under it you write what to write in the console
##when making the file

twitie-tagger/TwitShorter.exe: tweetsetup.o abbreviate.o synonyms.o hotspots.o main.c 
	gcc tweetsetup.o abbreviate.o synonyms.o hotspots.o main.c -o twitie-tagger/TwitShorter

tweetsetup.o: src/tweetsetup.c includes/tweetsetup.h includes/StructsAndConsts.h
	gcc -c src/tweetsetup.c

abbreviate.o: src/abbreviate.c includes/abbreviate.h includes/tweetsetup.h includes/StructsAndConsts.h
	gcc -c src/abbreviate.c

synonyms.o: src/synonyms.c includes/synonyms.h includes/StructsAndConsts.h
	gcc -c src/synonyms.c

hotspots.o: src/hotspots.c includes/hotspots.h includes/StructsAndConsts.h
	gcc -c src/hotspots.c

clean:
	$(RM) *.o program.exe *~
