/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:57:28 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/22 10:04:33 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
 		MANUEL
NAME
    export - Set export attribute for shell variables.

SYNOPSIS
    export [-fn] [name[=value] ...] or export -p

DESCRIPTION
    Set export attribute for shell variables.
    
    Marks each NAME for automatic export to the environment of subsequently
    executed commands.  If VALUE is supplied, assign VALUE before exporting.
    
    Options:
      -f	refer to shell functions
      -n	remove the export property from each NAME
      -p	display a list of all exported variables and functions
    
    An argument of `--' disables further option processing.
    
    Exit Status:
    Returns success unless an invalid option is given or NAME is invalid.

SEE ALSO
    bash(1)

IMPLEMENTATION
    GNU bash, version 5.0.17(1)-release (x86_64-redhat-linux-gnu)
    Copyright (C) 2019 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>

*/
/* Voici les principaux cas où la commande `export` avec un paramètre est considérée comme invalide :

1. **Nom de variable invalide** :
   - Le nom de la variable doit respecter certaines règles : il doit commencer par une lettre (ou un underscore `_`) et ne peut contenir que des lettres, chiffres ou underscores.
   - **Exemples** :
     - `export 1VAR=VALUE` : invalide (le nom commence par un chiffre).
     - `export VAR@NAME=VALUE` : invalide (le nom contient un caractère spécial).

2. **Caractères non autorisés dans le nom de la variable** :
   - Si le nom de la variable contient des caractères spéciaux comme `!`, `@`, `$`, `%`, etc., cela rend la commande invalide.
   - **Exemples** :
     - `export VAR!NAME=VALUE` : invalide.
     - `export V$AR=VALUE` : invalide.

3. **Guillemets mal formés** :
   - Si des guillemets sont utilisés, ils doivent être correctement ouverts et fermés. Si ce n'est pas le cas, une erreur de syntaxe se produit.
   - **Exemples** :
     - `export "VAR=VALUE` : invalide (guillemet ouvrant sans guillemet fermant).
     - `export VAR="VALUE` : invalide (guillemet fermant manquant pour la valeur).

4. **Tentative d’exportation de chaînes de caractères avec guillemets** :
   - Exporter un nom de variable entouré de guillemets simples ou doubles rend la commande invalide, car le shell ne reconnaît pas le nom de la variable avec ces guillemets.
   - **Exemples** :
     - `export "VAR"` : invalide (les guillemets autour de `VAR` sont interprétés comme un nom de variable littéral).
     - `export 'VAR'=VALUE` : invalide (guillemets simples autour du nom de la variable).

5. **Mauvaise syntaxe avec des symboles `=`** :
   - Si le symbole `=` est mal positionné ou utilisé incorrectement, la commande est invalide.
   - **Exemples** :
     - `export =VALUE` : invalide (il manque le nom de la variable avant le `=`).
     - `export VAR==VALUE` : invalide (deux symboles `=` dans la même commande).

6. **Espaces mal gérés autour du signe `=`** :
   - Les espaces entre le nom de la variable et le signe `=` peuvent causer des erreurs, car le shell les interprète comme des séparateurs.
   - **Exemples** :
     - `export VAR = VALUE` : invalide (il ne doit pas y avoir d'espace autour du `=`).

7. **Tentative d’exporter des expressions invalides** :
   - Si une expression est fournie, mais qu'elle ne respecte pas les règles du shell, elle est considérée comme invalide.
   - **Exemple** :
     - `export VAR+VALUE` : invalide (le `+` n'est pas une syntaxe supportée pour l'export).

En résumé, une commande `export` est invalide dans les cas suivants : nom de variable incorrect, guillemets mal formés, mauvaise utilisation du signe `=`, ou syntaxe invalide dans l’expression fournie. Le shell attend des noms de variables corrects et des valeurs bien formatées.
 */

/*
Voici les différents cas où la valeur d'une variable dans la commande `export` est considérée comme invalide :

---

### 1. **Guillemets mal formés dans la valeur** :

Si des guillemets sont utilisés dans la valeur mais ne sont pas correctement ouverts ou fermés, cela entraîne une erreur de syntaxe.

**Exemples** :
- `export VAR="VALUE` : invalide (le guillemet ouvrant n'est pas fermé).
- `export VAR=VALUE"` : invalide (le guillemet fermant apparaît sans guillemet ouvrant).
- `export VAR='VALUE"` : invalide (mélange de guillemets simples et doubles).

---

### 2. **Utilisation non échappée de caractères spéciaux dans la valeur** :

Les caractères spéciaux comme `$`, `\`, `!`, et d'autres doivent être correctement échappés s'ils sont utilisés dans la valeur. Sinon, ils provoquent des erreurs ou des comportements inattendus.

**Exemples** :
- `export VAR="Hello$World"` : invalide (le symbole `$` est interprété comme une expansion de variable).
- `export VAR="Hello\World"` : invalide (le symbole `\` n'est pas échappé correctement).

---

### 3. **Valeur avec des guillemets mal équilibrés** :

Si des guillemets apparaissent au sein de la valeur mais ne sont pas équilibrés (ou mal échappés), cela provoque une erreur.

**Exemples** :
- `export VAR="He said "Hello"` : invalide (guillemets internes non échappés correctement).
- `export VAR='He said "Hello'` : invalide (guillemets simples et doubles mal assortis dans la valeur).

---

### 4. **Valeur incomplète ou sans valeur attribuée** :

Une valeur assignée doit être correctement complétée après le signe `=`. Si la valeur est partiellement assignée ou totalement manquante, c'est invalide.

**Exemples** :
- `export VAR=` : invalide (pas de valeur assignée après le `=`).
- `export VAR= ` : invalide (espace vide après le `=` sans valeur réelle).

---

### 5. **Caractères non imprimables ou non autorisés** :

La valeur ne doit pas contenir de caractères non imprimables ou non autorisés, tels que des retours à la ligne, des tabulations non échappées, ou d'autres caractères de contrôle.

**Exemples** :
- `export VAR="Hello\nWorld"` : invalide (saut de ligne non échappé dans la valeur).
- `export VAR="Hello\tWorld"` : invalide (tabulation non échappée).

---

### 6. **Utilisation non échappée des apostrophes ou doubles apostrophes dans la valeur** :

Si des apostrophes simples ou doubles sont utilisées dans la valeur sans être correctement échappées, cela peut provoquer des erreurs de syntaxe.

**Exemples** :
- `export VAR="It's fine"` : invalide (l'apostrophe dans `It's` n'est pas échappée).
- `export VAR='"Hello"'` : invalide (les guillemets doubles ne sont pas échappés correctement dans la valeur).

---

### 7. **Valeur contenant des commandes shell non intentionnelles** :

Si une commande shell est incluse dans la valeur sans être protégée (par exemple avec des guillemets simples ou un échappement), le shell peut tenter de l'exécuter au lieu de l’interpréter comme une chaîne.

**Exemples** :
- `export VAR=$(rm -rf /)` : invalide (interprétation de la commande `rm -rf /` au lieu d'être une simple chaîne).
- `export VAR=`expr 1 + 1`` : invalide (la commande `expr` sera exécutée, causant une erreur si non voulu).

---

### 8. **Espaces mal protégés dans la valeur** :

Si des espaces sont utilisés dans la valeur sans être entourés de guillemets ou échappés, cela est considéré comme une erreur de syntaxe.

**Exemples** :
- `export VAR=Hello World` : invalide (les espaces dans `Hello World` ne sont pas protégés par des guillemets).
- `export VAR=Hello\ World` : invalide (le caractère d'échappement `\` est mal utilisé pour les espaces).

---

Ces cas couvrent les principales situations où la valeur d'une variable dans une commande `export` est considérée comme invalide. Ils englobent des erreurs de syntaxe liées aux guillemets, caractères spéciaux, espaces non protégés et commandes shell non intentionnelles.
*/

bool	has_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (true);
		++i;
	}
	return (false);
}

