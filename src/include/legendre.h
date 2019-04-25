/**
 * \file legendre.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2017
 * \brief Implementações necessárias para o cálculo dos momentos de Legendre
 * para nuvens de pontos. O momentos de Legendre são derivados da relação
 * recursiva dos polinômios de Legendre.
 */

#ifndef LEGENDRE_H
#define LEGENDRE_H

#define LEGENDRE_ORDER 2
#define LEGENDRE_MOMENTS 27

#include "cloud.h"
#include "matrix.h"

/**
 * \brief Calcula o polinômio de Legendre
 * \param n A ordem do polinômio
 * \param x O argumento do polinômio
 * \return O polinômio de x de ordem n
 */
real legendre_poly(int n, real x)
{
    if (n < 0)
        return 0.0f;

    if (n == 0)
        return 1.0f;

    if (n == 1)
        return x;

    return (((2 * n) - 1) * x * legendre_poly(n-1, x) - (n - 1) *
            legendre_poly(n - 2, x)) / (1.0f * n);
}

/**
 * \brief Calcula a coordenada normalizada de legendre
 * \param c O valor original da coordenada
 * \param numpts O número de pontos da nuvem
 * \return A coordenada normalizada
 */
real legendre_normcoord(real c, int numpts)
{
	return ((2.0f * c) - numpts - 1) / (numpts - 1);
}

/**
 * \brief Calcula a constante de normalização
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return A constante de normalização(p,q,r)
 */
real legendre_norm(int p, int q, int r, struct cloud* cloud)
{
	real num = ((2.0f * p) + 1) * ((2.0f * q) + 1) * ((2.0f * r) + 1);
    real den = 1.0f * cloud_size(cloud);
    
    return num / den;
}

/**
 * \brief Calcula o momento de legendre
 * \param p A ordem da dimensão x
 * \param q A ordem da dimensão y
 * \param r A ordem da dimensão z
 * \param cloud A nuvem alvo
 * \return O momento de ordem p+q+r da nuvem cloud
 */
real legendre_moment(int p, int q, int r, struct cloud* cloud)
{
    struct vector3* center = cloud_get_center(cloud);
	int numpts = cloud_size(cloud);
	
	center->x = legendre_normcoord(center->x, numpts);
	center->y = legendre_normcoord(center->y, numpts);
	center->z = legendre_normcoord(center->z, numpts);
	
	real moment = 0.0f;
	real center_x = 0.0f;
	real center_y = 0.0f;
	real center_z = 0.0f;
	
    for (uint i = 0; i < cloud->num_pts; i++) {
    	struct vector3* point = vector3_from_vector(&cloud->points[i]);
    	
    	point->x = legendre_normcoord(point->x, numpts);
    	point->y = legendre_normcoord(point->y, numpts);
    	point->z = legendre_normcoord(point->z, numpts);
    	
    	center_x = point->x - center->x;
		center_y = point->y - center->y;
		center_z = point->z - center->z;
		
        moment += legendre_poly(p, center_x) *
                  legendre_poly(q, center_y) *
                  legendre_poly(r, center_z) *
                  vector3_distance(point, center);
		
		vector3_free(point);
	}
	
	vector3_free(center);
	
    return legendre_norm(p, q, r, cloud) * moment;
}

/**
 * \brief Calcula os momentos de Legendre de uma nuvem
 * \param cloud A nuvem alvo
 * \return A matrix aonde os resultados serão salvos
 */
struct matrix* legendre_cloud_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, LEGENDRE_MOMENTS);

    int p = 0;
    int q = 0;
    int r = 0;
    int col = 0;

    for (p = 0; p <= LEGENDRE_ORDER; p++) {
        for (q = 0; q <= LEGENDRE_ORDER; q++) {
            for (r = 0; r <= LEGENDRE_ORDER; r++) {
                matrix_set(results, 0, col, legendre_moment(p, q, r, cloud));
                col++;
            }
        }
    }

    return results;
}

/**
 * \brief Calcula os momentos invariantes de Legendre de uma nuvem (!!!TEMP!!!)
 * \param cloud A nuvem alvo
 * \return A matrix aonde os resultados serão salvos
 */
struct matrix* legendre_invariant_moments(struct cloud* cloud)
{
    struct matrix* results = matrix_new(1, 6);
	
	real m200 = legendre_moment(2, 0, 0, cloud);
    real m020 = legendre_moment(0, 2, 0, cloud);
    real m002 = legendre_moment(0, 0, 2, cloud);
    real m110 = legendre_moment(1, 1, 0, cloud);
    real m101 = legendre_moment(1, 0, 1, cloud);
    real m011 = legendre_moment(0, 1, 1, cloud);
	
    real i1 = m200 + m020 + m002;
    real i2 = m200*m020 + m200*m002 + m020*m002 -
              m110*m110 - m101*m101 - m011*m011;
    real i3 = m200*m020*m002 + 2*m110*m101*m011 -
              m002*m110*m110 - m020*m101*m101 - m200*m011*m011;
	
	real j1 = i1;
	real j2 = i1*i1 - 2*i2;
	real j3 = i1*i1*i1 - 3*i1*i2 + 3*i3;
	
    matrix_set(results, 0, 0, i1);
    matrix_set(results, 0, 1, i2);
    matrix_set(results, 0, 2, i3);
    matrix_set(results, 0, 3, j1);
    matrix_set(results, 0, 4, j2);
    matrix_set(results, 0, 5, j3);

    return results;
}

#endif // LEGENDRE_H

