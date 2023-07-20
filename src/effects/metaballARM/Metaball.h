#pragma once
#include "../../utils/common.h"

class Metaball
{
public:
    // Variables
    // Metaball variables

    /* General metaballs example properties. */
    GLfloat model_time = 0.0f;            /**< Time (in seconds), increased each rendering iteration.                                         */
    const GLuint tesselation_level = 100; /**< Level of details you would like to split model into. Please use values from th range [8..256]. */
    GLfloat isosurface_level = 12.0f;     /**< Scalar field's isosurface level.                                                               */
    unsigned int window_width = 256;      /**< Window width resolution (pixels).                                                              */
    unsigned int window_height = 256;     /**< Window height resolution (pixels).                                                             */

    /* Marching Cubes algorithm-specific constants. */
    const GLuint samples_per_axis = tesselation_level;                                         /**< Amount of samples we break scalar space into (per each axis). */
    const GLuint samples_in_3d_space = samples_per_axis * samples_per_axis * samples_per_axis; /**< Amount of samples in 3D space. */
    const GLuint cells_per_axis = samples_per_axis - 1;                                        /**< Amount of cells per each axis. */
    const GLuint cells_in_3d_space = cells_per_axis * cells_per_axis * cells_per_axis;         /**< Amount of cells in 3D space. */
    static const GLuint vertices_per_triangle = 3;                                             /**< Amount of vertices that defines one triangle. */
    static const GLuint triangles_per_cell = 5;                                                /**< Amount of triangles that can be generated for a single cell by the Marching Cubes algorithm. */
    static const GLuint mc_vertices_per_cell = vertices_per_triangle * triangles_per_cell;     /**< Amount of vertices in tri_table representing triangles by vertices for one cell. */
    static const GLuint mc_cells_types_count = 256;                                            /**< Amount of cell types. */
    /* [tri_table chosen part for documentation] */
    const GLint tri_table[mc_cells_types_count * mc_vertices_per_cell] =
        {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            /* [tri_table chosen part for documentation] */
            1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1,
            3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1,
            3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1,
            3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1,
            9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1,
            1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1,
            9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1,
            2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1,
            8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1,
            9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1,
            4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1,
            3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1,
            1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1,
            4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1,
            4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1,
            9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1,
            1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1,
            5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1,
            2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1,
            9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1,
            0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1,
            2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1,
            10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1,
            4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1,
            5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1,
            5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1,
            9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1,
            0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1,
            1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1,
            10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1,
            8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1,
            2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1,
            7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1,
            9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1,
            2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1,
            11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1,
            9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1,
            5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0,
            11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0,
            11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1,
            1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1,
            9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1,
            5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1,
            2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1,
            0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1,
            5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1,
            6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1,
            0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1,
            3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1,
            6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1,
            5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1,
            1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1,
            10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1,
            6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1,
            1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1,
            8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1,
            7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9,
            3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1,
            5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1,
            0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1,
            9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6,
            8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1,
            5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11,
            0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7,
            6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1,
            10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1,
            10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1,
            8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1,
            1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1,
            3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1,
            0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1,
            10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1,
            0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1,
            3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1,
            6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1,
            9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1,
            8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1,
            3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1,
            6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1,
            0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1,
            10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1,
            10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1,
            1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1,
            2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9,
            7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1,
            7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1,
            2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7,
            1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11,
            11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1,
            8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6,
            0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1,
            7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1,
            10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1,
            2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1,
            6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1,
            7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1,
            2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1,
            1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1,
            10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1,
            10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1,
            0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1,
            7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1,
            6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1,
            8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1,
            9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1,
            6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1,
            1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1,
            4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1,
            10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3,
            8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1,
            0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1,
            1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1,
            8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1,
            10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1,
            4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3,
            10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1,
            5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1,
            11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1,
            9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1,
            6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1,
            7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1,
            3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6,
            7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1,
            9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1,
            3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1,
            6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8,
            9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1,
            1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4,
            4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10,
            7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1,
            6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1,
            3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1,
            0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1,
            6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1,
            1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1,
            0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10,
            11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5,
            6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1,
            5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1,
            9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1,
            1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8,
            1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6,
            10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1,
            0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1,
            5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1,
            10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1,
            11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1,
            0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1,
            9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1,
            7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2,
            2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1,
            8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1,
            9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1,
            9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2,
            1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1,
            9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1,
            9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1,
            5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1,
            0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1,
            10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4,
            2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1,
            0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11,
            0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5,
            9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1,
            5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1,
            3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9,
            5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1,
            8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1,
            0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1,
            9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1,
            0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1,
            1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1,
            3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4,
            4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1,
            9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3,
            11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1,
            11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1,
            2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1,
            9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7,
            3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10,
            1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1,
            4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1,
            4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1,
            0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1,
            3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1,
            3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1,
            0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1,
            9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1,
            1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    /** Amount of spheres defining scalar field. This value should be synchronized between all files. */
    const int n_spheres = 2;

    /** Amount of components in sphere position varying. */
    const int n_sphere_position_components = 4;

    /** Matrix that transforms vertices from model space to perspective projected world space. */
    vmath::mat4 mvp;

    /* 1. Calculate sphere positions stage variable data. */
    /** Program object id for sphere update stage. */
    GLuint spheres_updater_program_id = 0;
    /** Vertex shader id for sphere update stage. */
    GLuint spheres_updater_vert_shader_id = 0;
    /** Fragment shader id for sphere update stage. */
    GLuint spheres_updater_frag_shader_id = 0;

    /** Buffer object id to store calculated sphere positions. */
    GLuint spheres_updater_sphere_positions_buffer_object_id = 0;

    /** Id of transform feedback object to keep sphere update stage buffer bindings. */
    GLuint spheres_updater_transform_feedback_object_id = 0;

    /** Name of time uniform for sphere positions update stage. */
    const GLchar *spheres_updater_uniform_time_name = "time";
    /** Location of time uniform for sphere positions update stage. */
    GLuint spheres_updater_uniform_time_id = 0;

    /** Sphere position output variable's name. */
    const GLchar *sphere_position_varying_name = "sphere_position";

    /* 2. Scalar field generation stage variable data. */
    /** Program object id for scalar field generator stage. */
    GLuint scalar_field_program_id = 0;
    /** Vertex shader id for scalar field generator stage. */
    GLuint scalar_field_vert_shader_id = 0;
    /** Fragment shader id for scalar field generator stage. */
    GLuint scalar_field_frag_shader_id = 0;

    /** Buffer object id to store calculated values of scalar field. */
    GLuint scalar_field_buffer_object_id = 0;

    /** Id of transform feedback object to keep scalar field buffer binding. */
    GLuint scalar_field_transform_feedback_object_id = 0;

    /** Name of samples_per_axis uniform. */
    const GLchar *scalar_field_uniform_samples_per_axis_name = "samples_per_axis";
    /** Location of samples_per_axis uniform. */
    GLuint scalar_field_uniform_samples_per_axis_id = 0;

    /** Name of uniform block storing sphere data. */
    const GLchar *scalar_field_uniform_spheres_name = "spheres_uniform_block";
    /** Index of uniform block storing sphere data. */
    GLuint scalar_field_uniform_spheres_id = 0;

    /** Scalar_field_value output variable's name. */
    const GLchar *scalar_field_value_varying_name = "scalar_field_value";

    /** Id of a 3D texture object storing scalar field data. */
    GLuint scalar_field_texture_object_id = 0;

    /* 3. Marching Cubes cell-splitting stage variable data. */
    /** Program object id for cell splitting stage. */
    GLuint marching_cubes_cells_program_id = 0;
    /** Vertex shader id for cell splitting stage. */
    GLuint marching_cubes_cells_vert_shader_id = 0;
    /** Fragment shader id for cell splitting stage. */
    GLuint marching_cubes_cells_frag_shader_id = 0;

    /** Name of cells_per_axis uniform. */
    const GLchar *marching_cubes_cells_uniform_cells_per_axis_name = "cells_per_axis";
    /** Location of cells_per_axis uniform. */
    GLuint marching_cubes_cells_uniform_cells_per_axis_id = 0;

    /** Name of iso_level uniform. */
    const GLchar *marching_cubes_cells_uniform_isolevel_name = "iso_level";
    /** Location of iso_level uniform. */
    GLuint marching_cubes_cells_uniform_isolevel_id = 0;

    /** Name of scalar_field uniform. */
    const GLchar *marching_cubes_cells_uniform_scalar_field_sampler_name = "scalar_field";
    /** Location of scalar_field uniform. */
    GLuint marching_cubes_cells_uniform_scalar_field_sampler_id = 0;

    /** Cell_type_index output variable's name. */
    const GLchar *marching_cubes_cells_varying_name = "cell_type_index";

    /** Id of transform feedback object to keep cell types buffer binding. */
    GLuint marching_cubes_cells_transform_feedback_object_id = 0;

    /** Id of a buffer object to hold result cell type data. */
    GLuint marching_cubes_cells_types_buffer_id = 0;

    /** Id of a texture object to hold result cell type data. */
    GLuint marching_cubes_cells_types_texture_object_id = 0;

    /* 4. Marching Cubes algorithm triangle generation and rendering stage variable data. */
    /** Program object id for marching cubes algorthim's for rendering stage. */
    GLuint marching_cubes_triangles_program_id = 0;
    /** Vertex shader id for marching cubes algorthim's for rendering stage. */
    GLuint marching_cubes_triangles_frag_shader_id = 0;
    /** Fragment shader id for marching cubes algorthim's for rendering stage. */
    GLuint marching_cubes_triangles_vert_shader_id = 0;

    /** Name of samples_per_axis uniform. */
    const GLchar *marching_cubes_triangles_uniform_samples_per_axis_name = "samples_per_axis";
    /** Location of samples_per_axis uniform. */
    GLuint marching_cubes_triangles_uniform_samples_per_axis_id = 0;

    /** Name of iso_level uniform. */
    const GLchar *marching_cubes_triangles_uniform_isolevel_name = "iso_level";
    /** Location of iso_level uniform. */
    GLuint marching_cubes_triangles_uniform_isolevel_id = 0;

    /** Name of time uniform. */
    const GLchar *marching_cubes_triangles_uniform_time_name = "time";
    /** Location of time uniform. */
    GLuint marching_cubes_triangles_uniform_time_id = 0;

    /** Name of mvp uniform. */
    const GLchar *marching_cubes_triangles_uniform_mvp_name = "mvp";
    /** Location of mvp uniform. */

    /** Name of cell_types uniform. */
    const GLchar *marching_cubes_triangles_uniform_cell_types_sampler_name = "cell_types";
    /** Location of cell_types uniform. */
    GLuint marching_cubes_triangles_uniform_cell_types_sampler_id = 0;

    /** Name of scalar_field uniform. */
    const GLchar *marching_cubes_triangles_uniform_scalar_field_sampler_name = "scalar_field";
    /** Location of scalar_field uniform. */
    GLuint marching_cubes_triangles_uniform_scalar_field_sampler_id = 0;

    /** Name of sphere_positions_uniform_block uniform block. */
    const GLchar *marching_cubes_triangles_uniform_sphere_positions_name = "sphere_positions_uniform_block";
    /** Index of sphere_positions_uniform_block uniform block. */
    GLuint marching_cubes_triangles_uniform_sphere_positions_id = 0;

    /** Name of tri_table uniform. */
    const GLchar *marching_cubes_triangles_uniform_tri_table_sampler_name = "tri_table";
    /** Location of tri_table uniform. */
    GLuint marching_cubes_triangles_uniform_tri_table_sampler_id = 0;

    /** Id of a texture object to hold triangle look-up table data. */
    GLuint marching_cubes_triangles_lookup_table_texture_id = 0;

    /** Id of vertex array object. */
    GLuint marching_cubes_triangles_vao_id = 0;

    BOOL initialize(void)
    {
        // initializeBloom();

        /// INITIALIZATION
        /* Specify one byte alignment for pixels rows in memory for pack and unpack buffers. */
        // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // glPixelStorei(GL_PACK_ALIGNMENT, 1);

        /* 1. Calculate sphere positions stage. */
        /* Create sphere updater program object. */
        spheres_updater_program_id = glCreateProgram();

        /* Load and compile sphere updater shaders. */
        // vertex Shader
        spheres_updater_vert_shader_id = CreateAndCompileShaderObjects(".\\src\\shaders\\metaball\\spheres_updater_vert_shader.glsl", VERTEX);
        spheres_updater_frag_shader_id = CreateAndCompileShaderObjects(".\\src\\shaders\\metaball\\spheres_updater_frag_shader.glsl", FRAGMENT);

        /* Attach the shaders. */
        glAttachShader(spheres_updater_program_id, spheres_updater_vert_shader_id);
        glAttachShader(spheres_updater_program_id, spheres_updater_frag_shader_id);

        /* [Stage 1 Specifying output variables] */
        /* Specify shader varyings (output variables) we are interested in capturing. */
        glTransformFeedbackVaryings(spheres_updater_program_id, 1, &sphere_position_varying_name, GL_SEPARATE_ATTRIBS);

        /* Link the program object. */
        glLinkProgram(spheres_updater_program_id);
        /* [Stage 1 Specifying output variables] */

        /* [Stage 1 Specifying input variables] */
        /* Get input uniform location. */
        spheres_updater_uniform_time_id = glGetUniformLocation(spheres_updater_program_id, spheres_updater_uniform_time_name);
        /* [Stage 1 Specifying input variables] */

        /* Activate spheres updater program. */
        glUseProgram(spheres_updater_program_id);

        /* [Stage 1 Allocate buffer for output values] */
        /* Generate buffer object id. Define required storage space sufficient to hold sphere positions data. */
        glGenBuffers(1, &spheres_updater_sphere_positions_buffer_object_id);
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, spheres_updater_sphere_positions_buffer_object_id);
        glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, n_spheres * n_sphere_position_components * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
        /* [Stage 1 Allocate buffer for output values] */

        /* [Stage 1 Transform feedback object initialization] */
        /* Generate and bind transform feedback object. */
        glGenTransformFeedbacks(1, &spheres_updater_transform_feedback_object_id);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, spheres_updater_transform_feedback_object_id);

