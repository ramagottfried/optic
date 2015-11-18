#include <stdio.h>

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/tracking.hpp"

#include "osc.h"
#include "osc_bundle_u.h"
#include "osc_bundle_s.h"
#include "osc_timetag.h"

#include "sys/socket.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sys/types.h"
#include "unistd.h"

#define UDP_PORT 4444

using namespace cv;
using namespace std;

RNG rng(12345);

//globals which will probably end up in a class
int sigma = 3;
int ksize = (sigma*5)|1;
Mat prev_frame;
vector<Point> prev_pts;
Size dst_size;

unsigned long countfails = 0;
struct sockaddr_in sendto_addr;
int fd;

template <typename T>
vector<size_t> sort_indexes( const vector<T> &v )
{
    vector<size_t>  idx(v.size());
    for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;
    
    sort(idx.begin(), idx.end(),
         [ &v ]( size_t i1, size_t i2 ){ return v[i1] < v[i2]; } );
    
    return idx;
}

void send_osc(t_osc_bndl_u *bndl)
{
    if(bndl)
    {
        t_osc_bndl_s *s_bndl = osc_bundle_u_serialize(bndl);
        if(s_bndl)
        {
            if( sendto(fd, osc_bundle_s_getPtr(s_bndl), osc_bundle_s_getLen(s_bndl), 0, (struct sockaddr *)&sendto_addr, sizeof(sendto_addr)) < 0)
            {
                printf("failed to send %ld\n", countfails++);
            }
            
            osc_bundle_s_deepFree(s_bndl);
        }
    }
}


