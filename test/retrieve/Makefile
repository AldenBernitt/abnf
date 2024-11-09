maketest = ~/maketest

incswitch = -I/home/alden/C/projects/abnf/include/ -I/home/alden/C/include/

objdir = ~/C/projects/abnf/obj/
customobjdir = ~/C/include/obj/

chstream_o = $(customobjdir)chstream.o
varr_o = $(customobjdir)varr.o
strbuffer_o = $(customobjdir)strbuffer.o
arrmem_o = $(customobjdir)arrmem.o
convert_o = $(customobjdir)convert.o
strutil_o = $(customobjdir)strutil.o

tests_c = tests.c
tester_c = tester.c
tester_o = tester.o

abnf_o = $(objdir)abnf.o

objs = $(abnf_o) $(chstream_o) $(varr_o) $(strbuffer_o) $(arrmem_o)\
		$(convert_o) $(strutil_o)

test : $(tester_o) $(objs)
	gcc $(tester_o) $(objs) -o test

$(tester_o) : $(tester_c) $(objs)
	gcc $(incswitch) -c $(tester_c) -o $(tester_o)

$(tester_c) : $(tests_c)
	$(maketest) $(tests_c)

.PHONY : clean

clean :
	rm test $(tester_c) $(tester_o)
