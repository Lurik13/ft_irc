/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:45:54 by lribette          #+#    #+#             */
/*   Updated: 2024/05/17 11:37:24 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream> 
#include <unistd.h> // close()
#include <sys/types.h> // getaddrinfo()
#include <sys/socket.h> // getaddrinfo()
#include <netdb.h> // getaddrinfo()
#include <arpa/inet.h> // htonl()
#include <netinet/in.h> // struct sockaddr_in
#include <exception>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <poll.h>
#include <vector>
#include <algorithm>

#define RESET "\e[0m"
#define BLACK "\e[38;2;0;0;0;1m"
#define RED "\e[38;2;255;0;0;1m"
#define GREEN "\e[38;2;0;255;0;1m"
#define YELLOW "\e[38;2;255;255;0;1m"
#define BLUE "\e[38;2;0;0;255;1m"
#define MAGENTA "\e[38;2;255;0;255;1m"
#define CYAN "\e[38;2;0;255;255;1m"
#define WHITE "\e[38;2;255;255;255;1m"

#define PASSWORD "test"