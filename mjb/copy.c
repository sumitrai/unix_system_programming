/**
 * copy.c: My own implementation of Maurice J. Bach Figure 1.3 from
 * The design of Unix Operating System.
 * Usage: copy srcfile dstfile
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEF_IO_BLOCK_SIZE 2048
char buffer[DEF_IO_BLOCK_SIZE];
int version = 1;

// copy srcfd to dstfd: 0 on success, -1 on failure
int copyfd(int srcfd, int dstfd)
{
    int count = 0;
    char buffer[DEF_IO_BLOCK_SIZE];
    
    while ((count = read(srcfd, buffer, sizeof buffer)) > 0) {
        if (write(dstfd, buffer, count) == -1) {
            fprintf(stderr, "write error on fd: %d\n", dstfd);
            return -1;
        }
    }

    // check of read error
    if (count == -1) {
        fprintf(stderr, "read error on fd: %d\n", srcfd);
        return -1;
    } else {
        return 0;
    }
}

int main(int argc, char *argv[])
{
    int ret, srcfd, dstfd;
    char *srcfile, *dstfile;
    
    if (argc < 3) {
        fprintf(stderr, "%s requires 2 arguments\n", argv[0]);
        fprintf(stdout, "Usage: %s srcfile dstfile\n", argv[0]);
        exit(EXIT_FAILURE);
    } else {
        srcfile = argv[1];
        dstfile = argv[2];
    }

    // open src file in read-only mode
    srcfd = open(argv[1], O_RDONLY);
    if (srcfd == -1) {
        fprintf(stderr, "cannot open file: %s due to: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    // create the dst. file
    dstfd = creat(argv[2], 0666);
    if (dstfd == -1) {
        fprintf(stderr, "cannot create file: %s due to %s\n", argv[2],
                strerror(errno));
        close(srcfd);
        exit(EXIT_FAILURE);
    }

    // copy the src file to dst. file
    if (copyfd(srcfd, dstfd) == -1) {
        fprintf(stderr, "failed to copy file: %s to file: %s\n", srcfile,
                dstfile);
        ret = EXIT_FAILURE;
    } else {
        ret = EXIT_SUCCESS;
    }

    // cleanup and exit
    close(srcfd);
    close(dstfd);
    exit(ret);
}
