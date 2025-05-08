#include "minishell.h"
#include <fcntl.h>

 /* 0:<, 1:>, 2:>>, 3:<< */

/* O_RDONLY        open for reading only
   O_WRONLY        open for writing only
   O_RDWR          open for reading and writing
   O_NONBLOCK      do not block on open or for data to become available
   O_APPEND        append on each write
   O_CREAT         create file if it does not exist
   O_TRUNC         truncate size to 0
   O_EXCL          error if O_CREAT and the file exists
   O_SHLOCK        atomically obtain a shared lock
   O_EXLOCK        atomically obtain an exclusive lock
   O_NOFOLLOW      do not follow symlinks
   O_SYMLINK       allow open of symlinks
   O_EVTONLY       descriptor requested for event notifications only
   O_CLOEXEC       mark as close-on-exec */

int open_file(int type, char *file)
{
    int fd = -1;
    if (type == 0)
    {
        fd = open(file, O_RDONLY);
        if (fd == -1)
            {
                write(2, "minishell $>", 13);
                write(2, file, ft_strlen(file));
                write(2, ": No such file or directory\n", 29);
            }
    }
    if (type == 1)
    {
        fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (fd == -1)
        {
            write(2, "minishell $> ", 13);
            write(2, file, ft_strlen(file));
            write(2, ": Cannot create or write to file\n", 33);
        }
        
    }
    if (type == 2)
    {
        fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
        if (fd == -1)
        {
            write(2, "minishell $> ", 13);
            write(2, file, ft_strlen(file));
            write(2, ": Cannot append to file\n", 24);
        }
    }
    return fd;
}