.PHONY: exc clean

exc:
	make -f MakeMYLS
	make -f MakeMYCAT
	make -f MakeMYCP
	make -f MakeMYSH

clean:
	make -f MakeMYLS clean
	make -f MakeMYSH clean
	make -f MakeMYCAT clean
	make -f MakeMYCAT clean

