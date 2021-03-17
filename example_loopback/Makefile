TARGET = main

$(TARGET): main.o slip.a
	gcc $^ -o $@

main.o: main.c
	gcc -c -I.. $< -o $@

slip.a: slip.o
	ar rcs $@ $^

slip.o: ../slip.c ../slip.h
	gcc -c -o $@ $<

clean:
	rm -f *.o *.a $(TARGET)
