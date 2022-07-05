// use global var for return value, wasn't allowed 

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int my_printf(char* restrict format, ...);
void print_int(int arg);
void print_oct(long arg);
void print_unsigned(int arg);
void print_hex(void* arg);
void print_string(char* arg);
void print_ptr(void* arg);
char* convert(long num, char* digits, int baseLen);
int my_strlen(char* string);
char* my_strcpy(char* dst, char* src);
char* my_strcat(char* dest, char* src);

const int ret;

int main(void)
{
    char* str = "it works";
    char c = '!';
    int i = 123;
    printf("My int: %d, oct: %o, hex: %x, and my string: %s%c\n", i, i, i, str, c);
    my_printf("My int: %d, oct: %o, hex: %x, and my string: %s%c\n", i, i, i, str, c);
    int* ptr;
    ptr = &i;
    printf("My ptr is: %p\n", ptr);
    my_printf("My ptr is: %p\n", ptr);
    int n = 0;
    n = printf("NULL STRING %s!\n", (char*)NULL);
    printf("%d\n", n);
    my_printf("NULL STRING %s!\n", (char*)NULL);    
    return 0;
}

int my_printf(char* restrict format, ...)
{
    va_list va_args;
    va_start(va_args, format);

    for (; *format != '\0'; format++)
    {
        if (*format != '%') putchar(*format);
        if (*format == '%')
        {
            format++;
            if (*format == 'd') {
                int i = va_arg(va_args, int);
                print_int(i);
            } else if (*format == 'o') {
                long o = va_arg(va_args, long);
                print_oct(o);
            } else if (*format == 'u') {
                int u = va_arg(va_args, int);
                print_unsigned(u);
            } else if (*format == 'x') {
                void* x = va_arg(va_args, void*);
                print_hex(x);
            } else if (*format == 'c') {
                int c = va_arg(va_args, int);
                putchar(c);
            } else if (*format == 's') {
                char* str = va_arg(va_args, char*);
                print_string(str);
            } else if (*format == 'p') {
                void* ptr = va_arg(va_args, void*);
                print_ptr(ptr);
            }
        }
    }
    va_end(va_args);
    return 0;
}

void print_int(int arg)
{
    if(arg < 0) {
        putchar('-');
        arg = -arg;
    }
    if(arg == 0)
        putchar('0');
    if(arg / 10) {   // e.g. if 12/1 = 1
        print_int(arg / 10);  // recursion goes on until arg is below 10; here: 1/10 = 0
        putchar(arg % 10 + '0'); // 1%10 = 1; 
    } else {
        putchar(arg + '0');
    }
}

void print_oct(long arg)
{
    char* base = "01234567";
    char* ptr = convert(arg, base, 8);
    print_string(ptr);
    free(ptr);
}

void print_unsigned(int arg)
{
    if(arg == 0) {
        putchar('0');
    }
    if(arg / 10) {
        print_int(arg / 10);
        putchar(arg % 10 + '0');
    } else {
        putchar(arg + '0');
    }
}

void print_hex(void* arg)
{
    char* base = "0123456789abcdef";
    char* ptr = convert((long)arg, base, 16);
    print_string(ptr);
    free(ptr);
}

void print_string(char* arg)
{   
    if (arg == NULL) {
        char* ptr = "(null)";
        print_string(ptr);
    }
    else {
        int i = 0;
        while(arg[i] != '\0') {
            putchar(arg[i]);
            i++;
        }
    }
}

void print_ptr(void* arg)
{
    char* base = "0123456789abcdef";
    char *hex = convert((long long) arg, base, 16);
    char *ptr = calloc((my_strlen(hex) + 2), sizeof(int));
    my_strcpy(ptr, "0x");
    my_strcat(ptr, hex);
    print_string(ptr);
    free(hex);
    free(ptr);
}

char* convert(long num, char* digits, int baseLen) {
    int len = 0;
    long numCopy = num; // for handling it in the next steps (and only there)

    // find length of num in the num system (according to baseLen) for malloc!!
    do {
        len++;
        numCopy /= baseLen;
    } while (numCopy != 0);

    char* buffer = malloc(sizeof(int) * len);

    // add null terminator, then reverse string
    buffer[len] = '\0';
    len--;

    while(num != 0) {
        buffer[len] = digits[num % baseLen];
        num /= baseLen;
        len--;
    }
    return buffer;
}

int my_strlen(char* string)
{
    int counter = 0;
    while(string[counter] != '\0')
        counter++;    
    return counter;
}

char* my_strcpy(char* dst, char* src)
{
    char *ptr = dst;

    while(*src != '\0') {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = *src;
    return ptr;
}

char* my_strcat(char* dest, char* src)
{
    while(*dest) dest++;
    while((*dest++ = *src++));
    return --dest;
}