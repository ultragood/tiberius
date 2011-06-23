FILES=my_test.cpp Cassandra/%
BUILDDIR=.
SOURCE_FILES=$(filter-out $(FILES),$(wildcard */*.cpp) )
OBJECT_FILES=$(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCE_FILES))
LIBS=-ldb_cxx -lmorfo -lfries -lomlet -lpcre $(call FLEXLIB,boost_filesystem) 
CXXFLAGS=-Wall -Werror -fPIC -ggdb
SOURCE_DEFINES=-DHAVE_CXX_STDHEADERS
DEFINES=-D_LINUX -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE $(D)
INCLUDES=-I. 


FLEXLIB=-l$(subst lib,,$(basename $(notdir $(firstword $(wildcard /usr/lib/lib$1*.so /usr/lib64/lib$1*.so /usr/local/lib/lib$1*.so)))))


$(BUILDDIR)/%.o: %.cpp %.h
	##@$(MKDIR) $(dir $@)
	echo "buildir $@ "
	$(CXX) $(DEFINES) $(CXXFLAGS) $(INCLUDES) $(@:.o=.cpp) -c -o $(BUILDDIR)/$@

libnlp.so: CORE_FLAGS=$(CXXFLAGS) -fpermissive -shared
libnlp.so: $(OBJECT_FILES)
	echo $(SOURCE_FILES)
	$(CXX) $(DEFINES) $(CORE_FLAGS) $(LINK_PATH) $(INCLUDES) $(LIBS) $(OBJECT_FILES) -o $@

my_sample: libnlp.so
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(LIBS) -L. -lnlp my_sample.cpp -o my_sample

### TEST STUFF
GTEST_DIR=/opt/gtest-1.6.0
TEST_INCLUDES=$(INCLUDES) -I$(GTEST_DIR)/include
USER_DIR=.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h 
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GTEST_DIR)/include $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GTEST_DIR)/include $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# All Google Test headers.  Usually you shouldn't change this
# definition.

TEST_DIR=test
TESTS = sample1_unittest

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	echo "compiling .0"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(GTEST_DIR)/include -c $(@:.o=.cpp) -o $@


clean-tests:
	rm $(wildcard $(TEST_DIR)/*.o)	

tests: clean-tests compile-tests

compile-tests: libnlp.so gtest_main.a $(patsubst %.cpp, %.o, $(wildcard $(TEST_DIR)/*.cpp))
#	$(patsubst %.cpp, %.o, $(wildcard test/*.cpp))
	@cd ../canned_data && tar xvfz cnn.tar.gz > /dev/null
	@sudo cp ../conf/freeling/tiberius-np.dat /usr/local/share/FreeLing/en/
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_INCLUDES) -lpthread gtest_main.a $(LIBS) -L. -lnlp $(patsubst %.cpp, %.o, $(wildcard $(TEST_DIR)/*.cpp)) -o test/testsuite
