#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "client.h"

int		main(int ac, char *argv[])
{
  t_client	cstruct;

  if (ac != 3)
    {
      printf("%sUsage%s : %s <host> <port>\n",
	     COLOR_RED, COLOR_RESET, argv[0]);
      return (1);
    }
  memset(&cstruct, 0, sizeof(cstruct));
  if ((cstruct.sockfd = create_socket_client(argv[1], argv[2])) < 0)
    return (EXIT_FAILURE);
  snprintf(cstruct.prompt, PROMPT_SIZE, "%s::%s > ", argv[1], argv[2]);
  while (write(1, cstruct.prompt, sizeof(cstruct.prompt))
	 && (!my_getline(&cstruct)) && strcmp(cstruct.command_line, "quit"))
    {
      epur_str(cstruct.command_line);
      if (exec_cmd(&cstruct))
	break;
    }
  close(cstruct.sockfd);
  return (0);
}
