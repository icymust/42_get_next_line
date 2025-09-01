/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:53:25 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/01 15:50:56 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*stash = NULL;// Хранилище для накопления данных
	char		buf[BUFFER_SIZE + 1];// Буфер для чтения
	ssize_t		bytes; // ssize_t для результата read
	char		*temp;

	bytes = 0;
    // Проверка входных данных
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
    // Проверяем наличие '\n' в stash перед чтением
	while (!gnl_strchr(stash, '\n') && (bytes = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[bytes] = '\0';// Завершаем строку, чтобы избежать мусора
		temp = gnl_strjoin(stash, buf);// Склеиваем stash и buf
		if (!temp)
		{
			free(stash);
            stash = NULL;
			return (NULL);
		}
		stash = temp;
	}
    // Обработка ошибок read
	if (bytes < 0)
	{
		free(stash);
        stash = NULL;
		return (NULL);
	}
    if(stash == NULL || stash[0] == '\0'){
        free(stash);
        stash = NULL;
        return(NULL);
    }

	// Найти \n в stash, Найти границу строки и посчитать длину
	char *nl = gnl_strchr(stash, '\n');
	size_t line_len;
	if(nl)
		line_len= (size_t)(nl-stash + 1);
	else
		line_len = gnl_strlen(stash);
	// создать строку для копирования из stash до '\n'
	char *line = malloc(line_len +1);
	if(!line){
		free(stash);
		stash = NULL;
		return NULL;
	}
	
	size_t i = 0;
	while(i < line_len){
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
    // Обновить stash, убрать скопированную строку
	if (nl) {
        char *new_stash = gnl_strjoin(NULL, nl + 1); // копируем остаток после \n
        free(stash);
        stash = new_stash;
    } else {
        free(stash);
        stash = NULL;
    }
    return line;

}