static void calc_draw_send(const Mat src_gray, Mat& outframe, bool draw=true)
{

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat threshold_output;
    double thresh = 100;
    
    /// Find contours
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
    findContours( threshold_output, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    /// sizes based on contours
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<vector<Point> >hullP( contours.size() );
    vector<vector<int> >hullI( contours.size() );
    vector<vector<Vec4i> >defects( contours.size() );
    vector<double>focus_val( contours.size() );
    vector<double>contour_area( contours.size() );
    //flat vector for optical flow
    vector<Point> defect_startpt;

    
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    
    t_osc_bndl_u *bndl = osc_bundle_u_alloc();
    t_osc_msg_u *cx = osc_message_u_allocWithAddress((char *)"/center/x");
    t_osc_msg_u *cy = osc_message_u_allocWithAddress((char *)"/center/y");
    t_osc_msg_u *sx = osc_message_u_allocWithAddress((char *)"/size/x");
    t_osc_msg_u *sy = osc_message_u_allocWithAddress((char *)"/size/y");
    t_osc_msg_u *angle = osc_message_u_allocWithAddress((char *)"/angle");
    t_osc_msg_u *area = osc_message_u_allocWithAddress((char *)"/area");
    t_osc_msg_u *area_sort = osc_message_u_allocWithAddress((char *)"/area_sort");
    t_osc_msg_u *convex = osc_message_u_allocWithAddress((char *)"/convex");
    t_osc_msg_u *child_of = osc_message_u_allocWithAddress((char *)"/child_of");
    t_osc_msg_u *focus = osc_message_u_allocWithAddress((char *)"/focus");
    t_osc_msg_u *srcdim = osc_message_u_allocWithAddress((char *)"/dim_xy");
    t_osc_msg_u *defect_count = osc_message_u_allocWithAddress((char *)"/defect/count");
    t_osc_msg_u *defect_dist_sum = osc_message_u_allocWithAddress((char *)"/defect/dist_sum");

    
    osc_message_u_appendInt32(srcdim, src_gray.size().width);
    osc_message_u_appendInt32(srcdim, src_gray.size().height);
    
    long npix = src_gray.size().width * src_gray.size().height;
    
    for( int i = 0; i < contours.size(); i++ )
    {
        boundRect[i] = boundingRect( Mat(contours[i]) );
        minRect[i] = minAreaRect( Mat(contours[i]) );
        
        convexHull( Mat(contours[i]), hullP[i], false );
        convexHull( Mat(contours[i]), hullI[i], false );
        convexityDefects( contours[i], hullI[i], defects[i] );

        Mat rot_mtx = getRotationMatrix2D(minRect[i].center, minRect[i].angle, 1.0);
        Mat rot;
        Mat roi;
        warpAffine( src_gray, rot, rot_mtx, src_gray.size(), INTER_AREA );
        getRectSubPix(rot, minRect[i].size, minRect[i].center, roi);

//        Mat roi = src_gray( boundRect[i] ); //<< slightly faster backup, maybe better result even?
        
        focus_val[i] = 0.0;
        if( minRect[i].size.width > 15  )
        {
            Mat lap;
            Laplacian(roi, lap, CV_16S, 5);
            Scalar avg = mean(lap);
            focus_val[i] = avg[0];
        }
        
        if( contours[i].size() > 5 )
        {
            minEllipse[i] = fitEllipse( Mat(contours[i]) );
        }
        
        //filter by focus
//        if( focus_val[i] > 1) continue;
        
        contour_area[i] = contourArea(Mat(contours[i]));
        
        
        osc_message_u_appendFloat(cx, minRect[i].center.x / src_gray.size().width);
        osc_message_u_appendFloat(cy, minRect[i].center.y / src_gray.size().width);
        osc_message_u_appendFloat(sx, minRect[i].size.width / src_gray.size().width);
        osc_message_u_appendFloat(sy, minRect[i].size.height / src_gray.size().width);
        osc_message_u_appendFloat(angle, minRect[i].angle);
        osc_message_u_appendFloat(area, contour_area[i] / npix);
        osc_message_u_appendInt32(convex, isContourConvex(Mat(contours[i])));
        osc_message_u_appendInt32(child_of, hierarchy[i][3]);
        osc_message_u_appendDouble(focus, focus_val[i]);
        osc_message_u_appendInt64(defect_count, defects[i].size());


        Scalar color = Scalar( rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) );
        if( draw )
        {
            // contour
            drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            
            // ellipse
            ellipse( drawing, minEllipse[i], color, focus_val[i], 8 );
            
            // rotated rectangle
            Point2f rect_points[4];
            minRect[i].points( rect_points );
            for( int j = 0; j < 4; j++ )
                line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
        }
        
        

        // convex hull defects
       // if( contours[i].size() < 300 ) continue; //skip if contour is small
  
        // might want to collect data on all the set of defects and only send general information rather than overloading ... but maybe the points are interesting too...
        
//        Mat dist_stats = Mat::zeros((int)defects[i].size(), 1, CV_64FC1);
        
        /*
         
         - number of extruding points
         
         - size of concavity... right now this is far-start distance, but maybe should be distance from convex hull line in middle
            -- this might also want to be a min/max/mean situation?  or maybe just send the points and deal with it in max?
         
         
         */
        
        double dist_sum = 0;
        vector<double> defect_dist;
        vector<Vec4i>::iterator d = defects[i].begin();
        while ( d != defects[i].end() )
        {
            Vec4i& v = (*d);
            Point ptStart(  contours[ i ][ v[0] ] );
            Point ptEnd(    contours[ i ][ v[1] ] );
            Point ptFar(    contours[ i ][ v[2] ] );
            //float depth = v[3] / 256.; // depth from center of contour
            double dist = norm(ptFar - ptStart);
            dist_sum += dist;
            defect_startpt.push_back( ptStart );
            defect_dist.push_back( dist / npix);
            
            if( draw && dist > 0 )
            {
                line( drawing, ptStart, ptFar, color, 1 );
                line( drawing, ptFar, ptEnd, color, 1 );
                int thick = (dist <= 255 ? dist : 255) / 10;
              //  printf("dist %f\n", dist);
                circle( drawing, ptStart, 4, color, thick );
            }
            d++;
        }
        
        osc_message_u_appendDouble(defect_dist_sum, dist_sum);
        
    }
    
    
    
    /*
    Mat stat, err;
    if( prev_pts.size() > 0)
        calcOpticalFlowPyrLK(prev_frame, threshold_output, prev_pts, defect_startpt, stat, err);
    prev_pts = defect_startpt;
    prev_frame = threshold_output.clone();
    */
    t_osc_msg_u *tmsg = osc_message_u_allocWithAddress((char *)"/timetag");
    osc_message_u_appendTimetag(tmsg, osc_timetag_now());

    vector<size_t> sorted = sort_indexes( contour_area );
    for_each(sorted.begin(), sorted.end(),
             [&area_sort](int ii){ osc_message_u_appendInt32(area_sort, ii); });
    
    
    osc_bundle_u_addMsg(bndl, cx);
    osc_bundle_u_addMsg(bndl, cy);
    osc_bundle_u_addMsg(bndl, sx);
    osc_bundle_u_addMsg(bndl, sy);
    osc_bundle_u_addMsg(bndl, angle);
    osc_bundle_u_addMsg(bndl, area);
    osc_bundle_u_addMsg(bndl, convex);
    osc_bundle_u_addMsg(bndl, child_of);
    osc_bundle_u_addMsg(bndl, focus);
    osc_bundle_u_addMsg(bndl, tmsg);
    osc_bundle_u_addMsg(bndl, area_sort);
    osc_bundle_u_addMsg(bndl, srcdim);
    osc_bundle_u_addMsg(bndl, defect_count);
    osc_bundle_u_addMsg(bndl, defect_dist_sum);
    
    send_osc(bndl);
    osc_bundle_u_free(bndl);

    add(outframe, drawing, outframe);
}

