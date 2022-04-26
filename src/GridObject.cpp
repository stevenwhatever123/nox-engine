#include <GridObject.h>

using namespace NoxEngine;
using NoxEngineUtils::Logger;

GridObject::GridObject(vec3 start_point, vec3 end_point, const i32 count_point) : points(), normals(), texCoords()
{

	glRenderType = GL_LINES;
	// has_texture = 1;
	// has_normal = 1;

	x = start_point.x;
	y = start_point.y;
	z = start_point.z;

	f32 delta_distance_x = (end_point.x - start_point.x)/count_point;
	f32 delta_distance_z = (end_point.z - start_point.z)/count_point;


	points.resize(3*(4*count_point - 4));
	texCoords.resize(2*count_point*count_point);
	normals.resize(3*count_point*count_point);


	for(i32 i = 0; i < points.size(); i++) {
		points[i] = 0.0f;
	}

	// for(i32 i = 0; i < count_point; i++) {
	// 	f32 x = start_point.x;
	// 	f32 z = start_point.z + i*delta_distance_z;
	// 	for(i32 j = 0; j < count_point; j++ ) {
	// 		points[i*3*count_point + j*3 + 0] = x + j*delta_distance_x;
	// 		points[i*3*count_point + j*3 + 1] = 0;
	// 		points[i*3*count_point + j*3 + 2] = z;
	// 	}
	// }

	// two rows, top and bottom

	i32 first_row_base_index = 0;
	i32 last_row_base_index  = first_row_base_index + 3*(count_point);
	i32 first_col_base_index = last_row_base_index +  3*(count_point);
	i32 last_col_base_index  = first_col_base_index + 3*(count_point - 2);

	i32 last_index = count_point - 1;
	i32 col_counter = 0;

	for(i32 i = 0; i < count_point; i++) {

		points[first_row_base_index + i*3] = x + i*delta_distance_x;
		points[first_row_base_index + i*3 + 1] = 0;
		points[first_row_base_index + i*3 + 2] = z;

		points[last_row_base_index + i*3] = x + i*delta_distance_x;
		points[last_row_base_index + i*3 + 1] = 0;
		points[last_row_base_index + i*3 + 2] = z + (count_point - 1)*delta_distance_z;

		if(i > 0 && i < last_index) {
			points[first_col_base_index +     3*col_counter] = x;
			points[first_col_base_index + 1 + 3*col_counter] = 0;
			points[first_col_base_index + 2 + 3*col_counter] = z + i*delta_distance_z;

			// // right column
			points[last_col_base_index + 3*col_counter] = x + (count_point-1)*delta_distance_x;
			points[last_col_base_index + 3*col_counter + 1] = 0;
			points[last_col_base_index + 3*col_counter + 2] = z + i*delta_distance_z;

			col_counter++;
		}

	}

	elements.resize(count_point*4);

	for(i32 i = 0; i < count_point*4; i++) {
		elements[i] = 0;
	}

	glRenderType = GL_LINES;

	col_counter = 0;
	i32 fr_index = 0;
	i32 lr_index = fr_index + count_point;
	i32 fc_index = lr_index + count_point;
	i32 lc_index = fc_index + count_point - 2;

	i32 row_index = fc_index;
	i32 row_sec_index = lc_index;


	elements[0]   = fr_index;
	elements[1]   = fr_index + last_index;

	elements[(count_point*2) - 2 + 0] = lr_index;
	elements[(count_point*2) - 2+ 1]  = lr_index + last_index;

	i32 index = 0;
	for(i32 i = 0; i < (count_point - 2); i++, index += 2) {
		elements[2 + index + 0] = fc_index + i;
		elements[2 + index + 1] = lc_index + i;
	}

	index = 0;
	for(i32 i = 0; i < count_point; i++, index += 2) {
		elements[(count_point*2) + index + 0] = fr_index + i;
		elements[(count_point*2) + index + 1] = lr_index + i;
	}

}
