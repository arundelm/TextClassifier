
CXX ?= g++
CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

test: 
		main.exe

main.exe: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@

.SUFFIXES:

.PHONY: clean
clean :
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out.txt

# Run style check tools
CPD ?= /usr/um/pmd-6.0.1/bin/run.sh cpd
OCLINT ?= /usr/um/oclint-0.13/bin/oclint
FILES := BinarySearchTree.hpp BinarySearchTree_tests.cpp Map.hpp main.cpp
CPD_FILES := BinarySearchTree.hpp Map.hpp main.cpp
style :
	$(OCLINT) \
    -no-analytics \
    -rule=LongLine \
    -rule=HighNcssMethod \
    -rule=DeepNestedBlock \
    -rule=TooManyParameters \
    -rc=LONG_LINE=90 \
    -rc=NCSS_METHOD=40 \
    -rc=NESTED_BLOCK_DEPTH=4 \
    -rc=TOO_MANY_PARAMETERS=4 \
    -max-priority-1 0 \
    -max-priority-2 0 \
    -max-priority-3 0 \
    $(FILES) \
    -- -xc++ --std=c++17
	$(CPD) \
    --minimum-tokens 100 \
    --language cpp \
    --failOnViolation true \
    --files $(CPD_FILES)
	@echo "########################################"
