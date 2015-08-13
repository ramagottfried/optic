#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/conversions.h>


#include "libfreenect.h"
#include "libfreenect_sync.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "jit.common.h"



// Our Jitter object instance data
typedef struct _jit_freenect_pcl {
	t_object	ob;
	double		gain;	// our attribute (multiplied against each cell in the matrix)
} t_jit_freenect_pcl;


// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err           jit_freenect_pcl_init				(void);
t_jit_freenect_pcl  *jit_freenect_pcl_new				(void);
void                jit_freenect_pcl_free				(t_jit_freenect_pcl *x);
t_jit_err           jit_freenect_pcl_matrix_calc		(t_jit_freenect_pcl *x, void *inputs, void *outputs);
void                jit_freenect_pcl_calculate_ndim	(t_jit_freenect_pcl *x, long dim, long *dimsize, long planecount, t_jit_matrix_info *in_minfo,char *bip, t_jit_matrix_info *out_minfo, char *bop);
END_USING_C_LINKAGE

pcl::PointCloud<pcl::PointNormal>::Ptr jit_freenect_pcl_normals( pcl::PointCloud<pcl::PointXYZ>::Ptr cloud );
pcl::PointCloud<pcl::PointXYZ>::Ptr jit_freenect_pcl_genTriangles( pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals );


// globals
static void *s_jit_freenect_pcl_class = NULL;


/************************************************************************************/

t_jit_err jit_freenect_pcl_init(void) 
{
	long			attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
	t_jit_object	*attr;
	t_jit_object	*mop;
    t_atom a[1];

	s_jit_freenect_pcl_class = jit_class_new("jit_freenect_pcl", (method)jit_freenect_pcl_new, (method)jit_freenect_pcl_free, sizeof(t_jit_freenect_pcl), 0);

	// add matrix operator (mop)
    // args are  num inputs and num outputs
	mop = (t_jit_object *)jit_object_new(_jit_sym_jit_mop, 1, 1);
    jit_mop_input_nolink(mop,1);


    attr = (t_jit_object *)jit_object_method(mop, _jit_sym_getinput, 1);
    jit_attr_setlong(attr,_jit_sym_dimlink,0);
    jit_attr_setlong(attr,_jit_sym_typelink,0);
    jit_attr_setlong(attr,_jit_sym_planelink,0);
    jit_object_method(attr,_jit_sym_ioproc,jit_mop_ioproc_copy_adapt);
    jit_atom_setsym(a, _jit_sym_float32);
    jit_object_method(attr, _jit_sym_types,1, a);
    
    attr = (t_jit_object *)jit_object_method(mop, _jit_sym_getoutput, 1);
    jit_attr_setlong(attr, _jit_sym_dimlink,0);
    
	jit_class_addadornment(s_jit_freenect_pcl_class, mop);
    
    
    
	// add method(s)
	jit_class_addmethod(s_jit_freenect_pcl_class, (method)jit_freenect_pcl_matrix_calc, "matrix_calc", A_CANT, 0);

	// add attribute(s)
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
										 "gain", 
										 _jit_sym_float64, 
										 attrflags, 
										 (method)NULL, (method)NULL, 
										 calcoffset(t_jit_freenect_pcl, gain));
    
	jit_class_addattr(s_jit_freenect_pcl_class, attr);

	// finalize class
	jit_class_register(s_jit_freenect_pcl_class);
	return JIT_ERR_NONE;
}

/************************************************************************************/
// Object Life Cycle

t_jit_freenect_pcl *jit_freenect_pcl_new(void)
{
	t_jit_freenect_pcl	*x = NULL;
	
	x = (t_jit_freenect_pcl*)jit_object_alloc(s_jit_freenect_pcl_class);
	if (x) {
		x->gain = 0.0;
	} 
	return x;
}


void jit_freenect_pcl_free(t_jit_freenect_pcl *x)
{
	;	// nothing to free for our simple object
}


// PCL
pcl::PointCloud<pcl::PointNormal>::Ptr jit_freenect_pcl_normals( pcl::PointCloud<pcl::PointXYZ>::Ptr cloud )
{
    // Normal estimation*
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
    pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud (cloud);
    n.setInputCloud (cloud);
    n.setSearchMethod (tree);
    n.setKSearch (20);
    n.compute (*normals);
    //* normals should not contain the point normals + surface curvatures
    
    // Concatenate the XYZ and normal fields*
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
    pcl::concatenateFields (*cloud, *normals, *cloud_with_normals);
    
    return cloud_with_normals;
}


