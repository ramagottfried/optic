/**
this is almost working, but I think the depth information may not be converted properly, right now I'm outputting a char, but the incoming value is a short (this might be wrong also...)
*/

#include "jit.common.h"
#include "pxcsensemanager.h"
#include "pxcmetadata.h"

// Our Jitter object instance data
typedef struct _jit_realsense_grab {
	t_object			ob;
	PXCSenseManager		*sm;
	t_critical			lock;
	double				gain;	// our attribute (multiplied against each cell in the matrix)
} t_jit_realsense_grab;


// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err		jit_realsense_grab_init				(void); 
t_jit_realsense_grab	*jit_realsense_grab_new				(void);
void			jit_realsense_grab_free				(t_jit_realsense_grab *x);
t_jit_err		jit_realsense_grab_matrix_calc		(t_jit_realsense_grab *x, void *inputs, void *outputs);
void			jit_realsense_grab_calculate_ndim	(t_jit_realsense_grab *x, long dim, long *dimsize, long planecount, t_jit_matrix_info *out_minfo, char *bop);
END_USING_C_LINKAGE


// globals
static void *s_jit_realsense_grab_class = NULL;


/************************************************************************************/

t_jit_err jit_realsense_grab_init(void) 
{
	t_jit_object	*mop;
	
	s_jit_realsense_grab_class = jit_class_new("jit_realsense_grab", (method)jit_realsense_grab_new, (method)jit_realsense_grab_free, sizeof(t_jit_realsense_grab), 0);

	// add matrix operator (mop)
	mop = (t_jit_object*)jit_object_new(_jit_sym_jit_mop, 0, 1); // args are  num inputs and num outputs 

	jit_class_addadornment(s_jit_realsense_grab_class, mop);

	// add method(s)
	jit_class_addmethod(s_jit_realsense_grab_class, (method)jit_realsense_grab_matrix_calc, "matrix_calc", A_CANT, 0);

	// finalize class
	jit_class_register(s_jit_realsense_grab_class);
	return JIT_ERR_NONE;
}


/************************************************************************************/
// Object Life Cycle

t_jit_realsense_grab *jit_realsense_grab_new(void)
{
	t_jit_realsense_grab	*x = NULL;
	
	x = (t_jit_realsense_grab*)jit_object_alloc(s_jit_realsense_grab_class);
	if (x) {
		x->gain = 0.0;
		critical_new(&x->lock);

		x->sm = PXCSenseManager::CreateInstance();
		x->sm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 640, 480, 60);
		x->sm->Init();
		if (!x->sm) {
			wprintf_s(L"Unable to create the SenseManager\n");
			return NULL;
		}

	} 
	return x;
}


void jit_realsense_grab_free(t_jit_realsense_grab *x)
{
	x->sm->Release();
	critical_free(x->lock);
}


/************************************************************************************/
// Methods bound to input/inlets

t_jit_err jit_realsense_grab_matrix_calc(t_jit_realsense_grab *x, void *inputs, void *outputs)
{
	t_jit_err err = JIT_ERR_NONE;
	long out_savelock;
	t_jit_matrix_info out_minfo;
	char *out_bp;
	long i, xx, yy, dimcount, planecount, dim[JIT_MATRIX_MAX_DIMCOUNT];
	//t_jit_noise_vecdata	vecdata;
	void *out_matrix;

	PXCSenseManager *sm = x->sm;

	// note: if there is no frame available, should probably send out previous frame (to do)
	if (sm->AcquireFrame(true) < PXC_STATUS_NO_ERROR)
	{
		object_error((t_object *)x, "camera not available");
		return JIT_ERR_GENERIC;
	}

	// retrieve the sample
	PXCCapture::Sample *sample = sm->QuerySample();
	PXCImage::ImageInfo info = sample->depth->QueryInfo();
	PXCImage::ImageData data;
	out_matrix = jit_object_method(outputs, _jit_sym_getindex, 0);

//	post("w %d h %d format %s", info.width, info.height, PXCImage::PixelFormatToString(info.format));

	//get zeroith outlet
	if (x && out_matrix) 
	{
		// lock output
		out_savelock = (long)jit_object_method(out_matrix, _jit_sym_lock, 1);

		// fill out matrix info structs for input and output
		jit_object_method(out_matrix, _jit_sym_getinfo, &out_minfo);
		
		// get pointer to matrix
		jit_object_method(out_matrix, _jit_sym_getdata, &out_bp);

		if (!out_bp){ err = JIT_ERR_INVALID_OUTPUT; goto out; }

		if (!(sample->depth->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH, &data) >= PXC_STATUS_NO_ERROR))
		{
			object_error((t_object *)x, "aquire error");
			goto out;
		}

		//get dimensions/planecount
		dimcount = out_minfo.dimcount;
		planecount = out_minfo.planecount;

		if (dimcount != 2){ err = JIT_ERR_MISMATCH_DIM; goto out; }
		
		//post("dim %d x %d planes %d stride %ld", out_minfo.dim[0], out_minfo.dim[1], planecount, out_minfo.dimstride);

		for (i = 0; i < dimcount; i++) {
			dim[i] = out_minfo.dim[i];
		}

		short *dpixels = (short *)data.planes[0];
		int dpitch = data.pitches[0] / sizeof(short);
		for (yy = 0; yy < dim[1]; yy++)
		{
			for (xx = 0; xx < dim[0]; xx++)
			{
				*out_bp++ = (char)dpixels[yy * dpitch + xx];
			}
		}


		//jit_parallel_ndim_simplecalc1((method)jit_realsense_grab_calculate_ndim, x, dimcount, dim, planecount, &out_minfo, out_bp, 0 /* flags1 */);
	}
	else 
		err = JIT_ERR_INVALID_PTR;

out:
	sample->depth->ReleaseAccess(&data);
	sm->ReleaseFrame();

	jit_object_method(out_matrix, _jit_sym_lock, out_savelock);
	return err;
}


void jit_realsense_grab_calculate_ndim(t_jit_realsense_grab *x, long dimcount, long *dim, long planecount, t_jit_matrix_info *out_minfo, char *bop)
{
	long i, j, width, height;
	char *op;
	
	if (dimcount<1) return; //safety

	switch (dimcount)
	{
	case 1:
		// if only 1D, interpret as 2D, falling through to 2D case 
		dim[1] = 1;
	case 2:
		width = dim[0];
		height = dim[1];

		// for each row
		for (i = 0; i<height; i++)
		{
			// increment data pointers according to byte stride 
			op = bop + i*out_minfo->dimstride[1];

			for (j = 0; j < width; j++)
			{
				*op++ = 255;
			}
		}
		break;
	default:
		
		for (i = 0; i<dim[dimcount - 1]; i++)
		{
			op = bop + i*out_minfo->dimstride[dimcount - 1];
			jit_realsense_grab_calculate_ndim(x, dimcount - 1,
				dim, planecount, out_minfo, op);
		}
	}
}
