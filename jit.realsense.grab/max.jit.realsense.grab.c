/**
	@file
	max.jit.simple - simple example of a Jitter external
	multiplies an incoming matrix by a constant
	 
	@ingroup	examples
	@see		jit.simple
 
	Copyright 2009 - Cycling '74
	Timothy Place, tim@cycling74.com
*/

#include "jit.common.h"
#include "max.jit.mop.h"


// Max object instance data
// Note: most instance data is in the Jitter object which we will wrap
typedef struct _max_jit_realsense_grab {
	t_object	ob;
	void		*obex;
} t_max_jit_realsense_grab;


// prototypes
BEGIN_USING_C_LINKAGE
t_jit_err	jit_realsense_grab_init(void);
void		*max_jit_realsense_grab_new(t_symbol *s, long argc, t_atom *argv);
void		max_jit_realsense_grab_free(t_max_jit_realsense_grab *x);
void		max_jit_realsense_grab_outputmatrix(t_max_jit_realsense_grab *x);
END_USING_C_LINKAGE

// globals
static void	*max_jit_realsense_grab_class = NULL;

void max_jit_realsense_grab_outputmatrix(t_max_jit_realsense_grab *x)
{
//	post("%s", __FUNCTION__);
	t_atom a;
	long outputmode = max_jit_mop_getoutputmode(x);
	void *mop = max_jit_obex_adornment_get(x, _jit_sym_jit_mop);
	t_jit_err err;

	if (outputmode&&mop)
	{ //always output unless output mode is none
		if (outputmode == 1)
		{
			if (err = (t_jit_err)jit_object_method(
				max_jit_obex_jitob_get(x),
				_jit_sym_matrix_calc,
				jit_object_method(mop, _jit_sym_getinputlist),
				jit_object_method(mop, _jit_sym_getoutputlist)))
			{
				jit_error_code(x, err);
			}
			else {
				max_jit_mop_outputmatrix(x);
			}
		}
		else {
			max_jit_mop_outputmatrix(x);
		}
	}
}


/************************************************************************************/

int C74_EXPORT main(void)
{
	t_class *max_class, *jit_class;
	
	jit_realsense_grab_init();	

	max_class = class_new("jit.realsense.grab", (method)max_jit_realsense_grab_new, (method)max_jit_realsense_grab_free, sizeof(t_max_jit_realsense_grab), NULL, A_GIMME, 0);
	max_jit_class_obex_setup(max_class, calcoffset(t_max_jit_realsense_grab, obex));

	jit_class = jit_class_findbyname(gensym("jit_realsense_grab"));
	max_jit_class_mop_wrap(max_class, jit_class, MAX_JIT_MOP_FLAGS_OWN_OUTPUTMATRIX | MAX_JIT_MOP_FLAGS_OWN_JIT_MATRIX);			// attrs & methods for name, type, dim, planecount, bang, outputmatrix, etc
	max_jit_class_wrap_standard(max_class, jit_class, 0);		// attrs & methods for getattributes, dumpout, maxjitclassaddmethods, etc

	max_jit_class_addmethod_usurp_low(max_class, (method)max_jit_realsense_grab_outputmatrix, "outputmatrix");

	class_addmethod(max_class, (method)max_jit_mop_assist, "assist", A_CANT, 0);	// standard matrix-operator (mop) assist fn

	class_register(CLASS_BOX, max_class);
	max_jit_realsense_grab_class = max_class;
	return 0;
}


/************************************************************************************/
// Object Life Cycle

void *max_jit_realsense_grab_new(t_symbol *s, long argc, t_atom *argv)
{
	t_max_jit_realsense_grab	*x;
	void			*o;

	x = (t_max_jit_realsense_grab*)max_jit_object_alloc(max_jit_realsense_grab_class, gensym("jit_realsense_grab"));
	if (x) {
		o = jit_object_new(gensym("jit_realsense_grab"));
		if (o) 
		{
			max_jit_mop_setup_simple(x, o, argc, argv);			
			max_jit_attr_args(x, argc, argv);
		} 
		else 
		{
			jit_object_error((t_object*)x, "jit.realsense.grab: could not allocate object");
			object_free((t_object*)x);
			x = NULL;
		}
	}
	return (x);
}


void max_jit_realsense_grab_free(t_max_jit_realsense_grab *x)
{
	max_jit_mop_free(x);
	jit_object_free(max_jit_obex_jitob_get(x));
	max_jit_object_free(x);
}