pcl::PointCloud<pcl::PointXYZ>::Ptr jit_freenect_pcl_genTriangles( pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals )
{
    // Create search tree*
    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud (cloud_with_normals);
    
    // Initialize objects
    pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
    pcl::PolygonMesh triangles;
    
    // Set the maximum distance between connected points (maximum edge length)
    gp3.setSearchRadius (0.025);
    
    // Set typical values for the parameters
    gp3.setMu (2.5);
    gp3.setMaximumNearestNeighbors (100);
    gp3.setMaximumSurfaceAngle(M_PI/4); // 45 degrees
    gp3.setMinimumAngle(M_PI/18); // 10 degrees
    gp3.setMaximumAngle(2*M_PI/3); // 120 degrees
    gp3.setNormalConsistency(false);
    
    // Get result
    gp3.setInputCloud (cloud_with_normals);
    gp3.setSearchMethod (tree2);
    gp3.reconstruct (triangles);
    
    //NOTE: use this later!
    // for each point, the ID of the containing connected component and its “state” (i.e. gp3.FREE, gp3.BOUNDARY or gp3.COMPLETED) can be retrieved.
    
    // Additional vertex information
    std::vector<int> parts = gp3.getPartIDs();
    std::vector<int> states = gp3.getPointStates();
    
    
    // Iterater for points in mesh:
    // this would be what we would pass to jitter if we wanted a mesh?
    pcl::PointCloud<pcl::PointXYZ>::Ptr tri_cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromPCLPointCloud2(triangles.cloud, *tri_cloud);
    
    pcl::PointCloud<pcl::PointXYZ>::Ptr vertexarr (new pcl::PointCloud<pcl::PointXYZ>);
    vertexarr->width = (uint32_t)triangles.polygons.size() * 3;
    vertexarr->height = 1;
    vertexarr->resize(vertexarr->width);
    
    size_t count = 0;
    for (std::vector<pcl::Vertices>::iterator it = triangles.polygons.begin(); it != triangles.polygons.end(); ++it)
    {
        for( size_t i = 0; i < it->vertices.size(); i++)
        {
             vertexarr->points[count++] = tri_cloud->points[it->vertices[i]];
        }
    }
    return vertexarr;
    
}



