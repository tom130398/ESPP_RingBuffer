#include <gtest/gtest.h>
#include "ringbuffer.h"
#include <string.h>

TEST(Test_init,MyTest1)
{
	unsigned char rx_buffer[BUFFER_SIZE];
    	struct buffer_type r;

    	init_buffer(&r,rx_buffer,BUFFER_SIZE);

    	EXPECT_EQ(r.head,rx_buffer);
    	EXPECT_EQ(r.tail,rx_buffer);
    	EXPECT_EQ(r.beginning,rx_buffer);
	EXPECT_EQ(r.end,rx_buffer+BUFFER_SIZE-1);
}

TEST(Test_empty,MyTest2)
{
	unsigned char rx_buffer[BUFFER_SIZE];
	struct buffer_type r;

	init_buffer(&r,rx_buffer,BUFFER_SIZE);
	empty_buffer(&r);

        EXPECT_EQ(r.head,rx_buffer);
        EXPECT_EQ(r.tail,rx_buffer);
        EXPECT_EQ(r.beginning,rx_buffer);
        EXPECT_EQ(r.end,rx_buffer+BUFFER_SIZE-1);
}

TEST(Test_getstate,MyTest1_OK)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
	error_type err;
	r.head = rx_buffer+2;
	r.tail = rx_buffer;
	r.beginning = rx_buffer;
	r.end = rx_buffer + BUFFER_SIZE -1;
	int len = get_buffer_state(&r,&err);

	EXPECT_EQ(2,len);
	EXPECT_EQ(err,OK);
}

TEST(Test_getstate,MyTest2_full)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;

	r.head = rx_buffer + BUFFER_SIZE - 2;
	r.tail = rx_buffer + BUFFER_SIZE - 1;
	r.beginning = rx_buffer;
	r.end = rx_buffer+BUFFER_SIZE-1;
        int len = get_buffer_state(&r,&err);
        EXPECT_EQ(BUFFER_SIZE-1,len);
        EXPECT_EQ(err, BUFFER_FULL);
}

TEST(Test_getstate,MyTest3_error)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;

        r.head = rx_buffer + BUFFER_SIZE + 1;
        r.tail = rx_buffer;
	r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
        int len = get_buffer_state(&r,&err);

        EXPECT_EQ(-1,len);
        EXPECT_EQ(err, POINTER_ERROR);
}

TEST(Test_getstate,MyTest4_empty)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;

        r.head = rx_buffer;
        r.tail = rx_buffer;
        r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
        int len = get_buffer_state(&r,&err);

        EXPECT_EQ(0,len);
        EXPECT_EQ(err,EMPTY_BUFFER);
}

TEST(Test_addchar,MyTest1_OK)
{
	unsigned char c = 'c';
	unsigned char rx_buffer[BUFFER_SIZE];
	struct buffer_type r;
	error_type err;

	r.head = rx_buffer + 255;
	r.tail = rx_buffer;
	r.beginning = rx_buffer;
	r.end = rx_buffer+BUFFER_SIZE-1;
	int k = 0;
	for(int i = 0; i<BUFFER_SIZE; i++)
	{
		rx_buffer[i] = 'a' + k;
		k++;
		if(k > 25)
			k = 0;
	}
	int len = add_char_to_buffer(&r,c,&err);
	EXPECT_EQ(256,len);
	EXPECT_EQ('a',rx_buffer[0]);
	EXPECT_EQ(OK,err);
}

TEST(Test_addchar,MyTest2_full)
{
        unsigned char c = 'c';
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;

        r.head = rx_buffer + BUFFER_SIZE - 2;
        r.tail = rx_buffer + BUFFER_SIZE - 1;
        r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
        for(int i = 0; i<BUFFER_SIZE; i++)
        {
                rx_buffer[i] = 0;
        }
        int len = add_char_to_buffer(&r,c,&err);
        EXPECT_EQ(-1,len);
        EXPECT_EQ('\0',rx_buffer[0]);
	EXPECT_EQ(BUFFER_FULL,err);
}

