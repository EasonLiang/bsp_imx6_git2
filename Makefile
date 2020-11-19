HELP2MAN = help2man -N

all:

check: check-guess check-sub

manpages: doc/config.guess.1 doc/config.sub.1

check-guess:
	cd testsuite && bash config-guess.sh && rm uname

check-sub:
	cd testsuite && bash config-sub.sh

shellcheck:
	shellcheck -f gcc config.guess config.sub testsuite/*.sh

sort:
	for f in config-guess.data config-sub.data ; do sort testsuite/$$f -o testsuite/$$f; done

doc/%.1: % doc/%.x
	$(HELP2MAN) --include=doc/$<.x --output=$@ ./$<
