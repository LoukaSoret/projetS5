CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
AM_CFLAGS=-mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 insertion_sort example_Kirill_Load_Multiples example_Kirill_Load_Multiples_ldmda example_Kirill_Load_Multiples_ldmib

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
example_Kirill_Load_Multiples_SOURCES=example_Kirill_Load_Multiples.s
example_Kirill_Load_Multiples_ldmda_SOURCES=example_Kirill_Load_Multiples_ldmda.s
example_Kirill_Load_Multiples_ldmib_SOURCES=example_Kirill_Load_Multiples_ldmib.s
insertion_sort_SOURCES=insertion_sort.c

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
