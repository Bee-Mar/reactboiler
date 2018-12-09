install: reactboiler
		sudo cp reactboiler /usr/local/bin
		sudo chown root:root ../man/reactboiler.1.gz
		sudo chmod 644 ../man/reactboiler.1.gz
		sudo cp ../man/reactboiler.1.gz /usr/share/man/man1/

reactboiler: main.c reactboiler.c reactboiler.h
		gcc -g main.c reactboiler.c reactboiler.h -o reactboiler

clean:
		rm -rf reactboiler
