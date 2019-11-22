#include <gtest/gtest.h>
#include <string.h>
#include <stdio.h>
#include "ringbuffer.h"

#ifndef MINH_HEADER
#define MINH_HEADER
void copy_buf(struct buffer_type *src, struct buffer_type *des)
{
	des->head 		= src->head;
    des->tail 		= src->tail;
    des->beginning 	= src->beginning;
    des->end 		= src->end;
}
#endif

void check_buffer_content_unchanged(unsigned char *str, unsigned char *temp_str, struct buffer_type temp_buf)
{
	int temp_buf_tail_found = 0;
	for(int i=0;; i++)
	{
		if(i == BUFFER_SIZE)
			i = 0;
		if(str+i == temp_buf.tail)
			temp_buf_tail_found = 1;
		
		if(!temp_buf_tail_found)
			continue;
		if(str+i == temp_buf.head)
			break;
		
		EXPECT_EQ(temp_str[i], str[i]);
	}
}

//----------------------Init buffer--------------------------------------------

TEST(INIT_BUFFER, init_1_normal)
{
    unsigned char str[BUFFER_SIZE];
    struct buffer_type buf;

    init_buffer(&buf, str, BUFFER_SIZE);

    EXPECT_EQ(str, buf.head);
    EXPECT_EQ(str, buf.tail);
    EXPECT_EQ(str, buf.beginning);
	EXPECT_EQ(str + BUFFER_SIZE - 1, buf.end);
}

TEST(INIT_BUFFER, init_2_used_buffer)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;

    init_buffer(&buf, str, BUFFER_SIZE);
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	buf.head += 5;
	buf.tail += 10;
	init_buffer(&buf, str, BUFFER_SIZE);

    EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
	
	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(INIT_BUFFER, init_3_change_str)
{
    unsigned char str_1[BUFFER_SIZE];
	unsigned char str_2[BUFFER_SIZE];
    struct buffer_type buf;

    init_buffer(&buf, str_1, BUFFER_SIZE);
	init_buffer(&buf, str_2, BUFFER_SIZE);

    EXPECT_EQ(str_2, buf.head);
    EXPECT_EQ(str_2, buf.tail);
    EXPECT_EQ(str_2, buf.beginning);
	EXPECT_EQ(str_2 + BUFFER_SIZE - 1, buf.end);
}

TEST(INIT_BUFFER, init_4_null_buffer)
{
    unsigned char str[BUFFER_SIZE];
    struct buffer_type buf;

	buf.head = buf.tail = buf.beginning = buf.end = NULL;
    init_buffer(&buf, str, BUFFER_SIZE);

    EXPECT_EQ(str, buf.head);
    EXPECT_EQ(str, buf.tail);
    EXPECT_EQ(str, buf.beginning);
	EXPECT_EQ(str + BUFFER_SIZE - 1, buf.end);
}

