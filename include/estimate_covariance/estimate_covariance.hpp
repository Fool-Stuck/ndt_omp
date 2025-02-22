#ifndef NDT_OMP__ESTIMATE_COVARIANCE_HPP_
#define NDT_OMP__ESTIMATE_COVARIANCE_HPP_

#include "multigrid_pclomp/multigrid_ndt_omp.h"

#include <Eigen/Core>

namespace pclomp
{

struct ResultOfMultiNdtCovarianceEstimation
{
  Eigen::Vector2d mean;
  Eigen::Matrix2d covariance;
  std::vector<Eigen::Matrix4f> ndt_initial_poses;
  std::vector<NdtResult> ndt_results;
};

/** \brief Estimate functions */
Eigen::Matrix2d estimate_xy_covariance_by_Laplace_approximation(
  const Eigen::Matrix<double, 6, 6> & hessian);
ResultOfMultiNdtCovarianceEstimation estimate_xy_covariance_by_multi_ndt(
  const NdtResult & ndt_result,
  std::shared_ptr<pclomp::MultiGridNormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>>
    ndt_ptr,
  const std::vector<Eigen::Matrix4f> & poses_to_search);
ResultOfMultiNdtCovarianceEstimation estimate_xy_covariance_by_multi_ndt_score(
  const NdtResult & ndt_result,
  std::shared_ptr<pclomp::MultiGridNormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ>>
    ndt_ptr,
  const std::vector<Eigen::Matrix4f> & poses_to_search, const double temperature);

/** \brief Find rotation matrix aligning covariance to principal axes
 * (1) Compute eigenvalues and eigenvectors
 * (2) Compute angle for first eigenvector
 * (3) Return rotation matrix
 */
Eigen::Matrix2d find_rotation_matrix_aligning_covariance_to_principal_axes(
  const Eigen::Matrix2d & matrix);

/** \brief Propose poses to search.
 * (1) Compute covariance by Laplace approximation
 * (2) Find rotation matrix aligning covariance to principal axes
 * (3) Propose search points by adding offset_x and offset_y to the center_pose
 */
std::vector<Eigen::Matrix4f> propose_poses_to_search(
  const NdtResult & ndt_result, const std::vector<double> & offset_x,
  const std::vector<double> & offset_y);

/** \brief Calculate weights by exponential */
std::vector<double> calc_weight_vec(const std::vector<double> & score_vec, double temperature);

/** \brief Calculate weighted mean and covariance */
std::pair<Eigen::Vector2d, Eigen::Matrix2d> calculate_weighted_mean_and_cov(
  const std::vector<Eigen::Vector2d> & pose_2d_vec, const std::vector<double> & weight_vec);

/** \brief Rotate covariance to base_link coordinate */
Eigen::Matrix2d rotate_covariance_to_base_link(
  const Eigen::Matrix2d & covariance, const Eigen::Matrix4f & pose);

/** \brief Rotate covariance to map coordinate */
Eigen::Matrix2d rotate_covariance_to_map(
  const Eigen::Matrix2d & covariance, const Eigen::Matrix4f & pose);

/** \brief  Adjust diagonal covariance */
Eigen::Matrix2d adjust_diagonal_covariance(
  const Eigen::Matrix2d & covariance, const Eigen::Matrix4f & pose, const double fixed_cov00,
  const double fixed_cov11);

}  // namespace pclomp

#endif  // NDT_OMP__ESTIMATE_COVARIANCE_HPP_
