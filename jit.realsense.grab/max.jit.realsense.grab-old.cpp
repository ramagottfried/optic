/* 
	Copyright 2001-2005 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com	
*/

#include "jit.common.h"
#include <windows.h>
#include "pxcsensemanager.h"
#include "pxcmetadata.h"
//#include <conio.h>

typedef struct _max_jit_realsense_grab 
{
	t_object			ob;
	void				*obex;

	PXCSenseManager		*pp;

	void 				*bangout;
	t_symbol			*matrix_name;
	long				plane;
	long				offsetcount;
	long 				offset[JIT_MATRIX_MAX_DIMCOUNT];
	
} t_max_jit_realsense_grab;

void *max_jit_realsense_grab_new(t_symbol *s, long argc, t_atom *argv);
void max_jit_realsense_grab_free(t_max_jit_realsense_grab *x);
void max_jit_realsense_grab_assist(t_max_jit_realsense_grab *x, void *b, long m, long a, char *s);
void max_jit_realsense_grab_int(t_max_jit_realsense_grab *x, long val);
void max_jit_realsense_grab_float(t_max_jit_realsense_grab *x, double val);
void max_jit_realsense_grab_list(t_max_jit_realsense_grab *x, t_symbol *s, long argc, t_atom *argv);
//void max_jit_realsense_grab_offset_bp(t_max_jit_realsense_grab *x, char **bp, t_jit_matrix_info *minfo);

t_messlist *max_jit_realsense_grab_class;
		 	
t_symbol *ps_done;

void C74_EXPORT main(void)
{	
	long attrflags;
	void *p,*attr;
	
	setup(&max_jit_realsense_grab_class, (method)max_jit_realsense_grab_new, (method)max_jit_realsense_grab_free, (short)sizeof(t_max_jit_realsense_grab), 
		0L, A_GIMME, 0);

	p = max_jit_classex_setup(calcoffset(t_max_jit_realsense_grab,obex));

	addint((method)max_jit_realsense_grab_int);
	addfloat((method)max_jit_realsense_grab_float);	
	max_addmethod_defer_low((method)max_jit_realsense_grab_list, "list");	

 	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_DEFER_LOW ;
	attr = jit_object_new(_jit_sym_jit_attr_offset,"matrix_name",_jit_sym_symbol,attrflags,
		(method)0L,(method)0L,calcoffset(t_max_jit_realsense_grab,matrix_name));
	max_jit_classex_addattr(p,attr);
	attr = jit_object_new(_jit_sym_jit_attr_offset,"plane",_jit_sym_long,attrflags,
		(method)0L,(method)0L,calcoffset(t_max_jit_realsense_grab,plane));
	max_jit_classex_addattr(p,attr);
	attr = jit_object_new(_jit_sym_jit_attr_offset_array,"offset",_jit_sym_long,JIT_MATRIX_MAX_DIMCOUNT,attrflags,
		(method)0L,(method)0L,calcoffset(t_max_jit_realsense_grab,offsetcount),calcoffset(t_max_jit_realsense_grab,offset));
	max_jit_classex_addattr(p,attr);
	
	max_jit_classex_standard_wrap(p,NULL,0);	
	addmess((method)max_jit_realsense_grab_assist,			"assist",			A_CANT,0);

}

void max_jit_realsense_grab_int(t_max_jit_realsense_grab *x, long val)
{
	t_atom a;
	
	jit_atom_setlong(&a,val);
	typedmess((t_object *) x,_jit_sym_list,1,&a);
}

void max_jit_realsense_grab_float(t_max_jit_realsense_grab *x, double val)
{
	t_atom a;
	
	jit_atom_setfloat(&a,val);
	typedmess((t_object *) x,_jit_sym_list,1,&a);
}