/************************************************************************************/
// Methods bound to input/inlets
t_jit_err jit_freenect_pcl_matrix_calc(t_jit_freenect_pcl *x, void *inputs, void *outputs)
{
	t_jit_err			err = JIT_ERR_NONE;
	long				in_savelock;
	long				out_savelock;
	t_jit_matrix_info	in_minfo; 
	t_jit_matrix_info	out_minfo;
	char				*in_bp;
	char				*out_bp;
	long				i, j;
	long				dimcount; 
	long				planecount; 
	long				dim[JIT_MATRIX_MAX_DIMCOUNT];
	void				*in_matrix;
	void				*out_matrix;

    long rowstride;
    float *fip, *fop;

	
	in_matrix 	= jit_object_method(inputs,_jit_sym_getindex,0);
	out_matrix 	= jit_object_method(outputs,_jit_sym_getindex,0);

	if (x && in_matrix && out_matrix)
    {
        in_savelock = (long) jit_object_method(in_matrix, _jit_sym_lock, 1);
		out_savelock = (long) jit_object_method(out_matrix, _jit_sym_lock, 1);
		
		jit_object_method(in_matrix, _jit_sym_getinfo, &in_minfo);
		jit_object_method(in_matrix, _jit_sym_getdata, &in_bp);
		
		if (!in_bp) { 
			err=JIT_ERR_INVALID_INPUT; 
			goto out;
		}
		
		//get dimensions/planecount
		dimcount   = in_minfo.dimcount;
        planecount = in_minfo.planecount;
        
        if( planecount < 3 )
        {
            object_error((t_object *)x, "jit.pcl requires at least a 3 plane matrix (xyzrgb)");
            goto out;
        }
        if( in_minfo.type != _jit_sym_float32)
        {
            object_error((t_object *)x, "received: %s jit.pcl uses only float32 matrixes", in_minfo.type->s_name );
            goto out;
        }

        //if dimsize is 1, treat as infinite domain across that dimension.
        //otherwise truncate if less than the output dimsize

		for (i=0; i<dimcount; i++) {
			dim[i] = in_minfo.dim[i];
			if ( in_minfo.dim[i]<dim[i] && in_minfo.dim[i]>1) {
				dim[i] = in_minfo.dim[i];
			}
		}
        
        //******
        // PCL stuff
        
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
        cloud->width    = (uint32_t)dim[0];
        cloud->height   = (uint32_t)dim[1];
        cloud->is_dense = false;
        cloud->points.resize (cloud->width * cloud->height);

        rowstride = in_minfo.dimstride[1];// >> 2L;
        size_t count = 0;
        
        for (j = 0; j < dim[0]; j++)
        {
            fip =  (float *)(in_bp + j * in_minfo.dimstride[0]);

            for( i = 0; i < dim[1]; i++)
            {
                cloud->points[count].x = ((float *)fip)[0];
                cloud->points[count].y = ((float *)fip)[1];
                cloud->points[count].z = ((float *)fip)[2];
                count++;
                fip += rowstride;
            }
        }
        
        
        pcl::PointCloud<pcl::PointNormal>::Ptr cloud_w_normals = jit_freenect_pcl_normals( cloud );
        pcl::PointCloud<pcl::PointXYZ>::Ptr triMesh = jit_freenect_pcl_genTriangles( cloud_w_normals );
      //  post( "res %d %d %d", triMesh->points.size(), triMesh->width, triMesh->height);

        
        //post("size %d, width %d, height %d counted %d", cloud_w_normals->points.size(), cloud_w_normals->width, cloud_w_normals->height, count );
        
        //*****
        // send back to jitter
        jit_object_method(out_matrix, _jit_sym_getinfo, &out_minfo);
        
        out_minfo.dim[0] = triMesh->width;
        out_minfo.dim[1] = triMesh->height;
        out_minfo.type = _jit_sym_float32;
        out_minfo.planecount = 3; //<< outputting xyz only now
        jit_object_method(out_matrix, _jit_sym_setinfo, &out_minfo);		
        jit_object_method(out_matrix, _jit_sym_getinfo, &out_minfo);
        
        jit_object_method(out_matrix, _jit_sym_getdata, &out_bp);
        if (!out_bp) {
            object_error((t_object *)x, "no output data" );
            err=JIT_ERR_INVALID_OUTPUT;
            goto out;
        }

        rowstride = out_minfo.dimstride[1];
        count = 0;
        for (j = 0; j < out_minfo.dim[0]; j++)
        {
            fop =  (float *)(out_bp + j * out_minfo.dimstride[0]);
            
            for( i = 0; i < out_minfo.dim[1]; i++)
            {
                if( count < cloud_w_normals->points.size() )
                {
                    fop[0] = triMesh->points[count].x;
                    fop[1] = triMesh->points[count].y;
                    fop[2] = triMesh->points[count].z;
//                    fop[3] = cloud_w_normals->points[count].normal_x;
//                    fop[4] = cloud_w_normals->points[count].normal_y;
//                    fop[5] = cloud_w_normals->points[count].normal_z;
                    
                }
                count++;
                fop += rowstride;
            }
        }
        

		
        // unable to make use of jitter's parallel methods since we need all the data together
		//jit_parallel_ndim_simplecalc2((method)jit_freenect_pcl_calculate_ndim,
		//	x, dimcount, dim, planecount, &in_minfo, in_bp, &out_minfo, out_bp,
		//	0 /* flags1 */, 0 /* flags2 */);
        
	} 
	else
		return JIT_ERR_INVALID_PTR;
	
out:
	jit_object_method( out_matrix, _jit_sym_lock, out_savelock );
	jit_object_method( in_matrix, _jit_sym_lock, in_savelock );
	return err;
}


