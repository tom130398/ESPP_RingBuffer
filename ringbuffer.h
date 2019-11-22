/*This is header file for ringbuffer.c file*/

#define BUFFER_SIZE 1024

/*Struct fir handling ring buffer*/
struct buffer_type
{
	unsigned char *head;
	unsigned char *tail;
	unsigned char *beginning;
	unsigned char *end;
};

typedef enum error_type
{
	OK,
	BUFFER_OVER_FLOW,
	EMPTY_BUFFER,
	POINTER_ERROR,
	BUFFER_FULL
};

/*Initialize the buffer by placing the pointers to begining of buffer*/
void init_buffer(struct buffer_type *b, unsigned char *buffer, int buffer_size);

/*Empty the buffer by removing tail to head*/
void empty_buffer(struct buffer_type *b);

/*Check the status of the buffer and return amount of characters in buffer and error type
  @param *err holds type of error: 	OK
					BUFFER OVER FLOW
					EMPTY BUFFER
					POINTER ERROR
					BUFFER FULL
*/
int get_buffer_state(struct buffer_type *b, error_type *err);

/*Adding one character to buffer. Function returns amount of characters and error type
  @param 	c-character to add in buffer
		*err-error type
*/
int add_char_to_buffer(struct buffer_type *b, unsigned char c, error_type *err);

/*Get one character from buffer. Function returns character read from buffer and error type*/
char get_char_from_buffer(struct buffer_type *b, error_type *err);

/*List characters from buffer. Function will return amount of character in buffer and error type*/
int print_buffer(struct buffer_type b, error_type *err);

/*Add string to buffer and return amount of added characters and error type*/
int add_string_to_buffer(struct buffer_type *b, unsigned char *s, error_type *err);

/*Get string from buffer and return amount of readed characters and error type*/
int  get_string_from_buffer(struct buffer_type *b, unsigned char *dest, int len, error_type *err);

/*List characters from buffer*/
int print_buffer(struct buffer_type *b, error_type *err);