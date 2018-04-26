.PHONY: exc clean

exc:
	make -f MakeMYLS
	make -f MakeMYSH
	make -f MakeMYCAT
clean:
	make -f MakeMYLS clean
	make -f MakeMYSH clean
	make -f MakeMYCAT clean
