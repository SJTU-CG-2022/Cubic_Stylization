#ifndef CUBE_STYLE_DATA_H
#define CUBE_STYLE_DATA_H

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <limits>
#include <igl/min_quad_with_fixed.h>

struct cube_style_data
{
	// user should tune these
	double lambda = 0.0;
	double rhoInit = 1e-3;
	double ABSTOL = 1e-6;
	double RELTOL = 1e-3;

	// usually these don't need to tune
	double mu = 10;
	double tao = 2;
	double maxIter_ADMM = 1000;
	double reldV = std::numeric_limits<float>::max();

	std::vector<Eigen::MatrixXd> dVList, UHis;
	std::vector<Eigen::VectorXd> WVecList;

	Eigen::SparseMatrix<double> K, L;
	Eigen::MatrixXd N, VA, zAll, uAll;
	Eigen::VectorXd rhoAll;

	// bool useBc = false;
	Eigen::MatrixXd bc;
	Eigen::VectorXi b;

	igl::min_quad_with_fixed_data<double> solver_data;

	// for plane constraints
	Eigen::VectorXi bx, by, bz;
	igl::min_quad_with_fixed_data<double> solver_data_x, solver_data_y, solver_data_z;
	double xPlane = 0.0;
	double yPlane = 0.0;
	double zPlane = 0.0;
	// for plane constraints

	void reset()
	{
		// user should tune these
		ABSTOL = 1e-5;
		rhoInit = 1e-3;
		RELTOL = 1e-3;

		// usually these don't need to tune
		mu = 10;
		tao = 2;
		maxIter_ADMM = 1000;
		reldV = std::numeric_limits<float>::max();

		dVList.clear();
		UHis.clear();
		WVecList.clear();
		K = Eigen::SparseMatrix<double>();
		L = Eigen::SparseMatrix<double>();
		N = Eigen::MatrixXd();
		VA = Eigen::MatrixXd();
		zAll =Eigen::MatrixXd();
		uAll = Eigen::MatrixXd();
		rhoAll = Eigen::VectorXd();

		igl::min_quad_with_fixed_data<double> solver_data;
	}
};

#endif