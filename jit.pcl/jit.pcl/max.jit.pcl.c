
#include "jit.common.h"
#include "max.jit.mop.h"


typedef struct _max_jit_pcl {
	t_object	ob;
	void		*obex;
} t_max_jit_pcl;


BEGIN_USING_C_LINKAGE
t_jit_err	jit_pcl_init(void);
void		*max_jit_pcl_new(t_symbol *s, long argc, t_atom *argv);
void		max_jit_pcl_free(t_max_jit_pcl *x);
END_USING_C_LINKAGE

static void	*max_jit_pcl_class = NULL;

void *max_jit_pcl_new(t_symbol *s, long argc, t_atom *argv)
{
    t_max_jit_pcl	*x;
    void			*o;
    
    x = (t_max_jit_pcl*)max_jit_object_alloc(max_jit_pcl_class, gensym("jit_pcl"));
    if (x) {
        o = jit_object_new(gensym("jit_pcl"));
        if (o) {
            max_jit_mop_setup_simple(x, o, argc, argv);
            max_jit_attr_args(x, argc, argv);
        }
        else {
            jit_object_error((t_object*)x, "jit.pcl: could not allocate object");
            object_free((t_object*)x);
            x = NULL;
        }
    }
    return (x);
}

void max_jit_pcl_free(t_max_jit_pcl *x)
{
    max_jit_mop_free(x);
    jit_object_free(max_jit_obex_jitob_get(x));
    max_jit_object_free(x);
}

int C74_EXPORT main(void)
{
	t_class *max_class, *jit_class;
	
	jit_pcl_init();	

	max_class = class_new("jit.pcl", (method)max_jit_pcl_new, (method)max_jit_pcl_free, sizeof(t_max_jit_pcl), NULL, A_GIMME, 0);
	max_jit_class_obex_setup(max_class, calcoffset(t_max_jit_pcl, obex));

	jit_class = jit_class_findbyname(gensym("jit_pcl"));
    max_jit_class_mop_wrap(max_class, jit_class, 0);			// attrs & methods for name, type, dim, planecount, bang, outputmatrix, etc
    max_jit_classex_mop_wrap(max_class, jit_class, MAX_JIT_MOP_FLAGS_OWN_BANG|MAX_JIT_MOP_FLAGS_OWN_OUTPUTMATRIX); //custom bang/outputmatrix
    
	max_jit_class_wrap_standard(max_class, jit_class, 0);		// attrs & methods for getattributes, dumpout, maxjitclassaddmethods, etc

	class_addmethod(max_class, (method)max_jit_mop_assist, "assist", A_CANT, 0);	// standard matrix-operator (mop) assist fn

	class_register(CLASS_BOX, max_class);
	max_jit_pcl_class = max_class;
	return 0;
}