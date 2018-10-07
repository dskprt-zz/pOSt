extern inline unsigned char inportb(unsigned int port);
extern inline void outportb(unsigned int port, unsigned char value);
extern void writestr(const char* data);
extern void write(const char* data, size_t size);
extern size_t strlen(const char* str);
