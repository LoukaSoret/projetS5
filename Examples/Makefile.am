CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
AM_CFLAGS=-mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 example5 insertion_sort \
example_Kirill_Load_Multiples example_Kirill_Load_Multiples_ldmda \
example_Kirill_Load_Multiples_ldmib example_Kirill_Load_Multiples_ldmdb example_STMIA \
example_STMIB example_STMDA example_STMDB 

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
example5_SOURCES=example5.s
example_Kirill_Load_Multiples_SOURCES=example_Kirill_Load_Multiples.s
example_Kirill_Load_Multiples_ldmda_SOURCES=example_Kirill_Load_Multiples_ldmda.s
example_Kirill_Load_Multiples_ldmib_SOURCES=example_Kirill_Load_Multiples_ldmib.s
example_Kirill_Load_Multiples_ldmdb_SOURCES=example_Kirill_Load_Multiples_ldmdb.s
example_STMIA_SOURCES=example_STMIA.s
example_STMIB_SOURCES=example_STMIB.s
example_STMDA_SOURCES=example_STMDA.s
example_STMDB_SOURCES=example_STMDB.s
insertion_sort_SOURCES=insertion_sort.c

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
