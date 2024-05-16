/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 15:50:24 by lribette          #+#    #+#             */
/*   Updated: 2024/05/16 16:11:24 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server.hpp"

class Error
{
    public:
        class Exception : public std::exception
        {
            private:
                const char *errorMessage;
            public:
                Exception(const char *err) : errorMessage(err) {}

                const char  *what() const throw()
                {
                    return (errorMessage);
                }
        };
};
