#include <string.h>
#include <stdlib.h>
#include "serveur.h"

static t_serveur_asso_func g_assofunc[] =
{
    {"get", get_serveur},
    {"put", put_serveur},
    {"ls", ls_serveur},
    {"cd", cd_serveur},
    {"pwd", pwd_serveur},
    {NULL, NULL}
  };

int			run_cmd_client(t_serveur *serveur, t_cmd *cmd)
{
  int			i;
  char			*ret;
  t_data		data;
  int			success;

  i = -1;
  while (g_assofunc[++i].cmd != NULL)
    if (!strcmp(g_assofunc[i].cmd, cmd->arg1))
      {
	memset(&data, 0, sizeof(data));
	ret = g_assofunc[i].func(serveur, cmd);
	if (ret == (void *)-1)
	  return (EXIT_FAILURE);
	if (ret != NULL)
	  {
	    success = !strncmp("Success", ret, strlen("Success"));
	    snprintf(data.data, DATA_SIZE, "%s%s%s",
		     success ? COLOR_GREEN : COLOR_RED, ret, COLOR_RESET);
	    data.flags = MSG_END;
	    if (send_result_client(serveur->sockfd, &data))
	      return (EXIT_FAILURE);
	  }
      }
  return (EXIT_SUCCESS);
}
