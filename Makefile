VPATH = src:include
objects = main.o instruction.o reservation_station.o tomasulo_sim.o
target = xqts.exe

$(target) : $(objects)
	g++ $^ -o $@ -Iinclude
#	@move $(target) ./bin
	del $(objects)

$(objects) : %.o : %.cpp
	g++ -c $? -o $@ -Iinclude -O2 -w


.PHONY : clean
clean :
	del $(target) $(objects)