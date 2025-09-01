/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 13:05:16 by mmustone          #+#    #+#             */
/*   Updated: 2025/08/29 13:05:40 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t gnl_strlen(const char *s){
    size_t i = 0;
    if(!s)
        return(0);
    while(s[i])
        i++;
    return(i);
}

char *gnl_strchr(const char *s, size_t c){
    if(!s)
        return(NULL);
    while(*s){
        if(*s == (char)c)
            return((char *)s);
        s++;
    }
    if((char)c == '\0')
        return((char *)s);
    return(NULL);
}

char *gnl_strjoin(char *s1, const char *s2){
    size_t len1 = gnl_strlen(s1);
    size_t len2 = gnl_strlen(s2);
    char *ns = malloc(len1 + len2 + 1);
    if (!ns)
        return NULL;
    size_t i = 0;
    if (s1) {
        while (s1[i]) {
            ns[i] = s1[i];
            i++;
        }
    }
    size_t j = 0;
    while (s2 && s2[j]) {
        ns[i + j] = s2[j];
        j++;
    }
    ns[i + j] = '\0';
    free(s1);
    return ns;
}