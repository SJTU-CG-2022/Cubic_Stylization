#ifndef FIT_ROTATIONS_L1_H
#define FIT_ROTATIONS_L1_H

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <cube_style_data.h>
#include <igl/vertex_triangle_adjacency.h>
#include <igl/vertex_triangle_adjacency.h>
#include <igl/slice.h>
#include <igl/parallel_for.h>
#include <math.h> 

void fit_rotations_l1(
	const Eigen::MatrixXd & V,
    const Eigen::MatrixXi & F,
	Eigen::MatrixXd & U,
	Eigen::MatrixXd & RAll,
	cube_style_data & data);
#endif