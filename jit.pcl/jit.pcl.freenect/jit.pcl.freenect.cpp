#include "kinect_device_wrapper.h"

#include "jit.common.h"


typedef struct _jit_pcl_freenect
{
	t_object                ob;
    
    long                    enable;
    
    io_kinect_device*       kinect;
    libfreenect2::Frame*    rgb_frame;
    libfreenect2::Frame*    depth_frame;
    libfreenect2::Frame*    ir_frame;
    
    t_critical              lock;
} t_jit_pcl_freenect;


BEGIN_USING_C_LINKAGE
t_jit_err                   jit_pcl_freenect_init				(void);
t_jit_pcl_freenect          *jit_pcl_freenect_new				(void);
void                        jit_pcl_freenect_free				(t_jit_pcl_freenect *x);
t_jit_err                   jit_pcl_freenect_matrix_calc		(t_jit_pcl_freenect *x, void *inputs, void *outputs);
void                        jit_pcl_freenect_calculate_ndim     (t_jit_pcl_freenect *x, long dim, long *dimsize, long planecount, t_jit_matrix_info *in_minfo,char *bip, t_jit_matrix_info *out_minfo, char *bop);
t_jit_err jit_pcl_freenect_poll_get(t_jit_pcl_freenect *x, void *attr, long *ac, t_atom **av);
t_jit_err jit_pcl_freenect_poll_set(t_jit_pcl_freenect *x, void *attr, long ac, t_atom *av);

END_USING_C_LINKAGE

static void *s_jit_pcl_freenect_class = NULL;

t_jit_err jit_pcl_freenect_matrix_calc(t_jit_pcl_freenect *x, void *inputs, void *outputs)
{
	t_jit_err			err = JIT_ERR_NONE;
	long				in_savelock;
	long				out_savelock;
	t_jit_matrix_info	out_minfo;
	char				*out_rgb_bp, *out_depth_bp;
	long				i, j;
	long				dimcount; 
	long				planecount; 
	long				dim[JIT_MATRIX_MAX_DIMCOUNT];
	void				*in_matrix;
	void				*out_matrix;

	
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
        
        
        // unable to make use of jitter's parallel methods since we need all the data together
		//jit_parallel_ndim_simplecalc2((method)jit_pcl_freenect_calculate_ndim,
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
void jit_pcl_freenect_vector(t_jit_pcl_freenect *x, long n, t_jit_op_info *in, t_jit_op_info *out)
{
	double	gain = 0;
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

template<typename T>
void jit_pcl_freenect_loop(t_jit_pcl_freenect *x, long n, t_jit_op_info *in_opinfo, t_jit_op_info *out_opinfo, t_jit_matrix_info *in_minfo, t_jit_matrix_info *out_minfo, char *bip, char *bop, long *dim, long planecount, long datasize)
{
	long	i;
	long	j;
	
	for (i=0; i<dim[1]; i++) {
		for (j=0; j<planecount; j++) {
			in_opinfo->p  = bip + i * in_minfo->dimstride[1]  + (j % in_minfo->planecount) * datasize;
			out_opinfo->p = bop + i * out_minfo->dimstride[1] + (j % out_minfo->planecount) * datasize;
			jit_pcl_freenect_vector<T>(x, n, in_opinfo, out_opinfo);
		}
	}
}


void jit_pcl_freenect_calculate_ndim(t_jit_pcl_freenect *x, long dimcount, long *dim, long planecount, t_jit_matrix_info *in_minfo, char *bip, t_jit_matrix_info *out_minfo, char *bop)
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
				jit_pcl_freenect_loop<uchar>(x, n, &in_opinfo, &out_opinfo, in_minfo, out_minfo, bip, bop, dim, planecount, 1);
			else if (in_minfo->type == _jit_sym_long)
				jit_pcl_freenect_loop<long>(x, n, &in_opinfo, &out_opinfo, in_minfo, out_minfo, bip, bop, dim, planecount, 4);
			else if (in_minfo->type == _jit_sym_float32)
				jit_pcl_freenect_loop<float>(x, n, &in_opinfo, &out_opinfo, in_minfo, out_minfo, bip, bop, dim, planecount, 4);
			else if (in_minfo->type == _jit_sym_float64)
				jit_pcl_freenect_loop<double>(x, n, &in_opinfo, &out_opinfo, in_minfo, out_minfo, bip, bop, dim, planecount, 8);
			break;
		default:
			for	(i=0; i<dim[dimcount-1]; i++) {
				ip = bip + i * in_minfo->dimstride[dimcount-1];
				op = bop + i * out_minfo->dimstride[dimcount-1];
				jit_pcl_freenect_calculate_ndim(x, dimcount-1, dim, planecount, in_minfo, ip, out_minfo, op);
			}
	}
}

