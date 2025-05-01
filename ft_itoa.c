#include "minishell.h"

int	get_num_length(long num)
{
	int	length;

	length = 0;
	if (num == 0)
		return (1);
	if (num < 0)
	{
		num = num * -1;
		length++;
	}
	while (num > 0)
	{
		num = num / 10;
		length++;
	}
	return (length);
}

void	fill_num_string(char *str, long num, int len)
{
	str[len] = '\0';
	if (num == 0)
		str[0] = '0';
	else
	{
		if (num < 0)
		{
			str[0] = '-';
			num = num * -1;
		}
		while (num)
		{
			len--;
			str[len] = (num % 10) + '0';
			num = num / 10;
		}
	}
}

char	*ft_itoa(int n)
{
	int		length;
	long	num;
	char	*result;

	num = n;
	length = get_num_length(num);
	result = malloc(length + 1);
	if (!result)
		return (NULL);
	fill_num_string(result, num, length);
	return (result);
}

int	ft_isalnum(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

int	is_valid_var_char(char c)
{
	if ((ft_isalnum(c)) || (c == '_'))
		return (1);
	else
		return (0);
}
