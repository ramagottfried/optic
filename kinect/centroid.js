
var mat = new JitterMatrix();

function calc_centroid(mat)
{
	var nCells = mat.dim[0] * mat.dim[1];
	
	var np = mat.planecount;
	var centroid = new Array(np);
	centroid = [0,0,0];

	for( var i = 0; i < mat.dim[0]; i++)
	{
		for( var j = 0; j < mat.dim[1]; j++)
		{
			var planes = mat.getcell(i,j);
			for( var p = 0; p < np; p++)
			{
				centroid[p] += planes[p];
			}
		}
	}

	for( var p = 0; p < np; p++)
	{
		centroid[p] /= nCells;
	}
	
	return centroid;
}

function jit_matrix(name)
{
	mat.name = name;
	
	var centroid = calc_centroid(mat);
	/*	
	for( var i = 0; i < mat.dim[0]; i++)
	{
		for( var j = 0; j < mat.dim[1]; j++)
		{
			var planes = mat.getcell(i,j);
			for( var p = 0; p < np; p++)
			{
				centroid[p] += planes[p];
			}
		}
	}
*/
		outlet(0, centroid);
}