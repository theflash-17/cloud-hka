all: run

test.exe: aont.cs
	@ mcs aont.cs -out:test

run: test.exe
	./test /n:1 lobj.txt

clean:
	@ -rm -f test.exe

remake :
	@ $(MAKE) clean
	@ $(MAKE)