        /* Bind buffers to store calculated sphere positions. */
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, spheres_updater_sphere_positions_buffer_object_id);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        /* [Stage 1 Transform feedback object initialization] */

        /* 2. Scalar field generation stage. */
        /* Create scalar field generator program object. */
        scalar_field_program_id = glCreateProgram();

        /* Load and compile scalar field generator shaders. */
        scalar_field_vert_shader_id = CreateAndCompileShaderObjects(".\\src\\shaders\\metaball\\scalar_field_vert_shader.glsl", VERTEX);
        scalar_field_frag_shader_id = CreateAndCompileShaderObjects(".\\src\\shaders\\metaball\\scalar_field_frag_shader.glsl", FRAGMENT);

        /* Attach the shaders. */
        glAttachShader(scalar_field_program_id, scalar_field_vert_shader_id);
        glAttachShader(scalar_field_program_id, scalar_field_frag_shader_id);

        /* Specify shader varyings (output variables) we are interested in capturing. */
        glTransformFeedbackVaryings(scalar_field_program_id, 1, &scalar_field_value_varying_name, GL_SEPARATE_ATTRIBS);

        /* Link the program object. */
        glLinkProgram(scalar_field_program_id);

        /* Get input uniform locations. */
        scalar_field_uniform_samples_per_axis_id = glGetUniformLocation(scalar_field_program_id, scalar_field_uniform_samples_per_axis_name);
        scalar_field_uniform_spheres_id = glGetUniformBlockIndex(scalar_field_program_id, scalar_field_uniform_spheres_name);

        /* Activate scalar field generating program. */
        glUseProgram(scalar_field_program_id);

        /* Initialize uniforms constant throughout rendering loop. */
        glUniform1i(scalar_field_uniform_samples_per_axis_id, samples_per_axis);

        /* Set binding point for uniform block. */
        glUniformBlockBinding(scalar_field_program_id, scalar_field_uniform_spheres_id, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, spheres_updater_sphere_positions_buffer_object_id);

        /* Generate buffer object id. Define required storage space sufficient to hold scalar field data. */
        glGenBuffers(1, &scalar_field_buffer_object_id);
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, scalar_field_buffer_object_id);

        glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, samples_in_3d_space * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

        /* Generate and bind transform feedback object. */
        glGenTransformFeedbacks(1, &scalar_field_transform_feedback_object_id);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, scalar_field_transform_feedback_object_id);

        /* Bind buffer to store calculated scalar field values. */
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, scalar_field_buffer_object_id);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        /* [Stage 2 Creating texture] */
        /* Generate texture object to hold scalar field data. */
        glGenTextures(1, &scalar_field_texture_object_id);

        /* Scalar field uses GL_TEXTURE_3D target of texture unit 1. */
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_3D, scalar_field_texture_object_id);

        /* Prepare texture storage for scalar field values. */
        glTexStorage3D(GL_TEXTURE_3D, 1, GL_R32F, samples_per_axis, samples_per_axis, samples_per_axis);
        /* [Stage 2 Creating texture] */

        /* Tune texture settings to use it as a data source. */
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        /* 3. Marching Cubes cell-splitting stage. */
        /* Create a program object to execute Marching Cubes algorithm cell splitting stage. */
        marching_cubes_cells_program_id = glCreateProgram();

        /* Marching cubes algorithm shaders initialisation. */
        marching_cubes_cells_vert_shader_id = CreateAndCompileShaderObjects(".\\src\\shaders\\metaball\\marching_cubes_cells_vert_shader.glsl", VERTEX);
        marching_cubes_cells_frag_shader_id = CreateAndCompileShaderObjects(".\\src\\shaders\\metaball\\marching_cubes_cells_frag_shader.glsl", FRAGMENT);

        /* Attach the shaders. */
        glAttachShader(marching_cubes_cells_program_id, marching_cubes_cells_vert_shader_id);
        glAttachShader(marching_cubes_cells_program_id, marching_cubes_cells_frag_shader_id);

        /* Specify shader varyings (output variables) we are interested in capturing. */
        glTransformFeedbackVaryings(marching_cubes_cells_program_id, 1, &marching_cubes_cells_varying_name, GL_SEPARATE_ATTRIBS);

        /* Link the program object. */
        glLinkProgram(marching_cubes_cells_program_id);

        /* Get input uniform locations. */
        marching_cubes_cells_uniform_cells_per_axis_id = glGetUniformLocation(marching_cubes_cells_program_id, marching_cubes_cells_uniform_cells_per_axis_name);
        marching_cubes_cells_uniform_scalar_field_sampler_id = glGetUniformLocation(marching_cubes_cells_program_id, marching_cubes_cells_uniform_scalar_field_sampler_name);
        marching_cubes_cells_uniform_isolevel_id = glGetUniformLocation(marching_cubes_cells_program_id, marching_cubes_cells_uniform_isolevel_name);

        /* Activate cell-splitting program. */
        glUseProgram(marching_cubes_cells_program_id);

        /* Initialize uniforms constant throughout rendering loop. */
        glUniform1i(marching_cubes_cells_uniform_cells_per_axis_id, cells_per_axis);
        glUniform1f(marching_cubes_cells_uniform_isolevel_id, isosurface_level);
        glUniform1i(marching_cubes_cells_uniform_scalar_field_sampler_id, 1);

        /* Generate buffer object id and allocate memory to store scalar field values. */
        glGenBuffers(1, &marching_cubes_cells_types_buffer_id);
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, marching_cubes_cells_types_buffer_id);
        glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, cells_in_3d_space * sizeof(GLint), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

        /* Generate and bind transform feedback object. */
        glGenTransformFeedbacks(1, &marching_cubes_cells_transform_feedback_object_id);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, marching_cubes_cells_transform_feedback_object_id);

        /* Bind buffer to store calculated cell type data. */
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, marching_cubes_cells_types_buffer_id);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        /* [Stage 3 Creating texture] */
        /* Generate a texture object to hold cell type data. (We will explain why the texture later). */
        glGenTextures(1, &marching_cubes_cells_types_texture_object_id);

        /* Marching cubes cell type data uses GL_TEXTURE_3D target of texture unit 2. */
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_3D, marching_cubes_cells_types_texture_object_id);

        /* Prepare texture storage for marching cube cell type data. */
        glTexStorage3D(GL_TEXTURE_3D, 1, GL_R32I, cells_per_axis, cells_per_axis, cells_per_axis);
        /* [Stage 3 Creating texture] */

        /* Tune texture settings to use it as a data source. */
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        /* 4. Marching Cubes algorithm triangle generation and rendering stage. */
        /* Create a program object that we will use for triangle generation and rendering stage. */
        marching_cubes_triangles_program_id = glCreateProgram();

        /* Initialize shaders for the triangle generation and rendering stage. */
        marching_cubes_triangles_vert_shader_id = CreateAndCompileShaderObjects(".\\src\\shaders\\metaball\\marching_cubes_triangles_vert_shader.glsl", VERTEX);
        marching_cubes_triangles_frag_shader_id = CreateAndCompileShaderObjects(".\\src\\shaders\\metaball\\marching_cubes_triangles_frag_shader.glsl", FRAGMENT);

        /* Attach the shaders. */
        glAttachShader(marching_cubes_triangles_program_id, marching_cubes_triangles_vert_shader_id);
        glAttachShader(marching_cubes_triangles_program_id, marching_cubes_triangles_frag_shader_id);

        /* Link the program object. */
        glLinkProgram(marching_cubes_triangles_program_id);

        /* Get input uniform locations. */
        marching_cubes_triangles_uniform_time_id = glGetUniformLocation(marching_cubes_triangles_program_id, marching_cubes_triangles_uniform_time_name);
        marching_cubes_triangles_uniform_samples_per_axis_id = glGetUniformLocation(marching_cubes_triangles_program_id, marching_cubes_triangles_uniform_samples_per_axis_name);
        marching_cubes_triangles_uniform_isolevel_id = glGetUniformLocation(marching_cubes_triangles_program_id, marching_cubes_triangles_uniform_isolevel_name);
        marching_cubes_triangles_uniform_cell_types_sampler_id = glGetUniformLocation(marching_cubes_triangles_program_id, marching_cubes_triangles_uniform_cell_types_sampler_name);
        marching_cubes_triangles_uniform_tri_table_sampler_id = glGetUniformLocation(marching_cubes_triangles_program_id, marching_cubes_triangles_uniform_tri_table_sampler_name);
        marching_cubes_triangles_uniform_scalar_field_sampler_id = glGetUniformLocation(marching_cubes_triangles_program_id, marching_cubes_triangles_uniform_scalar_field_sampler_name);
        marching_cubes_triangles_uniform_sphere_positions_id = glGetUniformBlockIndex(marching_cubes_triangles_program_id, marching_cubes_triangles_uniform_sphere_positions_name);

        /* Activate triangle generating and rendering program. */
        glUseProgram(marching_cubes_triangles_program_id);

        /* Initialize model view projection matrix. */
        /* Initialize uniforms constant throughout rendering loop. */
        glUniform1f(marching_cubes_triangles_uniform_isolevel_id, isosurface_level);
        glUniform1i(marching_cubes_triangles_uniform_samples_per_axis_id, samples_per_axis);
        glUniform1i(marching_cubes_triangles_uniform_tri_table_sampler_id, 4);
        glUniform1i(marching_cubes_triangles_uniform_cell_types_sampler_id, 2);
        glUniform1i(marching_cubes_triangles_uniform_scalar_field_sampler_id, 1);

        /* Allocate memory for buffer */
        glBindBuffer(GL_UNIFORM_BUFFER, spheres_updater_sphere_positions_buffer_object_id);

        /* Generate an Id for a texture object to hold look-up array data (tri_table). */
        glGenTextures(1, &marching_cubes_triangles_lookup_table_texture_id);

        /* Lookup array (tri_table) uses GL_TEXTURE_2D target of texture unit 4. */
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, marching_cubes_triangles_lookup_table_texture_id);

        /* Tune texture settings to use it as a data source. */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        /* Load lookup table (tri_table) into texture. */
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32I, mc_vertices_per_cell, mc_cells_types_count);
        glTexSubImage2D(GL_TEXTURE_2D,        /* Use texture bound to GL_TEXTURE_2D               */
                        0,                    /* Base image level                                 */
                        0,                    /* From the texture origin                          */
                        0,                    /* From the texture origin                          */
                        mc_vertices_per_cell, /* Width will represent vertices in all 5 triangles */
                        mc_cells_types_count, /* Height will represent cell type                  */
                        GL_RED_INTEGER,       /* Texture will have only one component             */
                        GL_INT,               /* ... of type int                                  */
                        tri_table             /* Data will be copied directly from tri_table      */
        );

        /* Generate a vertex array object. We'll go with the explanation later. */
        glGenVertexArrays(1, &marching_cubes_triangles_vao_id);

        /* In OpenGL ES, draw calls require a bound vertex array object.
         * Even though we're not using any per-vertex attribute data, we still need to bind a vertex array object.
         */
        glBindVertexArray(marching_cubes_triangles_vao_id);

        /* Enable facet culling, depth testing and specify front face for polygons. */
        // glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        // glFrontFace(GL_CW);

        // Bloom
        return TRUE;
    }

    void update()
    {
    }

    float metaballTime = 0.0f;
    void display()
    {
        /* Update time. */
        model_time = -2.0 + metaballTime;

        /* Activate triangle generating and rendering program. */
        glUseProgram(marching_cubes_triangles_program_id);
        {
            /* Initialize uniforms constant throughout rendering loop. */
            glUniformMatrix4fv(glGetUniformLocation(marching_cubes_triangles_program_id, "modelMatrix"), 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(glGetUniformLocation(marching_cubes_triangles_program_id, "viewMatrix"), 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(glGetUniformLocation(marching_cubes_triangles_program_id, "projectionMatrix"), 1, GL_FALSE, perspectiveProjectionMatrix);
            glUniform1f(glGetUniformLocation(marching_cubes_triangles_program_id, "lightZ"), 10.0);
        }
        glUseProgram(0);
        /*
         * Rendering section
         */
        /* Clear the buffers that we are going to render to in a moment. */
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* [Stage 1 Calculate sphere positions stage] */
        /* 1. Calculate sphere positions stage.
         *
         * At this stage we calculate new sphere positions in space
         * according to current time moment.
         */
        /* [Stage 1 Bind buffers to store calculated sphere position values] */
        /* Bind buffers to store calculated sphere position values. */
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, spheres_updater_transform_feedback_object_id);
        /* [Stage 1 Bind buffers to store calculated sphere position values] */

        /* [Stage 1 Enable GL_RASTERIZER_DISCARD] */
        /* Shorten GL pipeline: we will use vertex shader only. */
        glEnable(GL_RASTERIZER_DISCARD);
        /* [Stage 1 Enable GL_RASTERIZER_DISCARD] */
        {
            /* Select program for sphere positions generation stage. */
            glUseProgram(spheres_updater_program_id);

            /* [Stage 1 Specify input arguments to vertex shader] */
            /* Specify input arguments to vertex shader. */
            glUniform1f(spheres_updater_uniform_time_id, model_time);
            glUniform1f(glGetUniformLocation(spheres_updater_program_id, "time_noise"), ELAPSED_TIME);
            /* [Stage 1 Specify input arguments to vertex shader] */

            /* [Stage 1 Activate transform feedback mode] */
            /* Activate transform feedback mode. */
            glBeginTransformFeedback(GL_POINTS);
            /* [Stage 1 Activate transform feedback mode] */
            {
                /* [Stage 1 Execute n_spheres times vertex shader] */
                /* Run sphere positions calculation. */
                glDrawArrays(GL_POINTS, 0, n_spheres);
                /* [Stage 1 Execute n_spheres times vertex shader] */
            }
            /* [Stage 1 Deactivate transform feedback mode] */
            glEndTransformFeedback();
            /* [Stage 1 Deactivate transform feedback mode] */
        }
        /* [Stage 1 Disable GL_RASTERIZER_DISCARD] */
        glDisable(GL_RASTERIZER_DISCARD);
        /* [Stage 1 Disable GL_RASTERIZER_DISCARD] */

        /* Unbind buffers used at this stage. */
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        /* [Stage 1 Calculate sphere positions stage] */

        /* [Stage 2 Scalar field generation stage] */
        /* 2. Scalar field generation stage.
         *
         * At this stage we calculate scalar field and store it in buffer
         * and later copy from buffer to texture.
         */
        /* Bind sphere positions data buffer to GL_UNIFORM_BUFFER. */
        glBindBuffer(GL_UNIFORM_BUFFER, spheres_updater_sphere_positions_buffer_object_id);

        /* Bind buffer object to store calculated scalar field values. */
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, scalar_field_transform_feedback_object_id);

        /* Shorten GL pipeline: we will use vertex shader only. */
        glEnable(GL_RASTERIZER_DISCARD);
        {
            /* Select program for scalar field generation stage. */
            glUseProgram(scalar_field_program_id);

            /* Activate transform feedback mode. */
            glBeginTransformFeedback(GL_POINTS);
            {
                /* Run scalar field calculation for all vertices in space. */
                glDrawArrays(GL_POINTS, 0, samples_in_3d_space);
            }
            glEndTransformFeedback();
        }
        glDisable(GL_RASTERIZER_DISCARD);

        /* Unbind buffers used at this stage. */
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        /* [Stage 2 Scalar field generation stage] */

        /* Copy scalar field values from buffer into texture bound to target GL_TEXTURE_3D of texture unit 1.
         * We need to move this data to a texture object, as there is no way we could access data
         * stored within a buffer object in an OpenGL ES 3.0 shader.
         */
        /* [Stage 2 Scalar field generation stage move data to texture] */
        glActiveTexture(GL_TEXTURE1);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, scalar_field_buffer_object_id);
        glTexSubImage3D(GL_TEXTURE_3D,    /* Use texture bound to GL_TEXTURE_3D                                     */
                        0,                /* Base image level                                                       */
                        0,                /* From the texture origin                                                */
                        0,                /* From the texture origin                                                */
                        0,                /* From the texture origin                                                */
                        samples_per_axis, /* Texture have same width as scalar field in buffer                      */
                        samples_per_axis, /* Texture have same height as scalar field in buffer                     */
                        samples_per_axis, /* Texture have same depth as scalar field in buffer                      */
                        GL_RED,           /* Scalar field gathered in buffer has only one component                 */
                        GL_FLOAT,         /* Scalar field gathered in buffer is of float type                       */
                        NULL              /* Scalar field gathered in buffer bound to GL_PIXEL_UNPACK_BUFFER target */
        );
        /* [Stage 2 Scalar field generation stage move data to texture] */

        /* 3. Marching cube algorithm cell splitting stage.
         *
         * At this stage we analyze isosurface in each cell of space and
         * assign one of 256 possible types to each cell. Cell type data
         * for each cell is stored in attached buffer.
         */
        /* Bind buffer to store cell type data. */
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, marching_cubes_cells_transform_feedback_object_id);

        /* Shorten GL pipeline: we will use vertex shader only. */
        glEnable(GL_RASTERIZER_DISCARD);
        {
            /* Select program for Marching Cubes algorthim's cell splitting stage. */
            glUseProgram(marching_cubes_cells_program_id);

            /* Activate transform feedback mode. */
            glBeginTransformFeedback(GL_POINTS);
            {
                /* [Stage 3 Execute vertex shader] */
                /* Run Marching Cubes algorithm cell splitting stage for all cells. */
                glDrawArrays(GL_POINTS, 0, cells_in_3d_space);
                /* [Stage 3 Execute vertex shader] */
            }
            glEndTransformFeedback();
        }
        glDisable(GL_RASTERIZER_DISCARD);

        /* Unbind buffers used at this stage. */
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        /* Copy data from buffer into texture bound to target GL_TEXTURE2 in texture unit 2.
         * We need to move this data to a texture object, as there is no way we could access data
         * stored within a buffer object in a OpenGL ES 3.0 shader.
         */
        glActiveTexture(GL_TEXTURE2);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, marching_cubes_cells_types_buffer_id);
        glTexSubImage3D(GL_TEXTURE_3D,  /* Use texture bound to GL_TEXTURE_3D                                   */
                        0,              /* Base image level                                                     */
                        0,              /* From the texture origin                                              */
                        0,              /* From the texture origin                                              */
                        0,              /* From the texture origin                                              */
                        cells_per_axis, /* Texture have same width as cells by width in buffer                  */
                        cells_per_axis, /* Texture have same height as cells by height in buffer                */
                        cells_per_axis, /* Texture have same depth as cells by depth in buffer                  */
                        GL_RED_INTEGER, /* Cell types gathered in buffer have only one component                */
                        GL_INT,         /* Cell types gathered in buffer are of int type                        */
                        NULL            /* Cell types gathered in buffer bound to GL_PIXEL_UNPACK_BUFFER target */
        );

        /* 4. Marching Cubes algorithm triangle generation stage.
         *
         * At this stage, we render exactly (3 vertices * 5 triangles per cell *
         * amount of cells the scalar field is split to) triangle vertices.
         * Then render triangularized geometry.
         */

        glActiveTexture(GL_TEXTURE0);

        /* Activate triangle generating and rendering program. */
        glUseProgram(marching_cubes_triangles_program_id);

        /* Specify input arguments to vertex shader. */
        glUniform1f(marching_cubes_triangles_uniform_time_id, model_time);

        /* [Stage 4 Run triangle generating and rendering program] */
        /* Run triangle generating and rendering program. */
        glDrawArrays(GL_TRIANGLES, 0, cells_in_3d_space * triangles_per_cell * vertices_per_triangle);
        /* [Stage 4 Run triangle generating and rendering program] */
    }

    void uninitialize(void)
    {
        glDeleteVertexArrays(1, &marching_cubes_triangles_vao_id);
        glDeleteShader(marching_cubes_triangles_frag_shader_id);
        glDeleteShader(marching_cubes_triangles_vert_shader_id);
        glDeleteProgram(marching_cubes_triangles_program_id);
        glDeleteTextures(1, &marching_cubes_triangles_lookup_table_texture_id);
        glDeleteTextures(1, &marching_cubes_cells_types_texture_object_id);
        glDeleteTransformFeedbacks(1, &marching_cubes_cells_transform_feedback_object_id);
        glDeleteBuffers(1, &marching_cubes_cells_types_buffer_id);
        glDeleteShader(marching_cubes_cells_frag_shader_id);
        glDeleteShader(marching_cubes_cells_vert_shader_id);
        glDeleteProgram(marching_cubes_cells_program_id);
        glDeleteTextures(1, &scalar_field_texture_object_id);
        glDeleteTransformFeedbacks(1, &scalar_field_transform_feedback_object_id);
        glDeleteBuffers(1, &scalar_field_buffer_object_id);
        glDeleteShader(scalar_field_frag_shader_id);
        glDeleteShader(scalar_field_vert_shader_id);
        glDeleteProgram(scalar_field_program_id);
        glDeleteTransformFeedbacks(1, &spheres_updater_transform_feedback_object_id);
        glDeleteBuffers(1, &spheres_updater_sphere_positions_buffer_object_id);
        glDeleteShader(spheres_updater_frag_shader_id);
        glDeleteShader(spheres_updater_vert_shader_id);
        glDeleteProgram(spheres_updater_program_id);
    }
};
