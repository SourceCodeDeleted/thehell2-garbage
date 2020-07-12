edit : edit_map.o cel.o map.o mpq_lib.o monster.o dun.o global_allegro.o write.o
	gcc -Wall -O9 -pedantic -o edit_dun edit_map.o cel.o map.o mpq_lib.o write.o monster.o dun.o global_allegro.o -lalleg
join : join_dun.o write.o global_allegro.o dun.o mpq_lib.o map.o cel.o monster.o
	gcc -Wall -O9 -pedantic -o join_dun join_dun.o mpq_lib.o write.o global_allegro.o dun.o map.o cel.o monster.o -lalleg
split : split_dun.o write.o global_allegro.o dun.o mpq_lib.o map.o cel.o monster.o
	gcc -Wall -O9 -pedantic -o split_dun split_dun.o mpq_lib.o write.o global_allegro.o dun.o map.o cel.o monster.o -lalleg
extract : extract_dun.o write.o global_allegro.o dun.o mpq_lib.o map.o cel.o monster.o
	gcc -Wall -O9 -pedantic -o extract_dun extract_dun.o mpq_lib.o write.o global_allegro.o dun.o map.o cel.o monster.o -lalleg

edit_map.o : edit_map.c dun.h map.h monster.h mpq_lib.h cel.h global_allegro.h
	gcc -Wall -O9 -pedantic -c edit_map.c
cel.o : cel.c cel.h mpq_lib.h
	gcc -Wall -O9 -pedantic -c cel.c
map.o : map.c cel.h map.h mpq_lib.h monster.h
	gcc -Wall -O9 -pedantic -c map.c
mpq_lib.o : mpq_lib.c mpq_lib.h
	gcc -Wall -O9 -pedantic -c mpq_lib.c
monster.o : monster.c monster.h mpq_lib.h cel.h
	gcc -Wall -O9 -pedantic -c monster.c
dun.o : dun.c dun.h mpq_lib.h map.h
	gcc -Wall -O9 -pedantic -c dun.c
write.o : write.c write.h
	gcc -Wall -O9 -pedantic -c write.c
global_allegro.o : global_allegro.c global_allegro.h mpq_lib.h
	gcc -Wall -O9 -pedantic -c global_allegro.c
split_dun.o : split_dun.c write.h dun.h global_allegro.h mpq_lib.h
	gcc -Wall -O9 -pedantic -c split_dun.c
join_dun.o : join_dun.c write.h dun.h global_allegro.h mpq_lib.h
	gcc -Wall -O9 -pedantic -c join_dun.c
extract_dun.o : extract_dun.c write.h dun.h global_allegro.h mpq_lib.h
	gcc -Wall -O9 -pedantic -c extract_dun.c
clean :
	del *.o *.bak