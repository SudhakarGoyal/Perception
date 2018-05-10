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
    pointcloud<pointxyz>::ptr cloud(new pointcloud<pointxyz>);
    pointcloud<pointxyz>::ptr cloud_new(new pointcloud<pointxyz>);

    if(pcl::io::loadpcdfile("path of the pcd file", *cloud) == -1)//mention the path of the pcd file you wish to load
    {
        cout<<"failed to open the pcd file"<<endl;
        return(-1);
    }

    vector<int> indices;
    removenanfrompointcloud(*cloud,*cloud,indices); //removing points which whose values do not represent a number

    for(int i =1; i < cloud->size() - 1;i++)
    {
        float diffx, diffy, diffz;
        diffx = -1*cloud->points[i+1].x +2*cloud->points[i].x - cloud->points[i-1].x ;
        diffy = cloud->points[i+1].y - 2*cloud->points[i-1].y + cloud->points[i].y ;
        diffz = -1*cloud->points[i+1].z+2*cloud->points[i].z - cloud->points[i-1].z ;

        float diff = diffx*diffx + diffy*diffy + diffz*diffz;

        if(sqrt(diff) > 0.3)
        {
            pointxyz pt;
            pt.x = cloud->points[i].x;
            pt.y = cloud->points[i].y;
            pt.z = cloud->points[i].z;
            cloud_new->push_back(pt);
        }
    }

    boost::shared_ptr< ::pcl::visualization::pclvisualizer > viewer (new ::pcl::visualization::pclvisualizer("3d viewer"));
    viewer->initcameraparameters();
    viewer->setbackgroundcolor(0, 0, 0);
    viewer->addcoordinatesystem(1.0);
    viewer->addpointcloud<pointxyz>(cloud_new, "cloud");
    viewer->setpointcloudrenderingproperties(pcl::visualization::pcl_visualizer_point_size, 1, "cloud");
    while(!viewer->wasstopped())
    {
        viewer->spinonce(10);

    }
    return 0;
}


