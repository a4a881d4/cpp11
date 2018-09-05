if [ "%1" == "install" ]; then
	cp usrtos/scripts/post-commit .git/hooks/
elif [ "%1" == "remove" ]; then
	rm .git/hooks/post-commit 
fi