bool	has_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
		++i;
	}
	return (false);
}

void	print_error(char *error_mess)
{
	printf("ERROR: %s\n", error_mess);
}

bool	has_error(char *str)
{
	int	i;

	if (str[0] != '_' && !ft_isalpha(str[0]))
	{
		print_error("variable name should start with a letter or _");
		return (true);
	}
	i = 0;
	while (str[i] != '=')
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
		{
			print_error("variable name should only contains letters, digits or _");	
			return (true);
		}
		++i;
	}
	if (str[++i] == '=')
	{
		print_error("two successive equals");
		return (true);
	}
	return (false);
}

char	*get_variable_name(char *str)
{
	char	*name;
	int		i;

	i = 0;
	while (str[i] != '=')
		++i;
	name = malloc((i + 1) * sizeof(char));
	IF_RETURN(!name, NULL)
	ft_memcpy(name, str, i);
	name[++i] = '\0';
	return (name);
}

int	is_already_exist(char *name, char **env)
{
	int		i;
	char	*key;

	i  = 0;
	while (env[i])
	{
		key = get_variable_name(env[i]);
		IF_RETURN(!key, -1)
		IF_RETURN(!ft_strncmp(name, key, 69), 1)
		++i;
	}
	return (0);
}

bool	env_append(char ***env, char *str)
{
	size_t	size;
	char	**new_env;

	size = 0;
	while ((*env)[size])
		++size;
	new_env = malloc((size + 2) * sizeof(char *));
	IF_RETURN(!new_env, false)
	size = 0;
	while ((*env)[size])
	{
		new_env[size] = ft_strdup((*env)[size]);
		IF_RETURN(!new_env[size], false)
		++size;
	}
	new_env[size] = ft_strdup(str);
	IF_RETURN(!new_env[size], false)
	new_env[++size] = NULL;
	*env = new_env;
	return (true);
}

bool	env_update(char ***env, char *str)
{
	int		i;
	char	*name;
	char	**new_env;

	i = 0;
	while ((*env)[i])
		++i;
	new_env = malloc((i + 1) * sizeof(char *));
	IF_RETURN(!new_env, false)
	i = 0;
	while ((*env)[i])
	{
		name = get_variable_name((*env)[i]);
		IF_RETURN(!name, false)
		if (!ft_strncmp(name, get_variable_name(str), 69))
		{
			new_env[i] = ft_strdup(str);
			IF_RETURN(!new_env[i], false)
		}
		else
		{
			new_env[i] = ft_strdup((*env)[i]);
			IF_RETURN(!new_env[i], false)
		}
		++i;
	}
	new_env[i] = NULL;
	*env = new_env;
	return (true);
}

bool	export_minishell(char ***env, char **to_export)
{
	int		i;
	char	*variable_name;

	if (!to_export)
	{
		env_minishell(*env);
		return (true);
	}
	i = 0;
	while (to_export[i])
	{
		if (has_equal(to_export[i]))
		{
			if (!has_error(to_export[i]))
			{
				variable_name = get_variable_name(to_export[i]);
				IF_RETURN(!variable_name, false)
				if (is_already_exist(variable_name, *env) == 1)
					env_update(env, to_export[i]);
				else if (is_already_exist(variable_name, *env) == -1)
					return (false);
				else
					env_append(env, to_export[i]);
			}
		}
		++i;
	}
	return (true);
}
