GTEST_DIR=~/Embedded_Systems_Project/googletest/googletest

ring : test_case.o ringbuffer.o Gtest_main.o libgtest.a
	g++ -isystem ${GTEST_DIR}/include test_case.c ringbuffer.c Gtest_main.c libgtest.a -pthread -o my_test
ring2 : test_case_1.o ringbuffer.o Gtest_main.o libgtest.a
	g++ -isystem ${GTEST_DIR}/include test_case_1.c ringbuffer.c Gtest_main.c libgtest.a -pthread -o my_test
test_case.o : test_case.c
	g++ -c -isystem ${GTEST_DIR}/include -I${GTEST_DIR} test_case.c
test_case_1.o : test_case_1.c
	g++ -c -isystem ${GTEST_DIR}/include -I${GTEST_DIR} test_case_1.c
ringbuffer.o : ringbuffer.c
	g++ -c ringbuffer.c
Gtest_main.o : Gtest_main.c
	g++ -c -isystem ${GTEST_DIR}/include -I${GTEST_DIR} Gtest_main.c
libgtest.a :
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
clean:
	rm -f *.o

