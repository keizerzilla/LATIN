#include "../include/voxelgrid.h"

uint voxelgrid_idx_offset(uint i, uint j, uint k, uint size_x, uint size_y) {
    return (k * size_x * size_y) + (j * size_x) + i;
}

struct cloud *voxelgrid_sampling(struct cloud *src, real leafsize) {
    if (leafsize <= 0.0) {
        return NULL;
    }

    real apothem = leafsize / 2;

    struct vector3 *centroid = cloud_get_centroid(src);
    struct vector3 *axis_size = cloud_axis_size(src);
    
    uint num_voxels_x = (uint)ceil(axis_size->x/leafsize) + 1;
    uint num_voxels_y = (uint)ceil(axis_size->y/leafsize) + 1;
    uint num_voxels_z = (uint)ceil(axis_size->z/leafsize) + 1;

    struct vector3 *voxel_o = vector3_zero();
    
    voxel_o->x = (cloud_min_x(src)->x + cloud_max_x(src)->x) / 2;
    voxel_o->y = (cloud_min_y(src)->y + cloud_max_y(src)->y) / 2;
    voxel_o->z = (cloud_min_z(src)->z + cloud_max_z(src)->z) / 2;

    voxel_o->x -= (real) floor(apothem * num_voxels_x);
    voxel_o->y -= (real) floor(apothem * num_voxels_y);
    voxel_o->z -= (real) floor(apothem * num_voxels_z);

    uint pos_x, pos_y, pos_z;
    real d_x, min_x, max_x, d_y, min_y, max_y, d_z, min_z, max_z;
    uint idx_aux;

    struct cloud *output = cloud_new();
    
    uint total_dim = num_voxels_x * num_voxels_y * num_voxels_z;

    struct cloud** clouds = calloc (sizeof (struct cloud*), total_dim);
    if (clouds == NULL) {
        printf("%s: erro alocando memoria cloud", __FUNCTION__);
        return NULL;
    }

    for (struct pointset *set = src->points; set != NULL; set = set->next) {
        d_x = set->point->x - voxel_o->x;
        if (d_x < 0.0) {
            pos_x = 0;
        } else {
            pos_x = (uint)ceil(d_x / leafsize);
        
            min_x = voxel_o->x + (pos_x * leafsize) - apothem;
            max_x = voxel_o->x + (pos_x * leafsize) + apothem;

            if (set->point->x < centroid->x) {
                if (set->point->x < min_x) {
                    pos_x--;
                } else if (set->point->x >= max_x) {
                    pos_x++;
                }
            } else if (set->point->x > centroid->x) {
                if (set->point->x <= min_x) {
                    pos_x--;
                } else if (set->point->x > max_x) {
                    pos_x++;
                }
            }
        }
        
        d_y = set->point->y - voxel_o->y;
        if (d_y < 0.0) {
            pos_y = 0;
        } else {
            pos_y = (uint) ceil(d_y / leafsize);
            min_y = voxel_o->y + (pos_y * leafsize) - apothem;
            max_y = voxel_o->y + (pos_y * leafsize) + apothem;
            if (set->point->y < centroid->y) {
                if (set->point->y < min_y) {
                    pos_y--;
                } else if (set->point->y >= max_y) {
                    pos_y++;
                }
            } else if (set->point->y > centroid->y) {
                if (set->point->y <= min_y) {
                    pos_y--;
                } else if (set->point->y > max_y) {
                    pos_y++;
                }
            }
        }
        
        d_z = set->point->z - voxel_o->z;
        if (d_z < 0.0) {
            pos_z = 0;
        } else {
            pos_z = (uint) ceil(d_z / leafsize);
            min_z = voxel_o->z + (pos_z * leafsize) - apothem;
            max_z = voxel_o->z + (pos_z * leafsize) + apothem;
            if (set->point->z < centroid->z) {
                if (set->point->z < min_z) {
                    pos_z--;
                } else if (set->point->z >= max_z) {
                    pos_z++;
                }
            } else if (set->point->z > centroid->z) {
                if (set->point->z <= min_z) {
                    pos_z--;
                } else if (set->point->z > max_z) {
                    pos_z++;
                }
            }
        }

        idx_aux = voxelgrid_idx_offset(pos_x,
                                       pos_y,
                                       pos_z,
                                       num_voxels_x,
                                       num_voxels_y);

        if (clouds[idx_aux] == 0) {
            clouds[idx_aux] = cloud_new();
        }

        cloud_insert_vector3(clouds[idx_aux], set->point);
    }

    for (uint i = 0; i < num_voxels_x; i++) {
        for (uint j = 0; j < num_voxels_y; j++) {
            for (uint k = 0; k < num_voxels_z; k++) {
                idx_aux = voxelgrid_idx_offset(i,
                                               j,
                                               k,
                                               num_voxels_x,
                                               num_voxels_y);
				
                if (clouds[idx_aux] != 0) {
                    cloud_insert_vector3(output, 
                                         cloud_calc_centroid(clouds[idx_aux]));
                    cloud_free(&clouds[idx_aux]);
                }
            }
        }
    }
    
    free(clouds);

    return output;
}

