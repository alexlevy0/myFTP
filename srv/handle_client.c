#include <string.h>
#include <stdlib.h>
#include "serveur.h"

static int		init_serv_struct(t_serveur *serveur, int sockfd,
					 struct sockaddr_in *cli_addr)
{
  char			*path;

  serveur->sockfd = sockfd;
  serveur->cli_addr = cli_addr;
  path = realpath("./", NULL);
  if (!path)
    {
      perror("realpath");
      return (EXIT_FAILURE);
    }
  strncpy(serveur->base_pwd, path, PATH_MAX);
  strncpy(serveur->current_pwd, "/", PATH_MAX);
  return (EXIT_SUCCESS);
}

int		get_data(int fd, void *data, size_t size)
{
  size_t	rcv;
  size_t	ret;

  rcv = 0;
  while (rcv != size)
    {
      ret = read(fd, (char *)data + rcv, size - rcv);
      if (ret <= 0)
	return (EXIT_FAILURE);
      rcv += ret;
    }
  return (EXIT_SUCCESS);
}

void			handle_client(int sockfd, struct sockaddr_in *cli_addr)
{
  t_cmd			cmd;
  int			ret;
  t_serveur		serveur;

  printf("server: got connection from %s\n", inet_ntoa(cli_addr->sin_addr));
  init_serv_struct(&serveur, sockfd, cli_addr);
  while (1)
    {
      memset(&cmd, 0, sizeof(cmd));
      ret = get_data(sockfd, &cmd, sizeof(cmd));
      if (ret == EXIT_FAILURE)
	{
	  printf("client exited\n");
	  return;
	}
      if (run_cmd_client(&serveur, &cmd) == EXIT_FAILURE)
	return ;
    }
}
