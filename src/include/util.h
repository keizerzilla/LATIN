/**
 * \file util.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Arquivo com funções utilitárias genéricas.
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

#define ANSI_COLOR_DEFAULT		"\033[00m"
#define ANSI_COLOR_BOLD			"\033[01m"
#define ANSI_COLOR_UNDERLINE	"\033[04m"
#define ANSI_COLOR_RED			"\033[31m"
#define ANSI_COLOR_GREEN		"\033[32m"
#define ANSI_COLOR_BLUE			"\033[34m"
#define ANSI_COLOR_YELLOW		"\033[33m"
#define ANSI_COLOR_MAGENTA		"\033[35m"
#define ANSI_COLOR_CYAN			"\033[36m"
#define ANSI_COLOR_WHITE		"\033[37m"
#define ANSI_COLOR_FAIL			"\033[91m"
#define ANSI_COLOR_OKGREEN		"\033[92m"
#define ANSI_COLOR_WARNING		"\033[93m"
#define ANSI_COLOR_OKBLUE		"\033[94m"
#define ANSI_COLOR_HEADER		"\033[95m"

/**
 * @brief Mensagem de nível de erro
 * @param msg A mensagem a ser exibida
 */
void util_error(const char* msg, ...)
{
    char buffer[80];
    va_list arglist;

    va_start(arglist, msg);
    vsprintf(buffer, msg, arglist);
    va_end(arglist);
	
	fprintf(stderr, "%s%s", ANSI_COLOR_BOLD, ANSI_COLOR_FAIL);
    fprintf(stderr, "[ERRO]\t%s\n", buffer);
    fprintf(stdout, "%s", ANSI_COLOR_DEFAULT);
}

/**
 * @brief Mensagem de nível de informação/aviso
 * @param msg A mensagem a ser exibida
 */
void util_info(const char* msg, ...)
{
    char buffer[80];
    va_list arglist;

    va_start(arglist, msg);
    vsprintf(buffer, msg, arglist);
    va_end(arglist);
	
	fprintf(stderr, "%s%s", ANSI_COLOR_BOLD, ANSI_COLOR_OKBLUE);
    fprintf(stdout, "[INFO]\t%s\n", buffer);
    fprintf(stdout, "%s", ANSI_COLOR_DEFAULT);
}

/**
 * @brief Mensagem de nível de programa
 * @param msg A mensagem a ser exibida
 */
void util_prompt(const char* msg, ...)
{
    char buffer[80];
    va_list arglist;

    va_start(arglist, msg);
    vsprintf(buffer, msg, arglist);
    va_end(arglist);
	
	fprintf(stderr, "%s%s", ANSI_COLOR_BOLD, ANSI_COLOR_OKGREEN);
    fprintf(stdout, "[!!!!]\t%s\n", buffer);
    fprintf(stdout, "%s", ANSI_COLOR_DEFAULT);
}

/**
 * @brief Mensagem de nível de segmentação
 * @param msg A mensagem a ser exibida
 */
void util_seg(const char* msg, ...)
{
    char buffer[80];
    va_list arglist;

    va_start(arglist, msg);
    vsprintf(buffer, msg, arglist);
    va_end(arglist);
	
	fprintf(stderr, "%s%s", ANSI_COLOR_BOLD, ANSI_COLOR_MAGENTA);
    fprintf(stdout, "[!!!!]\t%s\n", buffer);
    fprintf(stdout, "%s", ANSI_COLOR_DEFAULT);
}

#endif

