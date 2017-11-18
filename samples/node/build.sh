g++ -D MODULEDIR='""' -D BUILTIN_MODULES="" -I ../.. -I ../../libproxy -Wall -Wextra node.cpp ../../libproxy/*.cpp ../../libmodman/*.cpp -ldl -o node
