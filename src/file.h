/* file.h */

#ifndef FILE_H
#define FILE_H 1


void
LoadHexFile( const char *filename, void (* cpu_write_pgm)( unsigned int Address, unsigned char Value ) );


#endif /* FILE_H */