void max_jit_realsense_grab_list(t_max_jit_realsense_grab *x, t_symbol *s, long argc, t_atom *argv)
{
	void *matrix;
	long err,i,j;
	long savelock,offset0,offset1;
	t_jit_matrix_info minfo;
	char *bp,*p;
	
	if (argc&&argv) {
		//find matrix
		matrix = jit_object_findregistered(x->matrix_name);
		if (matrix&&jit_object_method(matrix, _jit_sym_class_jit_matrix)) {
			savelock = (long) jit_object_method(matrix,_jit_sym_lock,1);
			jit_object_method(matrix,_jit_sym_getinfo,&minfo);
			jit_object_method(matrix,_jit_sym_getdata,&bp);
			
			if ((!bp)||(x->plane>=minfo.planecount)||(x->plane<0)) { 
				jit_error_sym(x,_jit_sym_err_calculate);
				jit_object_method(matrix,_jit_sym_lock,savelock);
				goto out;
			}
			
			//max_jit_realsense_grab_offset_bp(x,&bp,&minfo);
			
			//limited to filling at most into 2 dimensions per list
			offset0 = (x->offsetcount>0)?x->offset[0]:0;
			offset1 = (x->offsetcount>1)?x->offset[1]:0;
			CLIP_ASSIGN(offset0,0,minfo.dim[0]-1);
			CLIP_ASSIGN(offset1,0,minfo.dim[1]-1);
			CLIP_ASSIGN(argc,0,(minfo.dim[0]*(minfo.dim[1]-offset1))-offset0);
			j = offset0 + offset1*minfo.dim[0];
					
			if (minfo.type==_jit_sym_char) {
				bp += x->plane;
				for (i=0;i<argc;i++,j++) {
					p = bp + (j/minfo.dim[0])*minfo.dimstride[1] + (j%minfo.dim[0])*minfo.dimstride[0];
					
					*((uchar *)p) = jit_atom_getcharfix(argv+i);
				}
			} else if (minfo.type==_jit_sym_long) {
				bp += x->plane*4;
				for (i=0;i<argc;i++,j++) {
					p = bp + (j/minfo.dim[0])*minfo.dimstride[1] + (j%minfo.dim[0])*minfo.dimstride[0];
					*((t_int32 *)p) = jit_atom_getlong(argv+i);
				}
			} else if (minfo.type==_jit_sym_float32) {
				bp += x->plane*4;
				for (i=0;i<argc;i++,j++) {
					p = bp + (j/minfo.dim[0])*minfo.dimstride[1] + (j%minfo.dim[0])*minfo.dimstride[0];
					*((float *)p) = jit_atom_getfloat(argv+i);
				}
			} if (minfo.type==_jit_sym_float64) {
				bp += x->plane*8;
				for (i=0;i<argc;i++,j++) {
					p = bp + (j/minfo.dim[0])*minfo.dimstride[1] + (j%minfo.dim[0])*minfo.dimstride[0];
					*((double *)p) = jit_atom_getfloat(argv+i);
				}
			}
							
			jit_object_method(matrix,_jit_sym_lock,savelock);				
		} else {
			jit_error_sym(x,_jit_sym_err_calculate);
		}
	}
out:
	outlet_bang(x->bangout);
	return;
}
/* old, incorrect way
void max_jit_realsense_grab_offset_bp(t_max_jit_realsense_grab *x, char **bp, t_jit_matrix_info *minfo)
{
	long i,dimcount,offset;
	
	dimcount = MIN(x->offsetcount,minfo->dimcount);
	for (i=0;i<dimcount;i++)
	{	
		offset = x->offset[i];
		CLIP_ASSIGN(offset,0,minfo->dim[i]-1);
		*bp += offset*minfo->dimstride[i];
	}
}			
*/
void max_jit_realsense_grab_assist(t_max_jit_realsense_grab *x, void *b, long m, long a, char *s)
{
	if (m == 1) {
		sprintf(s, "list");
	}
	else {
		if (a == 1)
			sprintf(s, "dumpout");
		else
			sprintf(s, "bang when done");
	}
}
	
void max_jit_realsense_grab_free(t_max_jit_realsense_grab *x)
{
	//only max object, no jit object
	x->pp->Release();
	max_jit_obex_free(x);
}

void *max_jit_realsense_grab_new(t_symbol *s, long argc, t_atom *argv)
{
	t_max_jit_realsense_grab *x;
	long attrstart;

	if (x = (t_max_jit_realsense_grab *)max_jit_obex_new( max_jit_realsense_grab_class ,NULL ) ) { //only max object, no jit object
		max_jit_obex_dumpout_set(x, outlet_new(x,0L)); //general purpose outlet(rightmost)

		x->pp = PXCSenseManager::CreateInstance();
		if (!x->pp) {
			post("Unable to create the SenseManager, Intel driver may not be installed?");
			return NULL;
		}


		x->bangout 	= outlet_new(x,0L); // after done, outputs bang
		
		x->matrix_name = _jit_sym_nothing;
		x->plane = 0;
		x->offsetcount = 0;

		attrstart = max_jit_attr_args_offset(argc,argv);
		if (attrstart&&argv) {
			t_atom_long al;
			jit_atom_arg_getsym(&x->matrix_name, 0, attrstart, argv);
			if (!jit_atom_arg_getlong(&al, 1, attrstart, argv)) {
				C74_ASSERT_FITS_LONG(al);
				x->plane = (long) al;
			}
		}
		
		max_jit_attr_args(x,argc,argv); //handle attribute args
	}
	return (x);
}