// -------- freenect

void o_freenect_grab_callback(t_object *x_obj)
{
    t_jit_pcl_freenect *x = (t_jit_pcl_freenect *)x_obj;
    
    //    x->kinect->getRegisteredFrames();
    critical_enter(x->lock);
    
    if( x->kinect )
        x->kinect->getFrames( &x->rgb_frame, &x->depth_frame, &x->ir_frame );
    
    critical_exit(x->lock);
    
    
    if( x->rgb_frame )
        post("outer %ld %ld %d %d", x->rgb_frame->width, x->rgb_frame->height, x->rgb_frame->data[0], x->rgb_frame->data[1]);
    
}

void o_freenect_grab_connect(t_jit_pcl_freenect *x)
{
    critical_enter(x->lock);
    
    x->kinect = new io_kinect_device;
    
    t_object *x_obj = (t_object *)x;
    std::function<void(t_object*)> cb = std::bind(o_freenect_grab_callback, x_obj);
    x->kinect->setCallbackType( cb, x_obj, libfreenect2::Frame::Type::Depth );
    
    if ( ! x->kinect->acitve() )
    {
        post("no active kinect");
        return;
    }
    
    critical_exit(x->lock);
    
}


// -------- max seutp

t_jit_err jit_pcl_freenect_poll_get(t_jit_pcl_freenect *x, void *attr, long *ac, t_atom **av)
{
    if ((*ac)&&(*av)) {
        //memory passed in, use it
    } else {
        //otherwise allocate memory
        *ac = 1;
        *av = (t_atom *)jit_getbytes( sizeof(t_atom) * (*ac) );
        if ( !(*av) )
        {
            *ac = 0;
            return JIT_ERR_OUT_OF_MEM;
        }
    }
    jit_atom_setlong(*av, x->enable);
    return JIT_ERR_NONE;
}

t_jit_err jit_pcl_freenect_poll_set(t_jit_pcl_freenect *x, void *attr, long ac, t_atom *av)
{
    if (ac&&av)
    {
        x->enable = (jit_atom_getlong(av) > 0);
        if( x->enable )
            o_freenect_grab_connect( x );
        else
        {
            critical_enter(x->lock);
            if( x->kinect )
            {
                delete x->kinect;
                x->kinect = NULL;
            }
            critical_exit(x->lock);
        }
        
        post("hello got it");
    }
    else
    {
        x->enable = 0;
        
    }
    return JIT_ERR_NONE;
}


void jit_pcl_freenect_free(t_jit_pcl_freenect *x)
{
    if( x->kinect )
        delete x->kinect;
    if( x->rgb_frame )
        delete x->rgb_frame;
    if( x-> depth_frame )
        delete x->depth_frame;
    if( x->ir_frame )
        delete x->ir_frame;
    
    x->kinect = NULL;
    x->rgb_frame = NULL;
    x->depth_frame = NULL;
    x->ir_frame = NULL;

    critical_free(x->lock);
}

t_jit_pcl_freenect *jit_pcl_freenect_new(void)
{
    t_jit_pcl_freenect	*x = NULL;
    
    x = (t_jit_pcl_freenect*)jit_object_alloc(s_jit_pcl_freenect_class);
    if (x)
    {
        x->enable = 0;
        critical_new(&(x->lock));
        
        x->kinect = NULL;
        x->rgb_frame = NULL;
        x->depth_frame = NULL;
        x->ir_frame = NULL;
    }
    return x;
}

t_jit_err jit_pcl_freenect_init(void)
{
    long			attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
    t_jit_object	*attr;
    t_jit_object	*mop;
    t_atom a[1];
    
    s_jit_pcl_freenect_class = jit_class_new("jit_pcl_freenect",
                                             (method)jit_pcl_freenect_new,
                                             (method)jit_pcl_freenect_free,
                                             sizeof(t_jit_pcl_freenect),
                                             0);
    
    mop = (t_jit_object *)jit_object_new(_jit_sym_jit_mop, 0, 2);
    
    jit_class_addadornment(s_jit_pcl_freenect_class, mop);
    
    jit_class_addmethod(s_jit_pcl_freenect_class, (method)jit_pcl_freenect_matrix_calc, "matrix_calc", A_CANT, 0);
    
    
    attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,
                                          "enable",
                                          _jit_sym_long,
                                          attrflags,
                                          (method)jit_pcl_freenect_poll_get,
                                          (method)jit_pcl_freenect_poll_set,
                                          calcoffset(t_jit_pcl_freenect, enable));
    
    jit_class_addattr(s_jit_pcl_freenect_class, attr);

    jit_class_register(s_jit_pcl_freenect_class);
    return JIT_ERR_NONE;
}