static void get_convexHull(const Mat src_gray, Mat& outframe)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat threshold_output;
    
    double thresh = 100;
    
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
    
    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    /// Find the convex hull object for each contour
    vector<vector<Point> >hullP( contours.size() );
    vector<vector<int> >hullI( contours.size() );
    vector<vector<Vec4i> >defects( contours.size() );
    
    for( int i = 0; i < contours.size(); i++ )
    {
        convexHull( Mat(contours[i]), hullP[i], false );
        convexHull( Mat(contours[i]), hullI[i], false );
        convexityDefects( contours[i], hullI[i], defects[i] );
        
    }
    
    /// Draw contours + hull results
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        
        if( contours[i].size() < 500 )
        {
            // drawContours( drawing, hullP, i, color, 3, 8, vector<Vec4i>(), 0, Point() );
            drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        }
        
        if( contours[i].size() < 300 ) continue;
        
        defects[i].size();
        vector<Vec4i>::iterator d = defects[i].begin();
        while ( d != defects[i].end() )
        {
            Vec4i& v = (*d);
            Point ptStart(  contours[ i ][ v[0] ] );
            Point ptEnd(    contours[ i ][ v[1] ] );
            Point ptFar(    contours[ i ][ v[2] ] );
            float depth = v[3] / 256.;
            
            if( depth > 20 && depth < 100 )
            {
                line( drawing, ptStart, ptFar, color, 1 );
                line( drawing, ptFar, ptEnd, color, 1 );
                circle( drawing, ptStart, 4, color, 5 );
            }
            d++;
        }
        
    }
    
    add(outframe, drawing, outframe);
}

Mat get_polygonsCircles(InputArray src_gray)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat threshold_output;
    
    double thresh = 100;
    
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
    
    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
    
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }
    
    
    /// Draw polygonal contour + bonding rects + circles
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
        circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
    }
    
    return drawing;
}


int createSocketUDP()
{
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        printf("failed to create socket\n");
        return 0;
    }
    
    memset((char *)&sendto_addr, 0, sizeof(sendto_addr));
    sendto_addr.sin_family = AF_INET;
    sendto_addr.sin_port = htons(UDP_PORT);
    sendto_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    return 1;
}

void static camera_grab_resize(VideoCapture cap, Mat& resize_frame)
{
    Mat frame;
    cap >> frame; // get a new frame from camera
    resize(frame, resize_frame, Size(), 0.5, 0.5, INTER_AREA);
    if(!dst_size.width)
    {
        dst_size = resize_frame.size();
    }
//    printf("%d %d\n", dst_size.width, dst_size.height);

/*
    if( prev_frame.size().width == 0 )
        prev_frame = resize_frame;
  */
    frame.release(); //<< not sure if this is necessary, but there is a slow leak n the opencv video input
    
}

int main(int, char**)
{
    printf("check3\n");
    
    if(!createSocketUDP())
        return 0;
    
    VideoCapture cap(1); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    namedWindow("edges", WINDOW_NORMAL);
//    namedWindow("display", 1);

    Mat prev;

    int erosion_size = 0;
    int diation_size = 0;
    
    Mat er_element = getStructuringElement( MORPH_RECT,
                                           Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                           Point( erosion_size, erosion_size ) );
    Mat di_element = getStructuringElement( MORPH_RECT,
                                           Size( 2*diation_size + 1, 2*diation_size+1 ),
                                           Point( diation_size, diation_size ) );
    Mat firstframe;
    cap >> firstframe;
    
    for(;;)
    {
        Mat src_color_sized, src_gray;
        camera_grab_resize(cap, src_color_sized);
        flip(src_color_sized, src_color_sized, 1);
        /*
         //filter repetions
        if(prev.empty())
            prev = resize_frame;
        else if(countNonZero(resize_frame != prev) == 0)
        {
            //printf("repeated frame\n");
            prev = resize_frame;
            if(waitKey(1) >= 0) break;
            continue;
        }
        else
            prev = resize_frame;
        
        */
        
        cvtColor(src_color_sized, src_gray, CV_BGR2GRAY);
        
        //blur(src_gray, src_gray, Size(3,3));
        Mat dst(src_color_sized);

        GaussianBlur(src_gray, src_gray, Size(ksize, ksize), sigma, sigma);
        
        /// Apply the erosion operation
     //   dilate( src_gray, src_gray, di_element );
        erode( src_gray, src_gray, er_element );
        
        calc_draw_send(src_gray, dst);
        
        imshow("edges", dst);

//        imshow("display", dst);
        
        if(waitKey(1) >= 0) break;
    }
    
    close(fd);
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}


