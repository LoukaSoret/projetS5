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
example_STMIB example_STMDA example_STMDB test_add_adc test_and_orr_eor \
test_bic test_cmn test_cmp test_mov test_mvn test_rsb_rsc test_sub_sbc example6

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
example5_SOURCES=example5.s
example6_SOURCES=example6.s
example_Kirill_Load_Multiples_SOURCES=example_Kirill_Load_Multiples.s
example_Kirill_Load_Multiples_ldmda_SOURCES=example_Kirill_Load_Multiples_ldmda.s
example_Kirill_Load_Multiples_ldmib_SOURCES=example_Kirill_Load_Multiples_ldmib.s
example_Kirill_Load_Multiples_ldmdb_SOURCES=example_Kirill_Load_Multiples_ldmdb.s
example_STMIA_SOURCES=example_STMIA.s
example_STMIB_SOURCES=example_STMIB.s
example_STMDA_SOURCES=example_STMDA.s
example_STMDB_SOURCES=example_STMDB.s
test_add_adc_SOURCES=test_add_adc.s
test_and_orr_eor_SOURCES=test_and_orr_eor.s
test_bic_SOURCES=test_bic.s
test_cmn_SOURCES=test_cmn.s
test_cmp_SOURCES=test_cmp.s
test_mov_SOURCES=test_mov.s
test_mvn_SOURCES=test_mvn.s
test_rsb_rsc_SOURCES=test_rsb_rsc.s
test_sub_sbc_SOURCES=test_sub_sbc.s



insertion_sort_SOURCES=insertion_sort.c

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
