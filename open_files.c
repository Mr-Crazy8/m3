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
    int fd;

    fd = -1;
    if (type == 0)
    {
        fd = open(file, O_RDONLY);
        if (fd == -1)
            print_file_error(file, 0);
    }
    if (type == 1)
    {
        fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (fd == -1)
            print_file_error(file, 1);
        
    }
    if (type == 2)
    {
        fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
        if (fd == -1)
            print_file_error(file, 2);
    }
    return fd;
}



void print_file_error(char *file, int i)
{
    write(2, "minishell $> ", 13);
    write(2, file, ft_strlen(file));
    if (i == 0)
        write(2, " : No such file or directory\n", 29);
    else if (i == 1)
        write(2, ": Cannot create or write to file\n", 33);
    else if (i == 2)
        write(2, ": Cannot append to file\n", 24);
}









void file_opener(t_cmd *cmd)
{
    t_cmd *tmp;
    t_redir *tp = NULL;
    int fd;
    tmp = cmd;
    while (tmp)
    {
        tp = tmp->redirs;
        while (tp)
        {   
            fd = open_file(tp->type, tp->file);
            if (fd == -1)
            {
                tp->fd = fd;
                break;
            }
            else
            {
                tp->fd = fd;
            }
            tp = tp->next;
        }
        tmp = tmp->next;
    }
}

