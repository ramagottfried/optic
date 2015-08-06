#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/features/fpfh_omp.h>
#include <pcl/features/normal_3d.h>



void
makeTestCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    //-----
    // Fill in the cloud data
    cloud->width    = 500;
    cloud->height   = 1;
    cloud->is_dense = false;
    cloud->points.resize (cloud->width * cloud->height);
    
    for (size_t i = 0; i < cloud->points.size (); ++i)
    {
        cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
        cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
        cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
    }
    
    pcl::io::savePCDFileASCII ("test_pcd.pcd", *cloud);
    std::cerr << "Saved " << cloud->points.size () << " data points to test_pcd.pcd." << std::endl;
    
    for (size_t i = 0; i < cloud->points.size (); ++i)
        std::cerr << "    " << cloud->points[i].x << " " << cloud->points[i].y << " " << cloud->points[i].z << std::endl;
    
    //----
}


int
main (int argc, char** argv)
{
//    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    
    makeTestCloud(cloud);
    
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    ne.setInputCloud (cloud);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
    ne.setSearchMethod (tree);
    
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);
    
    // Use all neighbors in a sphere of radius 3cm
    ne.setRadiusSearch (0.03);
    
    // Compute the features
    ne.compute (*cloud_normals);
    
    for (int i = 0; i < cloud_normals->points.size(); i++)
    {
        if (!pcl::isFinite<pcl::Normal>(cloud_normals->points[i]))
        {
            PCL_WARN("normals[%d] is not finite\n", i);
        }
    }
    
    // Create the FPFH estimation class, and pass the input dataset+normals to it
    pcl::FPFHEstimationOMP<pcl::PointXYZ, pcl::Normal, pcl::FPFHSignature33> fpfh;
    fpfh.setInputCloud (cloud);
    fpfh.setInputNormals (cloud_normals);
    // alternatively, if cloud is of tpe PointNormal, do fpfh.setInputNormals (cloud);
    
    // Create an empty kdtree representation, and pass it to the FPFH estimation object.
    // Its content will be filled inside the object, based on the given input dataset (as no other search surface is given).
    pcl::search::KdTree<pcl::PointXYZ>::Ptr fpfh_tree (new pcl::search::KdTree<pcl::PointXYZ>);
    
    fpfh.setSearchMethod (fpfh_tree);
    
    // Output datasets
    pcl::PointCloud<pcl::FPFHSignature33>::Ptr fpfhs (new pcl::PointCloud<pcl::FPFHSignature33> ());
    
    // Use all neighbors in a sphere of radius 5cm
    // IMPORTANT: the radius used here has to be larger than the radius used to estimate the surface normals!!!
    fpfh.setRadiusSearch (0.05);
    
    // Compute the features
    fpfh.compute (*fpfhs);
    
    
    return (0);
}