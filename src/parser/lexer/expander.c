#include <minishell.h>

/*
 * This function is called by extract_text function
 * It convert an env variable by his value
 */
void	insert_env_in_s(char *text, char *s, int *i, int *j)
{
	char	*env;
	int		t;

	env = getenv(ft_substr(text, *i + 1, calc_env_len(text, *i + 1)));	
	t = 0;
	while (env[t])
	{
		s[*j] = env[t];
		++(*j);
		++t;
	}
	(*i) += calc_env_len(text, *i + 1) + 1;
}

/*
 * This function transform text by changing
 * all the env variables inside by their values
 */
char	*extract_text(char *text)
{
	char	*s;
	int		i;
	int		j;

	s = malloc((count_text_size(text) + 1) * sizeof(char));
	IF_RETURN(!s, NULL)
	i = 0;
	j = 0;
	while (text[i])
	{
		if (text[i] != '$')
		{
			s[j] = text[i];
			++j;
			++i;
		}
		else
			insert_env_in_s(text, s, &i, &j);
	}
	s[j] = '\0';
	return (s);
}

/*
 * This function create a cmd token
 * and append it in the list of tokens
 */
bool	insert_cmd_token(char *entry, t_token **root, int *i)
{
	char	*text;
	int		token_len;

	token_len = calc_text_token_len(entry, *i);
	text = ft_substr(entry, *i, token_len);
	IF_RETURN(!text, false)
	tokens_append(root, new_token(text, CMD));
	(*i) += token_len;
	return (true);
}

/*
 * This function create a argument token
 * and add it in the list of tokens
 *
 * If we have a '$' in the beginning of the argument
 * it transform it by the value of the variable env value
 *
 */
bool	insert_argument_token(char *entry, t_token **root, int *i)
{
	char	*text;
	int		token_len;

	if (entry[*i] == '\"' || entry[*i] == '\'')
	{
		token_len = calc_text_in_quote_len(entry, *i, entry[*i]);
		text = ft_strtrim(ft_substr(entry, *i, token_len), "\"\'");
		if (entry[*i] == '\"' && has_an_env(text))
			text = extract_text(text);
	}
	else
	{
		token_len = calc_text_token_len(entry, *i);
		text = ft_strtrim(ft_substr(entry, *i, token_len), "\"\'");
		if (text[0] == '$')
			text = getenv(ft_substr(text, 1, ft_strlen(text) - 1));
	}
	IF_RETURN(!text, false)
	tokens_append(root, new_token(text, ARGUMENT));
	(*i) += token_len;
	return (true);
}
