#include "mylib.h"

//The mymodulo() function returns the positive result of the modulo operation (dividend modulo divisor)
unsigned int mymodulo(int dividend, int divisor) {
    if(dividend >= 0)
        return dividend%divisor;
    else
        return (dividend%divisor + divisor)%divisor;
}

//The mywrite() function is similar to the write() function.
//On success, the number of bytes read is returned.
//On error, -1 is returned, and errno is set appropriately.
ssize_t mywrite(int fd, void *buf, size_t count) {
    ssize_t _written_ = 0;
    ssize_t ret = 1;
    while(count > 0 && ret != 0) {
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
    ssize_t ret = 1;
    while(count > 0 && ret != 0) {
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
    // memcpy(newptr, ptr, MIN(size, sizeof(ptr)));// sizeof(ptr)-> always 8 ??
    newptr = realloc(ptr,size);
    free(ptr);
    return newptr;
}

//The mystrcpy() function is similar to the strcpy() function
//The destination pointer is allocated to receive the copy
//It returns a pointer to the destination string dest
char *mystrcpy(char *dest, const char *src) {
    if(dest != NULL)
        free(dest);
    dest = malloc(strlen(src) + 1);
    strcpy(dest, src);
    return dest;
}

void new_stderr(){
    int new_stderr = open("stderr.log",O_CREAT | O_WRONLY,0777);

    lseek(new_stderr,0,SEEK_END);
    dup2(new_stderr,2);
    close(new_stderr);
}

void my_print_err(char* message){
    mywrite(2,message,strlen(message));
}

char* read_line(int fd){
    char* ret=NULL;
    int n;
    int i = 1;

    int pos_act = lseek(fd,0,SEEK_CUR);
    int pos_end = lseek(fd,0,SEEK_END);
    char buf[1];
    if(pos_end == pos_act){
        return ret; // if it's the end of the file
    }
    ret = malloc(i);
    lseek(fd,pos_act,SEEK_SET);

    do{
        n = read(fd,buf,1);
        if(n == -1){
            perror("erreur lecture du fichier");
            exit(-1);
        }
        ret[i-1] = *buf;
        i++;
        ret = realloc(ret,i);

    }while(ret[i-2] != '\n');
    ret[i-2] = '\0';
    
    return ret;
}


int my_rand(int min,int max){
    int f;
    unsigned int random;
    if ((f = open("/dev/urandom",O_RDONLY)) == -1) {
        perror("erreur : generation nombre aléatoir ");
        exit(1);
    }
    myread(f, &random, 4);
    close(f);
    return min+random%(max-min+1);
}
