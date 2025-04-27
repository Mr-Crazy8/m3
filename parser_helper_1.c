#include "minishell.h" //5 func

static char	*add(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	size_t	j;
	char	*s3;

	i = 0;
	j = 0;
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s3 = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (s3 == NULL)
		return (NULL);
	while (i < s1_len)
	{
		s3[i] = s1[i];
		i++;
	}
	while (j < s2_len)
		s3[i++] = s2[j++];
	s3[i] = '\0';
	return (s3);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	s3 = add(s1, s2);
	return (s3);
}



