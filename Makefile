files_cleanup_list = canvascii canvascii.o README.html sujet.html

canvascii: canvascii.c
	gcc -o canvascii -std=c11 -Wall -Wextra canvascii.c

html: README.html sujet.html ./misc/github-pandoc.css

README.html:
	pandoc README.md -s -o README.html -c ./misc/github-pandoc.css

sujet.html:
	pandoc sujet.md -s -o sujet.html -c ./misc/github-pandoc.css

test:
	bats check.bats

.PHONY: clean

clean:
	rm -f $(files_cleanup_list)
