/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:57:28 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/15 14:11:20 by pmihangy         ###   ########.fr       */
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

/*
La commande `export` dans les systèmes UNIX est utilisée pour ajouter ou modifier des variables d'environnement. Ces variables d'environnement sont ensuite accessibles à tous les processus fils créés par le shell.

Voici un aperçu détaillé de la gestion des cas que tu devras prendre en compte pour implémenter cette commande en C, avec des exemples spécifiques comme ceux que tu as fournis :

1. **`export DONTO`**  
   - **Action** : Ici, on essaie d'exporter la variable `DONTO`, mais elle n’a pas de valeur assignée. Le comportement typique dans le shell est de simplement ajouter `DONTO` aux variables d'environnement sans valeur.
   - **À gérer** : Ajouter `DONTO` à l'environnement si elle n'existe pas déjà, mais sans valeur associée.

2. **`export "'DONTO'"`**  
   - **Action** : Ici, le nom de la variable est invalide à cause des guillemets simples autour de `DONTO`.
   - **À gérer** : Afficher un message d'erreur. Le nom de la variable d'environnement doit être une chaîne sans caractères spéciaux comme des guillemets.

3. **`export "'DONTO'"=DONTO`**  
   - **Action** : Le nom de la variable est entouré de guillemets simples, ce qui est également invalide.
   - **À gérer** : Afficher un message d'erreur similaire à celui ci-dessus, car le nom de la variable contient des guillemets.

4. **`export DONTO=DONTO`**  
   - **Action** : Ici, on assigne la valeur `DONTO` à la variable `DONTO`. Le comportement attendu est d'ajouter `DONTO` à l'environnement avec la valeur `DONTO`.
   - **À gérer** : Insérer `DONTO=DONTO` dans les variables d'environnement.

5. **`export "DONTO"=DONTO`**  
   - **Action** : Même si le nom `DONTO` est entouré de guillemets doubles, cela reste valide, car les guillemets sont utilisés pour protéger les espaces ou caractères spéciaux. Le nom de la variable ici est toujours `DONTO`.
   - **À gérer** : Ajouter `DONTO=DONTO` dans l'environnement.

6. **`export "DONTO=DONTO`**  
   - **Action** : Il y a une erreur de syntaxe ici, car il manque la fermeture du guillemet. Cela devrait être considéré comme une erreur.
   - **À gérer** : Afficher un message d'erreur de syntaxe.

7. **`export DONTO="DONTO"`**  
   - **Action** : Ici, on assigne la valeur `DONTO` à la variable `DONTO`, mais la valeur est entourée de guillemets doubles, ce qui est valide.
   - **À gérer** : Ajouter `DONTO=DONTO` dans l'environnement.

8. **`export DONTO="'DONTO'"`**  
   - **Action** : Dans ce cas, on assigne la valeur `'DONTO'` (avec les guillemets simples) à la variable `DONTO`. C'est valide.
   - **À gérer** : Ajouter `DONTO='DONTO'` dans l'environnement.

### Cas supplémentaires à gérer :

- **`export` sans argument** : Affiche la liste des variables d'environnement.
- **`export var+=value`** : Certains shells supportent l'addition à la valeur existante d'une variable (non standard, mais utile à considérer).
- **Noms de variables invalides** : Les noms de variables ne peuvent pas commencer par un chiffre, contenir des caractères spéciaux, etc. Gérer cela avec un message d'erreur.

### Implémentation en C

Ton programme devra :
1. Vérifier la validité des noms de variables (alphanumériques, pas de caractères spéciaux).
2. Gérer les erreurs de syntaxe (comme les guillemets non fermés).
3. Mettre à jour l'environnement en utilisant une fonction comme `setenv()` ou `putenv()` pour ajouter ou modifier les variables.

Cela t'aidera à couvrir les différents cas comme demandé.
*/

/*
 * STEPS:
 * 	1 - manage errors
 * 	2 - if the argument is valid:
 * 		- if the variable already exist, we update his value
 * 		- else we create a new variable at the end
 
 *
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

bool	is_surround_by_quotes(char *str)
{
	int		i;
	char	quote;

	i = ft_strlen(str) - 2;
	printf("0: %c\n", str[0]);
	printf("l: %c\n", str[i]);
	/*if (str[0] == '\'' || str[0] == '\"')*/
	/*{*/
		/*quote = str[0];*/
		/*if (str[j] == quote)*/
			/*return (true);*/
	/*}*/
	return (false);
}

bool	export_minishell(char **env, char **to_export)
{
	int		i;
	char	*str;

	if (!to_export)
	{
		env_minishell(env);
		return (true);
	}
	// TODO:  manage errors
	i = 0;
	while (to_export[i])
	{
		/*if (has_open_quote(to_export[i], false, 0))*/
			/*printf("open quote\n");*/
		/*if (!has_equal(to_export[i]))*/
		/*{*/
			/*if (has_quote(to_export[i]))*/
			/*{*/
				/*if (is_surround_by_quotes(to_export[i]))*/
				/*{*/
					/*printf("is surrounded\n");*/
					/*[>str = trim_str(to_export[i]);<]*/
					/*[>if (str == NULL)<]*/
						/*[>return (false);<]*/
					/*[>if (!validate(str))<]*/
						/*[>printf("%s is not valid\n", str);<]*/
				/*}*/
			/*}*/
		/*}*/
		/*else*/
		/*{*/
			/*// TODO: do the same thing	*/
		/*}*/
		printf("%s\n", to_export[i]);
		++i;
	}
	return (true);
}
