#include "jit.common.h"

#include "kinect_device_wrapper.h"


typedef struct _jit_freenect_pcl
{
	t_object        ob;
    long            version;
    kinect_device   *kinect;
    bool            poll;
    
} t_jit_freenect_pcl;


// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err           jit_freenect_pcl_init				(void);
t_jit_freenect_pcl  *jit_freenect_pcl_new				(void);
void                jit_freenect_pcl_free				(t_jit_freenect_pcl *x);
t_jit_err           jit_freenect_pcl_matrix_calc		(t_jit_freenect_pcl *x, void *inputs, void *outputs);

void                jit_freenect_pcl_calculate_ndim     (t_jit_freenect_pcl *x,
                                                            long dim, long *dimsize, long planecount,
                                                            t_jit_matrix_info *in_minfo,char *bip,
                                                            t_jit_matrix_info *out_minfo, char *bop);
END_USING_C_LINKAGE



// globals
static void *s_jit_freenect_pcl_class = NULL;


t_jit_err jit_freenect_pcl_init(void) 
{
	long			attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
	t_jit_object	*attr;
	t_jit_object	*mop;

	s_jit_freenect_pcl_class = jit_class_new("jit_freenect_pcl", (method)jit_freenect_pcl_new, (method)jit_freenect_pcl_free, sizeof(t_jit_freenect_pcl), 0);


	mop = (t_jit_object *)jit_object_new(_jit_sym_jit_mop, 0, 2);

    jit_mop_single_planecount(mop, 3);
   
    attr = (t_jit_object *)jit_object_method(mop, _jit_sym_getoutput, 0);
    jit_attr_setsym(attr, _jit_sym_type, _jit_sym_float32 );
    
    attr = (t_jit_object *)jit_object_method(mop, _jit_sym_getoutput, 1);
    jit_attr_setsym(attr, _jit_sym_type, _jit_sym_char );
    
    
	jit_class_addadornment(s_jit_freenect_pcl_class, mop);
    
	// add method(s)
	jit_class_addmethod(s_jit_freenect_pcl_class, (method)jit_freenect_pcl_matrix_calc, "matrix_calc", A_CANT, 0);

	// add attribute(s)
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
										 "version",
										 _jit_sym_long,
										 attrflags, 
										 (method)NULL, (method)NULL, 
										 calcoffset(t_jit_freenect_pcl, version));
	jit_class_addattr(s_jit_freenect_pcl_class, attr);

    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "poll",
                                          _jit_sym_long,
                                          attrflags,
                                          (method)NULL, (method)NULL,
                                          calcoffset(t_jit_freenect_pcl, poll));
    jit_class_addattr(s_jit_freenect_pcl_class, attr);
    
	// finalize class
	jit_class_register(s_jit_freenect_pcl_class);
	return JIT_ERR_NONE;
}


t_jit_freenect_pcl *jit_freenect_pcl_new(void)
{
	t_jit_freenect_pcl	*x = NULL;
	
	x = (t_jit_freenect_pcl*)jit_object_alloc(s_jit_freenect_pcl_class);
	if (x)
    {
		x->version = 2;
        x->poll = 0;
        x->kinect = new kinect_device( (t_object *)x );
        
    }
	return x;
}


void jit_freenect_pcl_free(t_jit_freenect_pcl *x)
{
    delete x->kinect;
}


/************************************************************************************/
// Methods bound to input/inlets
t_jit_err jit_freenect_pcl_matrix_calc(t_jit_freenect_pcl *x, void *inputs, void *outputs)
{
	t_jit_err			err = JIT_ERR_NONE;
	long				out_savelock0, out_savelock1;
	t_jit_matrix_info	out_minfo0, out_minfo1;
	char				*out_bp0, *out_bp1;
	long				i, j;
	long				dimcount; 
	long				planecount; 
	long				dim[ JIT_MATRIX_MAX_DIMCOUNT ];
	void				*out_matrix0, *out_matrix1;

    long rowstride;
    float *fip, *fop;

	
	out_matrix0 	= jit_object_method( outputs, _jit_sym_getindex, 0 );
    out_matrix1 	= jit_object_method( outputs, _jit_sym_getindex, 1 );

	if (x && out_matrix0 && out_matrix1)
    {
		out_savelock0 = (long) jit_object_method( out_matrix0, _jit_sym_lock, 1 );
		out_savelock1 = (long) jit_object_method( out_matrix1, _jit_sym_lock, 1 );
        
		jit_object_method( out_matrix0, _jit_sym_getinfo, &out_minfo0 );
        jit_object_method( out_matrix1, _jit_sym_getinfo, &out_minfo1 );
        
		jit_object_method( out_matrix0, _jit_sym_getdata, &out_bp0 );
		jit_object_method( out_matrix1, _jit_sym_getdata, &out_bp1 );
        
        
		//get dimensions/planecount
		dimcount   = out_minfo0.dimcount;
        planecount = out_minfo0.planecount;
        
        post("0 type: %s  1 type: %s", out_minfo0.type->s_name, out_minfo1.type->s_name );
        

        //x->kinect->enable_polling( x->poll > 0 );
        
        // maybe do polling in max?
       // while ( x->poll )
        {
            x->kinect->getFrames(&out_bp0, &out_bp1);
        }
        
        
        dim[0] = 1920;
        dim[1] = 1080;

      //  jit_parallel_ndim_simplecalc2((method)jit_freenect_pcl_calculate_ndim, x, dimcount, dim, planecount, 0L, 0L, &out_minfo0, out_bp0, 0 /* flags1 */, 0 /* flags2 */);
        
	} 
	else
		return JIT_ERR_INVALID_PTR;
	
out:
	jit_object_method( out_matrix0, _jit_sym_lock, out_savelock0 );
    jit_object_method( out_matrix1, _jit_sym_lock, out_savelock1 );
	return err;
}


// We are using a C++ template to process a vector of the matrix for any of the given types.
// Thus, we don't need to duplicate the code for each datatype.
template<typename T>
void jit_freenect_pcl_vector(t_jit_freenect_pcl *x, long n, t_jit_op_info *in, t_jit_op_info *out)
{
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
			*++op = tmp;
		}		
	} 
	else {
		while (n--) {
			tmp = *ip;
			*op = tmp;
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
