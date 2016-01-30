# Makefile for Git
# Please import this to the bottom of your repos

pull :
	git $@

add : pull
	git $@ --all
	git status

commit : add
	git $@

push : commit
	git $@

.PHONY : pull add commit push
