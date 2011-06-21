MAIN_FILE=my_test.cpp
BUILDDIR=.
SOURCE_FILES=$(filter-out $(MAIN_FILE),$(wildcard */*.cpp) )
OBJECT_FILES=$(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCE_FILES))
LIBS=-ldb_cxx -lmorfo -lfries -lomlet -lpcre $(call FLEXLIB,boost_filesystem) 
CXXFLAGS=-Wall -Werror -fPIC -ggdb
SOURCE_DEFINES=-DHAVE_CXX_STDHEADERS
DEFINES=-D_LINUX -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE $(D)
INCLUDES=-I.

FLEXLIB=-l$(subst lib,,$(basename $(notdir $(firstword $(wildcard /usr/lib/lib$1*.so /usr/lib64/lib$1*.so /usr/local/lib/lib$1*.so)))))


$(BUILDDIR)/%.o: %.cpp %.h
	##@$(MKDIR) $(dir $@)
	echo $@
	$(CXX) $(DEFINES) $(CXXFLAGS) $(INCLUDES) $(@:.o=.cpp) -c -o $(BUILDDIR)/$@

libnlp.so: CORE_FLAGS=$(CXXFLAGS) -fpermissive -shared
libnlp.so: $(OBJECT_FILES)
	echo $(SOURCE_FILES)
	$(CXX) $(DEFINES) $(CORE_FLAGS) $(LINK_PATH) $(INCLUDES) $(LIBS) $(OBJECT_FILES) -o $@

my_sample: libnlp.so
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(LIBS) -L. -lnlp my_sample.cpp -o my_sample