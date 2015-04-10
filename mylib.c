#include "mylib.h"

//The mywrite() function is similar to the write() function.
//On success, the number of bytes read is returned.
//On error, -1 is returned, and errno is set appropriately.
ssize_t mywrite(int fd, void *buf, size_t count) {
	ssize_t _written_ = 0;
    ssize_t ret;
    while(count > 0) {
        if((ret = write(fd, buf+_written_, count)) == -1)
            return -1;
        _written_ += ret;
        count -= ret;
    }
    return _written_;
}

//The myread() function is similar to the read() function.
//On success, the number of bytes written is returned.
//On error, -1 is returned, and errno is set appropriately.
ssize_t myread(int fd, void *buf, size_t count) {
	ssize_t _read_ = 0;
    ssize_t ret;
    while(count > 0) {
        if((ret = read(fd, buf+_read_, count)) == -1)
            return -1;
        _read_ += ret;
        count -= ret;
    }
    return _read_;
}

//The myrealloc() function is similar to the realloc() function and fixes possible memory leaks.
//On success, a pointer to the newly allocated memory is returned.
//On error, NULL is returned. NULL can be returned on a successful call.
void *myrealloc(void *ptr, size_t size) {
	if(ptr == NULL)
		return malloc(size);
	if(size == 0) {
		free(ptr);
		return NULL;
	}
	void *newptr = malloc(size);
    memcpy(newptr, ptr, MIN(size, sizeof(ptr)));
    free(ptr);
    return newptr;
}

//The mymodulo() function returns the positive result of the modulo operation (dividend modulo divisor)
unsigned int mymodulo(int dividend, unsigned int divisor) {
    if(dividend >= 0)
        return dividend%divisor;
    else
        return (dividend%divisor + divisor)%divisor;
}