CFLAGS = -c -g
LFLAGS =

incdir = include/
srcdir = src/
objdir = obj/
customincdir = /home/alden/C/include/
customobjdir = $(customincdir)obj/
incswitch = -I$(incdir) -I$(customincdir)

arrmem_o = $(customobjdir)arrmem.o
varr_h = $(customincdir)varr.h
varr_o = $(customobjdir)varr.o
chstream_h = $(customincdir)chstream.h
chstream_o = $(customobjdir)chstream.o
strbuffer_h = $(customincdir)strbuffer.h
strbuffer_o = $(customobjdir)strbuffer.o
convert_h = $(customincdir)convert.h
convert_o = $(customobjdir)convert.o
strutil_h = $(customincdir)strutil.h
strutil_o = $(customobjdir)strutil.o

abnf_h = $(incdir)abnf.h
abnf_c = $(srcdir)abnf.c
abnf_o = $(objdir)abnf.o
abnferrcode_h = $(incdir)abnf_errcode.h
abnfdefs_h = $(incdir)abnf_defs.h
abnfresult_c = $(srcdir)abnf_result.c
abnfdefine_c = $(srcdir)define/abnf_define.c
abnfparse_c = $(srcdir)parse/abnf_parse.c
abnfterminals_c = $(srcdir)define/abnf_terminals.c
abnfdefinehelper_c = $(srcdir)define/abnf_define_helper.c
abnfoptimize_c = $(srcdir)define/optimize/abnf_optimize.c
abnfoptimizerep_c = $(srcdir)define/optimize/abnf_optimize_rep.c
abnfoptimizecat_c = $(srcdir)define/optimize/abnf_optimize_cat.c
abnfnormalize_c = $(srcdir)define/abnf_normalize.c
abnfdefineelements_c = $(srcdir)/define/abnf_define_elements.c
parsenode_c = $(srcdir)/parse/parsenode.c
abnfwrite_c = $(srcdir)/parse/abnf_write.c

customobjs = $(arrmem_o) $(varr_o) $(chstream_o) $(strbuffer_o) $(convert_o)\
        $(strutil_o)

objs = $(objdir)main.o $(abnf_o)

main : $(objs)
	gcc $(LFLAGS) $(objs) $(customobjs) -o main

$(objdir)main.o : main.c $(abnf_h)
	gcc $(incswitch) $(CFLAGS) main.c -o $(objdir)main.o

$(abnf_o) : $(abnf_c) $(abnf_h) $(abnferrcode_h) $(abnfdefs_c)\
		$(abnfresult_c) $(abnfdefine_c) $(abnfparse_c) $(abnfterminals_c)\
		$(abnfdefinehelper_c) $(abnfoptimize_c) $(abnfnormalize_c)\
		$(abnfdefineelements_c) $(abnfoptimizerep_c) $(abnfoptimizecat_c)\
		$(parsenode_c) $(abnfwrite_c)
	gcc $(incswitch) $(CFLAGS) $(abnf_c) -o $(abnf_o)

.PHONY : clean

clean :
	-rm main $(objs)
