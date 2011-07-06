BUILD_DIR=../build
BIN_DIR=../bin
LIB_DIR=../lib
TEST_DIR=../test

### TEST STUFF
GTEST_DIR=/opt/gtest-1.6.0
TEST_INCLUDES=-I$(GTEST_DIR)/include
USER_DIR=.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h 
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

FILES=my_test.cpp Cassandra/% test/%
SRC_DIR=.
SOURCE_FILES=$(filter-out $(FILES),$(wildcard */*.cpp) )
OBJECT_FILES=$(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o,$(SOURCE_FILES)))
CASSANDRA_SOURCE_FILES=$(wildcard Cassandra/*.cpp Cassandra/*/*.cpp Cassandra/*/*/*.cpp )
CASSANDRA_OBJECT_FILES=$(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o,$(CASSANDRA_SOURCE_FILES)))

LIBS=-ldb_cxx -lmorfo -lfries -lomlet -lpcre $(call FLEXLIB,boost_filesystem)  $(call FLEXLIB,boost_thread) $(call FLEXLIB,boost_serialization) -lthrift -L $(LIB_DIR) 
CXXFLAGS=-Wall -Werror -fPIC -ggdb
SOURCE_DEFINES=-DHAVE_CXX_STDHEADERS
DEFINES=-D_LINUX -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE $(D)
INCLUDES=-I. -I/usr/local/include/thrift -ICassandra/gen-cpp -ICassandra $(TEST_INCLUDES)



FLEXLIB=-l$(subst lib,,$(basename $(notdir $(firstword $(wildcard /usr/lib/lib$1*.so /usr/lib64/lib$1*.so /usr/local/lib/lib$1*.so)))))

$(BIN_DIR):

	@mkdir -p $@

$(LIB_DIR):

	@mkdir -p $@

%.cpp: %.h
	@touch $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(DEFINES) $(CXXFLAGS) $(INCLUDES) $< -c -o $@

libnlp.so : $(LIB_DIR)/libnlp.so

$(LIB_DIR)/libnlp.so: CORE_FLAGS=$(CXXFLAGS) -fpermissive -shared
$(LIB_DIR)/libnlp.so: $(OBJECT_FILES) $(LIB_DIR)
	$(CXX) $(DEFINES) $(CORE_FLAGS) $(LINK_PATH) $(INCLUDES) $(LIBS) $(OBJECT_FILES) -o $@

my_sample: 
	$(MAKE) $(BIN_DIR)/my_sample


$(BIN_DIR)/my_sample: libnlp.so $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(LIBS) -L. -lnlp my_sample.cpp -o $@

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(BUILD_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GTEST_DIR)/include $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc -o $@

$(BUILD_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GTEST_DIR)/include $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc -o $@

$(BUILD_DIR)/gtest.a : $(BUILD_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^ 

$(BUILD_DIR)/gtest_main.a : $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^ 

tests: compile-tests


TEST_SOURCE_FILES=$(wildcard test/*.cpp)
TEST_OBJECT_FILES=$(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(TEST_SOURCE_FILES)))

compile-tests: libnlp.so $(BUILD_DIR)/gtest_main.a $(TEST_OBJECT_FILES)

	@cd ../canned_data && tar xvfz cnn.tar.gz > /dev/null
	@sudo cp ../conf/freeling/tiberius-np.dat /usr/local/share/FreeLing/en/
	@mkdir -p ../test
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TEST_INCLUDES) -lpthread $(BUILD_DIR)/gtest_main.a $(LIBS) -L. -lnlp $(TEST_OBJECT_FILES) -o ../test/testsuite


#### Cassandra stuff, independent for right now

Cassandra/%.o: Cassandra/%.cpp

	$(CXX) $(DEFINES) $(CXXFLAGS) $(INCLUDES) $(@:.o=.cpp) -c -o $@

cassandra: $(BIN_DIR) $(BIN_DIR)/cassandra

$(BIN_DIR)/cassandra: $(CASSANDRA_OBJECT_FILES) 
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(LIBS) $^ -o $@



clean:

	rm -rf $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR) $(TEST_DIR)


mytest: mytest.cpp libnlp.so
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CASSANDRA_OBJECT_FILES) $(INCLUDES) -lpthread $(LIBS) -L. -lnlp mytest.cpp -o mytest