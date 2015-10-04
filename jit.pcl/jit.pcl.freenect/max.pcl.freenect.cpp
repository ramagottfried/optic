
#include "ext.h"
#include "ext_obex.h"
#include "jit.common.h"
#include "max.jit.mop.h"

#include "kinect_device_wrapper.h"


typedef struct _max_jit_pcl_freenect
{
	t_object        ob;
	void            *obex;
    
    void            *rgb_matrix;
    t_atom          rgb_name;
    void            *rgb_out;
    
    void            *depth_matrix;
    t_atom          depth_name;
    void            *depth_out;
    
    
    long                    enable;
    
    io_kinect_device        *kinect;
    libfreenect2::Frame     *rgb_frame;
    libfreenect2::Frame     *depth_frame;
    libfreenect2::Frame     *ir_frame;
    
    t_critical              lock;


    void                    *m_qelem;
    
} t_max_jit_pcl_freenect;


BEGIN_USING_C_LINKAGE
void		*max_jit_pcl_freenect_new(t_symbol *s, long argc, t_atom *argv);
void		max_jit_pcl_freenect_free(t_max_jit_pcl_freenect *x);
END_USING_C_LINKAGE

t_class	*max_jit_pcl_freenect_class = NULL;

void max_jit_pcl_freenect_outMatrix(t_max_jit_pcl_freenect *x)
{
    if( x->rgb_matrix )
        outlet_anything(x->rgb_out, _jit_sym_jit_matrix, 1, &x->rgb_name);
    
    if( x->depth_matrix )
        outlet_anything(x->depth_out, _jit_sym_jit_matrix, 1, &x->depth_name);
}

void o_freenect_grab_callback(t_object *x_obj)
{
    t_max_jit_pcl_freenect *x = (t_max_jit_pcl_freenect *)x_obj;
    
    if( !x->enable )
        return;
    if( !x->kinect )
        return;
    
    critical_enter(x->lock);
    bool errors = x->kinect->getMatrixes( x->rgb_matrix, x->depth_matrix );
    critical_exit(x->lock);
    
    if( !errors )
        qelem_set(x->m_qelem);
      
}

void o_freenect_grab_connect(t_max_jit_pcl_freenect *x)
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


void o_freenect_grab_disconnect(t_max_jit_pcl_freenect *x)
{
    critical_enter(x->lock);
    if( x->kinect )
    {
        delete x->kinect;
        x->kinect = NULL;
    }
    critical_exit(x->lock);
}

void max_jit_pcl_freenect_enable(t_max_jit_pcl_freenect *x, int i)
{
    x->enable = (i > 0);
    
    if( x->enable )
        o_freenect_grab_connect( x );
    else
        ;
        //o_freenect_grab_disconnect( x );
    
}


void max_jit_pcl_freenect_free(t_max_jit_pcl_freenect *x)
{
    
    x->enable = 0;
    
    if( x->kinect )
        delete x->kinect;
    if( x->rgb_frame )
        delete x->rgb_frame;
    if( x-> depth_frame )
        delete x->depth_frame;
    if( x->ir_frame )
        delete x->ir_frame;
   
    if( x->rgb_matrix )
        jit_object_free(x->rgb_matrix);

    if( x->depth_matrix )
        jit_object_free(x->depth_matrix);
    
    x->kinect = NULL;
    x->rgb_frame = NULL;
    x->depth_frame = NULL;
    x->ir_frame = NULL;
    
    critical_free(x->lock);
    
    qelem_free(x->m_qelem);
    
    max_jit_object_free(x);

}

void *max_jit_pcl_freenect_new(t_symbol *s, long argc, t_atom *argv)
{
    t_max_jit_pcl_freenect	*x;
    
    x = (t_max_jit_pcl_freenect *)max_jit_object_alloc( max_jit_pcl_freenect_class, NULL );
    if( x )
    {
        
        x->rgb_out = outlet_new(x, "jit_matrix");
        x->depth_out = outlet_new(x, "jit_matrix");
        
        t_jit_matrix_info rgb_info, depth_info;
        
        t_symbol *rgb_name_unique = symbol_unique();
        jit_matrix_info_default(&rgb_info);
        rgb_info.type = _jit_sym_char;
        rgb_info.planecount = 3;
        rgb_info.dim[0] = 1920;
        rgb_info.dim[1] = 1080;
        x->rgb_matrix = jit_object_new(_jit_sym_jit_matrix, &rgb_info);
        x->rgb_matrix = jit_object_method(x->rgb_matrix, _jit_sym_register, rgb_name_unique);
        atom_setsym( &x->rgb_name, rgb_name_unique );
       
        t_symbol *depth_name_unique = symbol_unique();
        jit_matrix_info_default(&depth_info);
        depth_info.type = _jit_sym_float32;
        depth_info.planecount = 1;
        depth_info.dim[0] = 512;
        depth_info.dim[1] = 424;
        x->depth_matrix = jit_object_new(_jit_sym_jit_matrix, &depth_info);
        x->depth_matrix = jit_object_method(x->depth_matrix, _jit_sym_register, depth_name_unique);
        atom_setsym( &x->depth_name, depth_name_unique );
        
        
        x->enable = 0;
        critical_new(&(x->lock));
        
        x->kinect = NULL;
        x->rgb_frame = NULL;
        x->depth_frame = NULL;
        x->ir_frame = NULL;
        
        x->m_qelem = qelem_new((t_object *)x, (method)max_jit_pcl_freenect_outMatrix);
    }
    return (x);
}

int C74_EXPORT main(void)
{
	t_class *max_class;
    
	max_class = class_new("jit.pcl.freenect",
                          (method)max_jit_pcl_freenect_new,
                          (method)max_jit_pcl_freenect_free,
                          sizeof(t_max_jit_pcl_freenect), NULL, A_GIMME, 0);

    max_jit_class_obex_setup(max_class, calcoffset(t_max_jit_pcl_freenect, obex));

    //class_addmethod(max_class, (method)max_jit_mop_assist, "assist", A_CANT, 0);
    class_addmethod(max_class, (method)max_jit_pcl_freenect_enable, "int", A_LONG, 0);
    
	class_register(CLASS_BOX, max_class);
	max_jit_pcl_freenect_class = max_class;
	return 0;
}