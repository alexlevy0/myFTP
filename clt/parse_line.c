#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "client.h"

void		epur_str(char *str)
{
  int           i;
  int           j;

  i = 0;
  j = 0;
  while (str[i] == ' ' || str[i] == '\t')
    i++;
  while (str[i])
    if (str[i] != ' ' && str[i] != '\t')
      str[j++] = str[i++];
    else
      {
	str[j++] = ' ';
        while (str[i] == ' ' || str[i] == '\t')
          i++;
      }
  str[j] = 0;
  if (str[j - 1] == ' ')
    str[j - 1] = '\0';
}

int		my_getline(t_client *client)
{
  int		ret;

  memset(client->command_line, 0, CMD_SIZE);
  ret = read(0, client->command_line, CMD_SIZE - 1);
  if (ret < 0)
    {
      perror("read");
      return (EXIT_FAILURE);
    }
  else if (ret == 0)
    return (EXIT_FAILURE);
  client->command_line[ret - 1] = '\0';
  return (EXIT_SUCCESS);
}

void		put_in_tab(t_client *client)
{
  char		*saveptr;
  char		*subtoken;

  subtoken = strtok_r(client->command_line, " ", &saveptr);
  client->arg[0] = subtoken;
  subtoken = strtok_r(NULL, " ", &saveptr);
  client->arg[1] = subtoken;
}
