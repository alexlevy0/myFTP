#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <limits.h>
#include <stdlib.h>
#include "serveur.h"

static int		check_path_file(char *file, t_serveur *serv)
{
  char			*real_path;
  int			base_pwd_len;

  base_pwd_len = strlen(serv->base_pwd);
  real_path = realpath(file, NULL);
  if (strncmp(serv->base_pwd, real_path, base_pwd_len))
    {
      return (-1);
    }
  return (0);
}

static int		check_valid_file(t_serveur *serv, t_cmd *cmd, t_data *data)
{
  int			fd;
  struct stat		st;

  memset(data, 0, sizeof(*data));
  if (cmd->arg2[0] == 0)
    {
      printf("cmd->arg2[0] = %d\n", (int)cmd->arg2[0]);
      snprintf(data->data, DATA_SIZE, "Usage : get [file]");
      return (-1);
    }
  if ((fd = open(cmd->arg2, O_RDONLY)) < 0)
    {
      snprintf(data->data, DATA_SIZE, "Invalid file");
      return (-1);
    }
  if (fstat(fd, &st) < 0 || (check_path_file(cmd->arg2, serv) == -1)
      || !S_ISREG(st.st_mode))
    {
      snprintf(data->data, DATA_SIZE, "Invalid file");
      return (-1);
    }
  data->size = st.st_size;
  snprintf(data->data, DATA_SIZE, "%s", cmd->arg2);
  return (fd);
}

char			*get_serveur(t_serveur *serv, t_cmd *cmd)
{
  int			fd;
  t_data		data;
  size_t		total_sent;
  ssize_t		sent;

  total_sent = 0;
  fd = check_valid_file(serv, cmd, &data);
  write(serv->sockfd, &data, sizeof(data));
  if (fd > 0)
    {
      while (total_sent < data.size)
	{
	  if ((sent = sendfile(serv->sockfd, fd, NULL, data.size)) < 0)
	    {
	      fprintf(stderr, "Error : sendfile error\n");
	      return ((void *)-1);
	    }
	  total_sent += sent;
	}
      return (NULL);
      close(fd);
    }
  return (NULL);
}
