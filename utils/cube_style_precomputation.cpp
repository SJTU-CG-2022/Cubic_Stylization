#include "cube_style_precomputation.h"

void cube_style_precomputation(
	const Eigen::MatrixXd & V,
    const Eigen::MatrixXi & F,
    cube_style_data & data)
{
    using namespace Eigen;
	using namespace std;

    data.reset();

    //V F n
    igl::per_vertex_normals(V, F, data.N);
    igl::cotmatrix(V, F, data.L);

    //ai
    SparseMatrix<double> M;
    igl::massmatrix(V, F, igl::MASSMATRIX_TYPE_BARYCENTRIC,M);
    data.VA = M.diagonal();

    //z vector
    data.zAll.resize(3, V.rows());
    data.zAll.setRandom();

    //u vector
    data.uAll.resize(3, V.rows());
    data.uAll.setRandom();

    //rho vector
    data.rhoAll.resize(V.rows());
    data.rhoAll.setConstant(data.rhoInit);

    //get adjacent face for each vertex
    vector<vector<int>> verFace, one_time_info;
    igl::vertex_triangle_adjacency(V.rows(), F, verFace, one_time_info);

    data.WVecList.resize(V.rows());
    data.dVList.resize(V.rows());
    vector<int> FaceSet;

    for (int i = 0; i < V.rows(); i++)
    {
        FaceSet = verFace[i];
        data.WVecList[i].resize(FaceSet.size() * 3);
        data.dVList[i].resize(3, FaceSet.size() * 3);

        for (int j = 0; j < FaceSet.size(); j++) {
            int a = F(FaceSet[j], 0), b = F(FaceSet[j], 1), c = F(FaceSet[j], 2);

            data.dVList[i].block(0, 3 * j    , 3, 1) = (V.row(b) - V.row(a)).transpose();
            data.dVList[i].block(0, 3 * j + 1, 3, 1) = (V.row(c) - V.row(b)).transpose();
            data.dVList[i].block(0, 3 * j + 2, 3, 1) = (V.row(a) - V.row(c)).transpose();

            data.WVecList[i](3 * j    ) = data.L.coeff(a,b);
            data.WVecList[i](3 * j + 1) = data.L.coeff(b,c);
            data.WVecList[i](3 * j + 2) = data.L.coeff(c,a);
        }
    }

    igl::arap_rhs(V, F, V.cols(), igl::ARAP_ENERGY_TYPE_SPOKES_AND_RIMS, data.K);
    igl::min_quad_with_fixed_precompute(data.L, data.b, SparseMatrix<double>(), false, data.solver_data);
}