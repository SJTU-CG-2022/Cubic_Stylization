#include "fit_rotations_l1.h"

void fit_rotations_l1(
    const Eigen::MatrixXd & V,
    const Eigen::MatrixXi & F,
	Eigen::MatrixXd & U,
    Eigen::MatrixXd & RAll,
	cube_style_data & data)
{
    using namespace Eigen;
	using namespace std;
    using namespace igl;

    vector<vector<int>> verFace, one_time_info;
    vertex_triangle_adjacency(V.rows(), F, verFace, one_time_info);

    parallel_for(
        V.rows(),
        [&data, &RAll, &U, &verFace, &F, &V](const int i)
        {
            VectorXd z = data.zAll.col(i);
            VectorXd u = data.uAll.col(i);
            VectorXd n = data.N.row(i).transpose();
            double rho = data.rhoAll(i);

            MatrixXd Di = data.dVList[i];

            vector<int> FaceSet;
            FaceSet = verFace[i];
            MatrixXd Di_deformed(3, FaceSet.size() * 3);
            for (int j = 0; j < FaceSet.size(); j++) {
                int a = F(FaceSet[j], 0), b = F(FaceSet[j], 1), c = F(FaceSet[j], 2);

                Di_deformed.block(0, 3 * j    , 3, 1) = (U.row(b) - U.row(a)).transpose();
                Di_deformed.block(0, 3 * j + 1, 3, 1) = (U.row(c) - U.row(b)).transpose();
                Di_deformed.block(0, 3 * j + 2, 3, 1) = (U.row(a) - U.row(c)).transpose();
            }

            MatrixXd Wi = data.WVecList[i].asDiagonal();
            double sqrt_val = sqrt(3.0);
            Matrix3d Ri;

            // local ADMM
            // A = -n, B = I, c = 0
            for (int k=0; k<data.maxIter_ADMM; k++)
            {
                // Ri step
                MatrixXd left;
                left.resize(3, Di.cols() + 1);
                left.block(0, 0, 3, Di.cols()) = Di;
                left.col(Di.cols()) = n;

                MatrixXd mid;
                mid.resize(Di.cols() + 1, Di.cols() + 1);
                mid.setZero();
                mid.block(0, 0, Di.cols(), Di.cols()) = Wi;
                mid(Di.cols(), Di.cols()) = rho;

                MatrixXd right;
                right.resize(3, Di.cols() + 1);
                right.block(0, 0, 3, Di.cols()) = Di_deformed;
                right.col(Di.cols()) = z-u;
                Matrix3d S = left * mid * right.transpose();

                // Matrix3d S = Spre + (rho * n * (z-u).transpose());
                // S /= S.norm();
                // orthogonal procrustes
                JacobiSVD<Matrix3d> svd(S, Eigen::ComputeFullU | Eigen::ComputeFullV );
                Ri = svd.matrixV() * svd.matrixU().transpose();
                if (Ri.determinant() <= 0)
                {
                    Matrix3d svdU = svd.matrixU().transpose();
                    svdU.row(2) = svdU.row(2) * -1;
                    Ri = svd.matrixV() * svdU;
                }

                // z step
                // shrinkage step
                VectorXd zOld = z;
                VectorXd input_x = Ri * n + u;
                double param = data.lambda * data.VA(i)/rho;
                for (int j = 0; j < 3; ++j) {
                    if(input_x(j) > param)
                        z(j) = input_x(j) - param;
                    else if(input_x(j) < -param)
                        z(j) = input_x(j) + param;
                    else
                        z(j) = 0;
                }

                // u step
                // u = uOld + Ax + Bz - c
                u += Ri * n - z;

                // compute residual
                // s = rho * A' * B (z - zOld)
                // l1-norm of n equals to 1 by def
                double s_norm = (-rho * (z - zOld)).norm();

                // r = A * x + B * z - c
                // x means Ri here
                double r_norm = (-Ri * n + z).norm();

                // rho step
                if (r_norm > data.mu * s_norm)
                {
                    rho *= data.tao;
                    u /= data.tao;
                }
                else if (s_norm > data.mu * r_norm)
                {
                    rho /= data.tao;
                    u *= data.tao;
                }

                // stopping criteria
                // p and n both equal to 3
                // c is 0 and we do not consider it
                double eps_pri = sqrt_val * data.ABSTOL + data.RELTOL * max((Ri * n).norm(), z.norm());
                //u = (1/p)y
                double eps_dual = sqrt_val * data.ABSTOL + data.RELTOL * rho * u.maxCoeff() * n.norm();
                if (r_norm < eps_pri && s_norm < eps_dual)
                {
                    // save parameters
                    data.zAll.col(i) = z;
                    data.uAll.col(i) = u;
                    data.rhoAll(i) = rho;
                    RAll.block(0, 3 * i, 3, 3) = Ri;
                    break;
                }
            }
        }   
    ,1000);
}