// We are using a C++ template to process a vector of the matrix for any of the given types.
// Thus, we don't need to duplicate the code for each datatype.
template<typename T>
void jit_freenect_pcl_vector(t_jit_freenect_pcl *x, long n, t_jit_op_info *in, t_jit_op_info *out)
{
	double	gain = x->gain;
	T		*ip;
	T		*op;
	long	is,
	os;
	long	tmp;
	
	ip = ((T *)in->p);
	op = ((T *)out->p);
	is = in->stride; 
	os = out->stride; 
	
	if ((is==1) && (os==1)) {
		++n;
		--op;
		--ip;
		while (--n) {
			tmp = *++ip;
			*++op = tmp * gain;
		}		
	} 
	else {
		while (n--) {
			tmp = *ip;
			*op = tmp * gain;
			ip += is;
			op += os;
		}
	}
}


// We also use a C+ template for the loop that wraps the call to jit_freenect_pcl_vector(),
// further reducing code duplication in jit_freenect_pcl_calculate_ndim().
// The calls into these templates should be inlined by the compiler, eliminating concern about any added function call overhead.
template<typename T>
void jit_freenect_pcl_loop(t_jit_freenect_pcl *x, long n, t_jit_op_info *in_opinfo, t_jit_op_info *out_opinfo, t_jit_matrix_info *in_minfo, t_jit_matrix_info *out_minfo, char *bip, char *bop, long *dim, long planecount, long datasize)
{
	long	i;
	long	j;
	
	for (i=0; i<dim[1]; i++) {
		for (j=0; j<planecount; j++) {
			in_opinfo->p  = bip + i * in_minfo->dimstride[1]  + (j % in_minfo->planecount) * datasize;
			out_opinfo->p = bop + i * out_minfo->dimstride[1] + (j % out_minfo->planecount) * datasize;
			jit_freenect_pcl_vector<T>(x, n, in_opinfo, out_opinfo);
		}
	}
}


void jit_freenect_pcl_calculate_ndim(t_jit_freenect_pcl *x, long dimcount, long *dim, long planecount, t_jit_matrix_info *in_minfo, char *bip, t_jit_matrix_info *out_minfo, char *bop)
{
	long			i;
	long			n;
	char			*ip;
	char			*op;
	t_jit_op_info	in_opinfo;
	t_jit_op_info	out_opinfo;
	
	if (dimcount < 1) 
		return; // safety
	
	switch (dimcount) {
		case 1:
			dim[1]=1;
			// (fall-through to next case is intentional)
		case 2:
			// if planecount is the same then flatten planes - treat as single plane data for speed
			n = dim[0];
			if ((in_minfo->dim[0] > 1) && (out_minfo->dim[0] > 1) && (in_minfo->planecount == out_minfo->planecount)) {
				in_opinfo.stride = 1;
				out_opinfo.stride = 1;
				n *= planecount;
				planecount = 1;
			} 
			else {
				in_opinfo.stride =  in_minfo->dim[0]>1  ? in_minfo->planecount  : 0;
				out_opinfo.stride = out_minfo->dim[0]>1 ? out_minfo->planecount : 0;
			}
			
			if (in_minfo->type == _jit_sym_char)
				jit_freenect_pcl_loop<uchar>(x, n, &in_opinfo, &out_opinfo, in_minfo, out_minfo, bip, bop, dim, planecount, 1);
			else if (in_minfo->type == _jit_sym_long)
				jit_freenect_pcl_loop<long>(x, n, &in_opinfo, &out_opinfo, in_minfo, out_minfo, bip, bop, dim, planecount, 4);
			else if (in_minfo->type == _jit_sym_float32)
				jit_freenect_pcl_loop<float>(x, n, &in_opinfo, &out_opinfo, in_minfo, out_minfo, bip, bop, dim, planecount, 4);
			else if (in_minfo->type == _jit_sym_float64)
				jit_freenect_pcl_loop<double>(x, n, &in_opinfo, &out_opinfo, in_minfo, out_minfo, bip, bop, dim, planecount, 8);
			break;
		default:
			for	(i=0; i<dim[dimcount-1]; i++) {
				ip = bip + i * in_minfo->dimstride[dimcount-1];
				op = bop + i * out_minfo->dimstride[dimcount-1];
				jit_freenect_pcl_calculate_ndim(x, dimcount-1, dim, planecount, in_minfo, ip, out_minfo, op);
			}
	}
}
