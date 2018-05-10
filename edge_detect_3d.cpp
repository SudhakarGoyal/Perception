#include <pcl/io/pcd_io.h>
#include <pcl/common/io.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <math.h>
#include <vector>
#include <pcl/filters/extract_indices.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/common/common.h>
using namespace std;
using namespace pcl;
using namespace pcl::io;



int main ()
{
    PointCloud<PointXYZ>::Ptr cloud(new PointCloud<PointXYZ>);
    PointCloud<PointXYZ>::Ptr cloud_new(new PointCloud<PointXYZ>);


    if(pcl::io::loadPCDFile("path of the pcd file", *cloud) == -1)//mention the path of the pcd file you wish to load
    {
        cout<<"failed to open the pcd file"<<endl;
        return(-1);
    }

    vector<int> indices;
    removeNaNFromPointCloud(*cloud,*cloud,indices); //removing points which whose values do not represent a number

    for(int i =1; i < cloud->size() - 1;i++)
    {
        float diffX, diffY, diffZ;


        diffX = -1*cloud->points[i+1].x +2*cloud->points[i].x - cloud->points[i-1].x ;
        diffY = cloud->points[i+1].y - 2*cloud->points[i-1].y + cloud->points[i].y ;
        diffZ = -1*cloud->points[i+1].z+2*cloud->points[i].z - cloud->points[i-1].z ;

        float diff = diffX*diffX + diffY*diffY + diffZ*diffZ;

        if(sqrt(diff) > 0.3)
        {
            PointXYZ pt;
            pt.x = cloud->points[i].x;
            pt.y = cloud->points[i].y;
            pt.z = cloud->points[i].z;
            cloud_new->push_back(pt);
        }
    }

    boost::shared_ptr< ::pcl::visualization::PCLVisualizer > viewer (new ::pcl::visualization::PCLVisualizer("3D Viewer"));
    viewer->initCameraParameters();
    viewer->setBackgroundColor(0, 0, 0);
    viewer->addCoordinateSystem(1.0);
    viewer->addPointCloud<PointXYZ>(cloud_new, "cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud");
    while(!viewer->wasStopped())
    {
        viewer->spinOnce(10);

    }
    return 0;
}


