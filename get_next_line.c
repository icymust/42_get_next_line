/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:53:25 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/02 15:54:14 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char	*nl;//Указатель на символ \n в stash, если он найден
	size_t	line_len;//Длина строки, которую нужно вернуть.
	char	*line;//Строка, которая будет возвращена.

	//Статическая переменная, которая сохраняет данные между вызовами функции. 
	//Используется для хранения данных, которые не были обработаны в предыдущем вызове.
	static char *stash = NULL; 
	char buf[BUFFER_SIZE + 1]; //Буфер для чтения данных из файла.
	ssize_t bytes;             //Количество байт, прочитанных функцией read.
	// Проверка входных данных
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	// Чтение данных в буфер и обновление stash
	while (!gnl_strchr(stash, '\n') && (bytes = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[bytes] = '\0';                    // Завершаем строку
		char *temp = gnl_strjoin(stash, buf); // Склеиваем stash и buf
		if (!temp)
		{
			free(stash);
			stash = NULL;
			return (NULL);
		}
		stash = temp; // Обновляем stash
	}
	// Обработка ошибок read
	// Проверяем, есть ли что отдавать
	if (bytes < 0 || stash == NULL || stash[0] == '\0')
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	// Находим перевод строки
	nl = gnl_strchr(stash, '\n');
	line_len = (nl != NULL) ? (size_t)(nl - stash + 1) : gnl_strlen(stash);
	// Извлекаем строку до '\n' с помощью gnl_substr
	line = gnl_substr(stash, 0, line_len);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	// Обновляем stash на остаток после '\n'
	if (nl)
	{
		char *new_stash = gnl_strdup(nl + 1); // Копируем остаток после \n
		free(stash);
		stash = new_stash;
	}
	else
	{
		free(stash);
		stash = NULL;
	}
	// Возвращаем строку
	return (line);
}
