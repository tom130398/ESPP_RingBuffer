#include <stdio.h>
#include <stdlib.h>
#include "ringbuffer.h"

void init_buffer(struct buffer_type *b, unsigned char *buffer, int buffer_size)
{
	b->head=buffer;
	b->tail=buffer;
	b->beginning=buffer;
	b->end=buffer+buffer_size-1;
}

void empty_buffer(struct buffer_type *b)
{
	b->tail=b->head;
}

int get_buffer_state(struct buffer_type *b, error_type *err)
{
	int amount = 0;
	unsigned char *temp;
	temp = b->tail;
	// check POINTER_ERROR
	if((b->tail < b->beginning)||(b->tail > b->end)||(b->head < b->beginning)||(b->head > b->end))
	{
		*err = POINTER_ERROR;
		amount = -1;
	}
	else if((b->head == b->tail -1) || ((b->head == b->end) && b->tail == b->beginning))
        {
                *err = BUFFER_FULL;
                amount = BUFFER_SIZE-1;
        }
	else if(b->tail < b->end)
	{
		if(temp != b->head)
		{
			while(temp != b->head)
			{
				if(temp == b->end)
					temp = b->beginning;
				else
					temp++;
				amount++;
				*err = OK;
			}
		}
		// check EMPTY_BUFFER
		else if(temp == b->head)
		{
			amount = 0;
			*err = EMPTY_BUFFER;
		}
	}
	return amount;
}

int add_char_to_buffer(struct buffer_type *b,  unsigned char c, error_type *err)
{
	*err = OK;
	int count = get_buffer_state(b,err);
	if(*err == POINTER_ERROR || *err == BUFFER_OVER_FLOW || *err == BUFFER_FULL)
		return -1;
	*(b->head) = c;
	if(b->head != b->end)
		b->head++;
	else
		b->head = b->beginning;
	return count+1;
}

char get_char_from_buffer(struct buffer_type *b, error_type *err)
{
	unsigned char c = 0;
	*err = OK;
	int count = get_buffer_state(b,err);
	if(*err == BUFFER_OVER_FLOW || *err == POINTER_ERROR || *err == EMPTY_BUFFER)
		return -1;
	c = *(b->tail);
	if(b->tail != b->end)
                b->tail++;
        else
                b->tail = b->beginning;
        return c;

}

int print_buffer(struct buffer_type *b, error_type *err)
{
	*err = OK;
	int count = get_buffer_state(b,err);
	if(*err == POINTER_ERROR || *err == BUFFER_OVER_FLOW || *err == EMPTY_BUFFER)
		return -1;
	else
	{
		for(int i=0; i<count; i++)
		{
			printf("%c\n",*(b->tail+i));
		}
	}
	return count;
}