TEST(Test_addchar,MyTest4_error)
{
        unsigned char c = 'c';
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;

        r.head = rx_buffer + BUFFER_SIZE + 1;
        r.tail = rx_buffer;
        r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
	int k = 0;
        for(int i = 0; i<BUFFER_SIZE; i++)
        {
                rx_buffer[i] = 'a' + k;
                k++;
                if(k > 25)
                        k = 0;
        }
        int len = add_char_to_buffer(&r,c,&err);
        EXPECT_EQ(-1,len);
        EXPECT_EQ('a',rx_buffer[0]);
        EXPECT_EQ(POINTER_ERROR,err);
}

TEST(Test_getchar, Test1_OK)
{
	unsigned char rx_buffer[BUFFER_SIZE];
	struct buffer_type r;
	error_type err;
	unsigned char readchar;
	r.head = rx_buffer+10;
	r.tail = rx_buffer;
	r.beginning = rx_buffer;
	r.end = rx_buffer+BUFFER_SIZE-1;
    	int k = 0;
    	for(int i = 0; i < BUFFER_SIZE; i++)
    	{
        	rx_buffer[i] = 'a'+ k;
        	k++;
        	if (k > 25)
			k = 0;
    	}
	readchar = get_char_from_buffer(&r, &err);

	EXPECT_EQ('a',readchar);
	EXPECT_EQ(OK,err);
}

TEST(Test_getchar, Test2_empty)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;
        unsigned char readchar;
        r.head = rx_buffer;
        r.tail = rx_buffer;
        r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
        int k = 0;
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
                rx_buffer[i] = 'a'+ k;
                k++;
                if (k > 25)
                        k = 0;
        }
        readchar = get_char_from_buffer(&r, &err);

        EXPECT_EQ((unsigned char)-1,readchar);
        EXPECT_EQ(EMPTY_BUFFER,err);
}

TEST(Test_getchar, Test3_error)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;
        unsigned char readchar;
        r.head = rx_buffer + BUFFER_SIZE + 1;
        r.tail = rx_buffer;
        r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
                rx_buffer[i] = 0;
        }
        readchar = get_char_from_buffer(&r, &err);

        EXPECT_EQ((unsigned char)-1,readchar);
        EXPECT_EQ(POINTER_ERROR,err);
}

TEST(Test_getchar, Test4_special)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;
        unsigned char readchar;
        r.head = rx_buffer;
        r.tail = rx_buffer+255;
        r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
        int k = 0;
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
                rx_buffer[i] = 'a'+ k;
                k++;
                if (k > 25)
                        k = 0;
        }
        readchar = get_char_from_buffer(&r, &err);

        EXPECT_EQ('v',readchar);
        EXPECT_EQ(OK,err);
}

TEST(Test_print, Test1_OK)
{
	unsigned char rx_buffer[BUFFER_SIZE];
	struct buffer_type r;
	error_type err;
	unsigned char readchar;
	r.head = rx_buffer+10;
	r.tail = rx_buffer;
	r.beginning = rx_buffer;
	r.end = rx_buffer+BUFFER_SIZE-1;
    	int k = 0;
    	for(int i = 0; i < BUFFER_SIZE; i++)
    	{
        	rx_buffer[i] = 'a'+ k;
        	k++;
        	if (k > 25)
			k = 0;
    	}
	readchar = print_buffer(&r, &err);

	EXPECT_EQ(10,readchar);
	EXPECT_EQ(OK,err);
}

TEST(Test_print, Test2_empty)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;
        unsigned char readchar;
        r.head = rx_buffer;
        r.tail = rx_buffer;
        r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
        int k = 0;
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
                rx_buffer[i] = 'a'+ k;
                k++;
                if (k > 25)
                        k = 0;
        }
        readchar = print_buffer(&r, &err);

        EXPECT_EQ((unsigned char)-1,readchar);
        EXPECT_EQ(EMPTY_BUFFER,err);
}

TEST(Test_print, Test3_error)
{
        unsigned char rx_buffer[BUFFER_SIZE];
        struct buffer_type r;
        error_type err;
        unsigned char readchar;
        r.head = rx_buffer + BUFFER_SIZE + 1;
        r.tail = rx_buffer;
        r.beginning = rx_buffer;
        r.end = rx_buffer+BUFFER_SIZE-1;
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
                rx_buffer[i] = 0;
        }
        readchar = get_char_from_buffer(&r, &err);

        EXPECT_EQ((unsigned char)-1,readchar);
        EXPECT_EQ(POINTER_ERROR,err);
}
