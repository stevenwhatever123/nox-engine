#include <GridObject.h>

using namespace NoxEngine;
using NoxEngineUtils::Logger;

GridObject::~GridObject()
{

}

GridObject::GridObject(vec3 start_point, vec3 end_point, const i32 count_point)
{

	glRenderType = GL_LINES;
	has_texture = 1;
	has_normal  = 1;
	use_indices = 1;

	x = start_point.x;
	y = start_point.y;
	z = start_point.z;

	f32 delta_distance_x = (end_point.x - start_point.x)/count_point;
	f32 delta_distance_z = (end_point.z - start_point.z)/count_point;


	vertices.resize((4*count_point - 4));
	texCoords.resize((4*count_point - 4));
	normals.resize((4*count_point - 4));


	for(i32 i = 0; i < vertices.size(); i++) {
		vertices[i].x = 0.0f;
		vertices[i].y = 0.0f;
		vertices[i].z = 0.0f;
	}

	// two rows, top and bottom

	i32 first_row_base_index = 0;
	i32 last_row_base_index  = first_row_base_index + count_point;
	i32 first_col_base_index = last_row_base_index +  count_point;
	i32 last_col_base_index  = first_col_base_index + count_point - 2;

	i32 last_index = count_point - 1;
	i32 col_counter = 0;

	for(i32 i = 0; i < count_point; i++) {

		vertices[first_row_base_index + i].x = x + i*delta_distance_x;
		vertices[first_row_base_index + i].y = 0;
		vertices[first_row_base_index + i].z = z;

		vertices[last_row_base_index + i].x = x + i*delta_distance_x;
		vertices[last_row_base_index + i].y = 0;
		vertices[last_row_base_index + i].z = z + (count_point - 1)*delta_distance_z;

		if(i > 0 && i < last_index) {
			vertices[first_col_base_index + col_counter].x = x;
			vertices[first_col_base_index + col_counter].y = 0;
			vertices[first_col_base_index + col_counter].z = z + i*delta_distance_z;

			// // right column
			vertices[last_col_base_index + col_counter].x = x + (count_point-1)*delta_distance_x;
			vertices[last_col_base_index + col_counter].y = 0;
			vertices[last_col_base_index + col_counter].z = z + i*delta_distance_z;

			col_counter++;
		}

	}

	indices.resize((count_point*4));

	for(i32 i = 0; i < count_point*4; i++) {
		indices[i] = 0;
	}

	glRenderType = GL_LINES;

	col_counter = 0;
	i32 fr_index = 0;
	i32 lr_index = fr_index + count_point;
	i32 fc_index = lr_index + count_point;
	i32 lc_index = fc_index + count_point - 2;

	i32 row_index = fc_index;
	i32 row_sec_index = lc_index;


	indices[0]   = fr_index;
	indices[1]   = fr_index + last_index;

	indices[(count_point*2) - 2 + 0] = lr_index;
	indices[(count_point*2) - 2+ 1]  = lr_index + last_index;

	i32 index = 0;
	for(i32 i = 0; i < (count_point - 2); i++, index += 2) {
		indices[2 + index + 0] = fc_index + i;
		indices[2 + index + 1] = lc_index + i;
	}

	index = 0;
	for(i32 i = 0; i < count_point; i++, index += 2) {
		indices[(count_point*2) + index + 0] = fr_index + i;
		indices[(count_point*2) + index + 1] = lr_index + i;
	}
}

