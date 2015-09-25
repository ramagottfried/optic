#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/point_types_conversion.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/filter.h>

//#include <pcl/surface/convex_hull.h>

#include "jit.common.h"

int
  main (int argc, char** argv)
{
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

  // Fill in the cloud data
  cloud->width    = 5;
  cloud->height   = 1;
  cloud->is_dense = false;
  cloud->points.resize (cloud->width * cloud->height);

  for (size_t i = 0; i < cloud->points.size (); ++i)
  {
    cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
  }

  std::vector<int> temp;
  pcl::removeNaNFromPointCloud(*cloud, *cloud, temp);
/*
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_voxel (new pcl::PointCloud<pcl::PointXYZ>);
 // pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_hull_ (new pcl::PointCloud<pcl::PointXYZ>);
 // std::vector<pcl::Vertices> vertices_;

  pcl::VoxelGrid<pcl::PointXYZ> grid;
  grid.setLeafSize (0.01f, 0.01f, 0.01f);
  grid.setInputCloud (cloud);
  grid.filter (*cloud_voxel);
*/
/*
  // Estimate 3D convex hull
//            pcl::ConcaveHull<pcl::PointXYZ> hr;
  pcl::ConvexHull<pcl::PointXYZ> hr;
  hr.setDimension(3);
//            hr.setAlpha (0.1);
  hr.setInputCloud (cloud_pass_);
//            cloud_hull_.reset (new Cloud); << save on reallocating memory later?
  hr.reconstruct (*cloud_hull_, vertices_);
*/
  // post("nverts %d", vertices_.size());
  // todo iterate vertices to view.

  for (size_t i = 0; i < cloud->points.size (); ++i)
    std::cerr << "    " << cloud->points[i].x << " " << cloud->points[i].y << " " << cloud->points[i].z << std::endl;

  return (0);
}
