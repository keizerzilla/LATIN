/**
 * \file extraction.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Funções de interface de programa para análise de nuvens de pontos
 */

#include <unistd.h>

#include "hu.h"
#include "zernike.h"
#include "legendre.h"
#include "chebyshev.h"
#include "matrix.h"
#include "cloud.h"
#include "util.h"

#define HU_TUTU "hututu"
#define HU_1980 "hu1980"
#define ZERNIKE "zernike"
#define LEGENDRE "legendre"
#define CHEBYSHEV "chebyshev"

/**
 * @brief Exibe mensagem ao usuário informando como usar o extrator de momentos
 */
void extraction_help()
{
    util_error("faltando argumentos!");
    printf("argumentos obrigatorios: [ -m | -c | -o ]\n");
    printf("  -m: momento usado para extracao de atributos\n");
    printf("      > hututu, hu1980, zernike, legendre ou chebyshev\n");
    printf("  -c: nuvem de entrada no formato XYZ\n");
    printf("  -o: arquivo aonde os momentos serao salvos\n");
    printf("      > path para arquivo texto ou stdout para saida padrao\n");
    printf("ex1: mcalc -m hu_1980 -i ../data/cloud1.xyz -o hu1.txt\n");
    printf("ex2: mcalc -m legendre -i ../dataset/bunny.xyz -o stdout\n");
}

/**
 * \brief Interface para análise de nuvens de pontos
 * \param argc Número de parâmetros passados pela linha de comando
 * \param argv Parâmetros passados por linha de comando
 */
void extraction_interface(int argc, char** argv)
{
    char* optm = NULL;
    char* opti = NULL;
    char* opto = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "m:c:o:")) != -1) {
        switch (opt) {
            case 'm':
                optm = optarg;
                break;
            case 'c':
                opti = optarg;
                break;
            case 'o':
                opto = optarg;
                break;
            default:
                abort();
        }
    }

    if (optm == NULL || opti == NULL || opto == NULL) {
        extraction_help();
        exit(1);
    }

    struct matrix* (*mfunc)(struct cloud*) = &hu_cloud_moments_artur_tcc;
    if (!strcmp(optm, HU_TUTU))
        mfunc = &hu_cloud_moments_artur_tcc;
    else if(!strcmp(optm, HU_1980))
        mfunc = &hu_cloud_moments_sadjadi_hall;
    else if (!strcmp(optm, LEGENDRE))
        mfunc = &legendre_cloud_moments;
    else if (!strcmp(optm, CHEBYSHEV))
        mfunc = &chebyshev_cloud_moments;
    else if (!strcmp(optm, ZERNIKE))
        mfunc = &zernike_cloud_moments;

    struct cloud* cloud = cloud_load_xyz(opti);
    if (cloud == NULL) {
        util_seg("abortando");
        exit(1);
    }

    struct matrix* results = (*mfunc)(cloud);

    if (!strcmp(opto, "stdout"))
        matrix_debug(stdout, results);
    else
        matrix_save_to_file(results, opto);

    matrix_free(results);
    cloud_free(cloud);
}

