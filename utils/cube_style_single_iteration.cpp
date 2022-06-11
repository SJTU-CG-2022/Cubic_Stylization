#include "cube_style_single_iteration.h"

void cube_style_single_iteration(
	const Eigen::MatrixXd & V,
    const Eigen::MatrixXi & F,
    Eigen::MatrixXd & U,
    cube_style_data & data)
{
    using namespace Eigen;
    using namespace std;

    // local step
    MatrixXd RAll(3,V.rows() * 3);
    {
        fit_rotations_l1(V, F, U, RAll, data);
    }

    // global step
    MatrixXd Upre = U;
    {
        VectorXd Rcol;
        igl::columnize(RAll, V.rows(), 2, Rcol);
        VectorXd Bcol = data.K * Rcol;
        for(int i = 0; i < 3; i++)
        {
            VectorXd Uc, Bc, bcc;
            Bc = Bcol.block(i * V.rows(), 0, V.rows(), 1);
            bcc = data.bc.col(i);
            min_quad_with_fixed_solve(data.solver_data, Bc, bcc, VectorXd(), Uc);
            U.col(i) = Uc;
        }
    }

    // print optimization date
    data.reldV = (U-Upre).cwiseAbs().maxCoeff() / (U-V).cwiseAbs().maxCoeff();
    // cout << "reldV:" << scientific << reldV << ", obj:" << data.objVal << endl;
}