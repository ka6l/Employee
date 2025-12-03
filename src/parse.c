#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"
#include "parse.h"

int create_db_header(int fd, struct dbHeader_t **headerOut){
    (void)fd; // fd is unused for step1 but tests will pass it

    if (headerOut == NULL)
        return STATUS_ERROR;

    struct dbHeader_t *header = calloc(1, sizeof(struct dbHeader_t));
    if (!header)
        return STATUS_ERROR;

    header->version = 1;            // must match test
    header->count   = 0;            // must match test
    header->magic   = HEADER_MAGIC; // in host-endian
    header->filesize= 0;            // overwritten by output_file

    *headerOut = header;

    return STATUS_SUCCESSFUL;
}

int validate_db_header(int fd, struct dbHeader_t **headerOut){
    if (fd < 0 || headerOut == NULL)
        return STATUS_ERROR;

    struct dbHeader_t *header = calloc(1, sizeof(struct dbHeader_t));
    if (!header)
        return STATUS_ERROR;

    if (read(fd, header, sizeof(struct dbHeader_t)) != sizeof(struct dbHeader_t)) {
        free(header);
        return STATUS_ERROR;
    }

    // Convert only big-endian fields
    header->magic    = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);
    header->version  = ntohs(header->version);
    header->count    = ntohs(header->count);

    if (header->magic != HEADER_MAGIC) {
        free(header);
        return STATUS_ERROR;
    }

    struct stat st;
    fstat(fd, &st);

    if (header->filesize != (uint32_t)st.st_size) {
        free(header);
        return STATUS_ERROR;
    }

    *headerOut = header;
    return STATUS_SUCCESSFUL;
}

int output_file(int fd, struct dbHeader_t *DBHDR){
    if (fd < 0 || DBHDR == NULL)
        return STATUS_ERROR;

    // Determine current file size
    struct stat st;
    fstat(fd, &st);
    DBHDR->filesize = st.st_size;

    // Convert only big-endian fields
    uint32_t magic_be    = htonl(DBHDR->magic);
    uint32_t filesize_be = htonl(DBHDR->filesize);

    uint16_t version_be = htons(DBHDR->version);
    uint16_t count_be   = htons(DBHDR->count);

    // Write header at start
    lseek(fd, 0, SEEK_SET);

    write(fd, &magic_be,    sizeof(uint32_t));
    write(fd, &version_be,  sizeof(uint16_t));
    write(fd, &count_be,    sizeof(uint16_t));
    write(fd, &filesize_be, sizeof(uint32_t));

    return STATUS_SUCCESSFUL;
}