TEST(INIT_BUFFER, init_5_null_string)
{
    unsigned char str[BUFFER_SIZE];
	memset(str, 0, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;

    init_buffer(&buf, str, BUFFER_SIZE);

    EXPECT_EQ(str, buf.head);
    EXPECT_EQ(str, buf.tail);
    EXPECT_EQ(str, buf.beginning);
	EXPECT_EQ(str + BUFFER_SIZE - 1, buf.end);
}

//----------------------Get buffer state---------------------------------------

TEST(GET_BUFFER_STATE, state_1_after_init)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = OK;

    init_buffer(&buf, str, BUFFER_SIZE);
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = get_buffer_state(&buf, &err);

    EXPECT_EQ(0, return_value);
    EXPECT_EQ(EMPTY_BUFFER, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_BUFFER_STATE, state_2_OK_first)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head++;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = get_buffer_state(&buf, &err);

    EXPECT_EQ(1, return_value);
    EXPECT_EQ(OK, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_BUFFER_STATE, state_3_OK_mid)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += BUFFER_SIZE/2;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = get_buffer_state(&buf, &err);

    EXPECT_EQ(BUFFER_SIZE/2, return_value);
    EXPECT_EQ(OK, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_BUFFER_STATE, state_4_OK_mid)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.tail += BUFFER_SIZE/2;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = get_buffer_state(&buf, &err);

    EXPECT_EQ(BUFFER_SIZE/2, return_value);
    EXPECT_EQ(OK, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_BUFFER_STATE, state_5_OK_last)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += BUFFER_SIZE - 2;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = get_buffer_state(&buf, &err);

    EXPECT_EQ(BUFFER_SIZE - 2, return_value);
    EXPECT_EQ(OK, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_BUFFER_STATE, state_6_empty_mid)
{
    unsigned char str[BUFFER_SIZE];
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += BUFFER_SIZE/2;
	buf.tail = buf.head;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = get_buffer_state(&buf, &err);

    EXPECT_EQ(0, return_value);
    EXPECT_EQ(EMPTY_BUFFER, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
}

TEST(GET_BUFFER_STATE, state_7_full_normal)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += BUFFER_SIZE/2;
	buf.tail = buf.head+1;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = get_buffer_state(&buf, &err);

    EXPECT_EQ(BUFFER_SIZE-1, return_value);
    EXPECT_EQ(BUFFER_FULL, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_BUFFER_STATE, state_8_full_edge)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head = buf.end;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = get_buffer_state(&buf, &err);

    EXPECT_EQ(BUFFER_SIZE-1, return_value);
    EXPECT_EQ(BUFFER_FULL, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

//----------------------Empty buffer-------------------------------------------

TEST(EMPTY_BUFFER, empty_1_after_init)
{
    unsigned char str[BUFFER_SIZE];
    struct buffer_type buf;

    init_buffer(&buf, str, BUFFER_SIZE);
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	empty_buffer(&buf);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.head, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
}

TEST(EMPTY_BUFFER, empty_2_normal_head_after)
{
    unsigned char str[BUFFER_SIZE];
    struct buffer_type buf;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 10;
	buf.tail += 5;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	empty_buffer(&buf);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.head, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
}

TEST(EMPTY_BUFFER, empty_3_normal_tail_after)
{
    unsigned char str[BUFFER_SIZE];
    struct buffer_type buf;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 5;
	buf.tail += 10;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	empty_buffer(&buf);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.head, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
}

TEST(EMPTY_BUFFER, empty_4_full)
{
    unsigned char str[BUFFER_SIZE];
    struct buffer_type buf;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += BUFFER_SIZE/2;
	buf.tail = buf.head+1;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	empty_buffer(&buf);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.head, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
}

TEST(EMPTY_BUFFER, empty_5_null_string)
{
    unsigned char str[BUFFER_SIZE];
	memset(str, 0, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 5;
	buf.tail += 10;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	empty_buffer(&buf);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.head, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
}

//----------------------Add char-----------------------------------------------

TEST(ADD_CHAR, add_1_empty)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = add_char_to_buffer(&buf, 'A', &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ(1, return_value);
	EXPECT_EQ('A', *(temp_buf.head));
	
	EXPECT_EQ(temp_buf.head + 1, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
	
	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(ADD_CHAR, add_2_normal_head_after)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 10;
	buf.tail += 5;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = add_char_to_buffer(&buf, 'A', &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ(6, return_value);
	EXPECT_EQ('A', *(temp_buf.head));
	
	EXPECT_EQ(temp_buf.head + 1, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(ADD_CHAR, add_3_normal_tail_after)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 5;
	buf.tail += 10;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = add_char_to_buffer(&buf, 'A', &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ(1020, return_value);
	EXPECT_EQ('A', *(temp_buf.head));
	
	EXPECT_EQ(temp_buf.head + 1, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(ADD_CHAR, add_4_null_char)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 10;
	buf.tail += 5;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = add_char_to_buffer(&buf, '\0', &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ(6, return_value);
	EXPECT_EQ('\0', *(temp_buf.head));
	
	EXPECT_EQ(temp_buf.head + 1, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(ADD_CHAR, add_5_full)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = OK;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += BUFFER_SIZE/2;
	buf.tail = buf.head+1;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = add_char_to_buffer(&buf, 'A', &err);
	
	EXPECT_EQ(BUFFER_FULL, err);
	EXPECT_EQ(-1, return_value);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(ADD_CHAR, add_6_near_full)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = OK;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += BUFFER_SIZE/2;
	buf.tail = buf.head+2;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = add_char_to_buffer(&buf, 'A', &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ(1023, return_value);
	EXPECT_EQ('A', *(temp_buf.head));
	
	EXPECT_EQ(temp_buf.head + 1, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(ADD_CHAR, add_7_edge)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = OK;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head = buf.end;
	buf.tail = buf.beginning + 1;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	int return_value = add_char_to_buffer(&buf, 'A', &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ(1023, return_value);
	EXPECT_EQ('A', *(temp_buf.head));
	
	EXPECT_EQ(temp_buf.beginning, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

//----------------------Get char-----------------------------------------------

TEST(GET_CHAR, get_1_tail_after)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	str[5] = 'A';
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 10;
	buf.tail += 5;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	char return_char = get_char_from_buffer(&buf, &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ('A', return_char);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail + 1, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_CHAR, get_2_head_after)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	str[10] = 'A';
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 5;
	buf.tail += 10;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	char return_char = get_char_from_buffer(&buf, &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ('A', return_char);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail + 1, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_CHAR, get_3_empty)
{
    unsigned char str[BUFFER_SIZE];
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	char return_char = get_char_from_buffer(&buf, &err);
	
	EXPECT_EQ(-1, return_char);
	EXPECT_EQ(EMPTY_BUFFER, err);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);
}

TEST(GET_CHAR, get_4_edge)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	str[BUFFER_SIZE-1] = 'A';
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 10;
	buf.tail = buf.end;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	char return_char = get_char_from_buffer(&buf, &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ('A', return_char);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.beginning, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}

TEST(GET_CHAR, get_5_near_empty)
{
    unsigned char str[BUFFER_SIZE], temp_str[BUFFER_SIZE];
	str[9] = 'A';
	memcpy(temp_str, str, BUFFER_SIZE * sizeof(unsigned char));
    struct buffer_type buf;
	error_type err = BUFFER_FULL;

    init_buffer(&buf, str, BUFFER_SIZE);
	buf.head += 10;
	buf.tail += 9;
	struct buffer_type temp_buf;
	copy_buf(&buf, &temp_buf);
	char return_char = get_char_from_buffer(&buf, &err);
	
	// cannot check err here, specification doesn't say if err contain correct value when return_value != -1
	EXPECT_EQ('A', return_char);
	
	EXPECT_EQ(temp_buf.head, buf.head);
	EXPECT_EQ(temp_buf.tail + 1, buf.tail);
	EXPECT_EQ(temp_buf.beginning, buf.beginning);
	EXPECT_EQ(temp_buf.end, buf.end);

	check_buffer_content_unchanged(str, temp_str, temp_buf);
}
