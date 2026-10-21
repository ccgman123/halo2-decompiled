// @flags /O2 /GL-
// Havok (third-party physics library): building triangle faces on a hull, a
// fan of new faces from an apex over a loop of horizon edges, and the four
// faces of the starting tetrahedron.
#include <xmmintrin.h>
#include "reference_counted_2d88a0.h"

struct edge_30aac0;
struct face_30aac0;

// a hull vertex: its position, and a horizon edge leaving it
struct vertex_30ac50
{
	__m128 position;
	edge_30aac0 *horizon_edge;
	char unknown14[0xc];
};

struct edge_30aac0
{
	vertex_30ac50 const *vertex;
	edge_30aac0 *next;
	edge_30aac0 *twin;
	face_30aac0 *face;
};

struct face_30aac0
{
	__m128 normal;
	float distance;
	edge_30aac0 edges[3];
	char mark;

	void function_30a9d0();
};

struct face_list_30ac50
{
	face_30aac0 **faces;
	int count;
};

// one edge of the starting tetrahedron, as byte offsets: of its vertex in
// the vertex array, and of its twin's face and of the twin within that face
struct tetrahedron_edge_30ad20
{
	long vertex_offset;
	long twin_face_offset;
	long twin_edge_offset;
};

// vertex, face and edge sizes are 0x20, 0x50 and 0x10 bytes
static tetrahedron_edge_30ad20 const g_tetrahedron_edges_413700[12] =
{
	{0 * 0x20, 2 * 0x50, 2 * 0x10},
	{2 * 0x20, 3 * 0x50, 0 * 0x10},
	{1 * 0x20, 1 * 0x50, 0 * 0x10},
	{0 * 0x20, 0 * 0x50, 2 * 0x10},
	{1 * 0x20, 3 * 0x50, 2 * 0x10},
	{3 * 0x20, 2 * 0x50, 0 * 0x10},
	{0 * 0x20, 1 * 0x50, 2 * 0x10},
	{3 * 0x20, 3 * 0x50, 1 * 0x10},
	{2 * 0x20, 0 * 0x50, 0 * 0x10},
	{1 * 0x20, 0 * 0x50, 1 * 0x10},
	{2 * 0x20, 2 * 0x50, 1 * 0x10},
	{3 * 0x20, 1 * 0x50, 1 * 0x10},
};

class class_30a810
{
public:
	char unknown0[0x1b0];
	face_30aac0 *faces;
	int face_count;
	char unknown1b8[0x2100 - 0x1b8];
	vertex_30ac50 *vertices;
	int vertex_count;

	face_30aac0 *faces_end()
	{
		return faces + face_count;
	}

	void clear_marks();
	int function_30ac50(face_list_30ac50 const *new_faces, void const *unused, edge_30aac0 *horizon, vertex_30ac50 const *const *apex);
	void function_30ad20();
	int function_30ae70(long count, edge_30aac0 *horizon, __m128 const *position);
};

// each new face gets one horizon edge, the next horizon edge's start and the
// apex; its sides are joined to the neighbouring new faces. Returns 2 when
// the horizon runs out or doesn't close back on its first edge.
// @retail 0x30ac50
int class_30a810::function_30ac50(face_list_30ac50 const *new_faces, void const *unused, edge_30aac0 *horizon, vertex_30ac50 const *const *apex)
{
	int result = 0;
	edge_30aac0 *edge = horizon;
	for (int i = 0; i < new_faces->count; i++)
	{
		if (edge == 0)
		{
			result = 2;
			break;
		}
		face_30aac0 *face = new_faces->faces[i];
		face_30aac0 *following = i + 1 < new_faces->count ? new_faces->faces[i + 1] : new_faces->faces[0];
		face->edges[0].face = face;
		face->edges[1].face = face;
		face->edges[2].face = face;
		face->edges[1].next = &face->edges[2];
		face->edges[2].next = &face->edges[0];
		face->edges[0].next = &face->edges[1];
		face->edges[0].vertex = edge->next->vertex;
		face->edges[1].vertex = edge->vertex;
		face->edges[2].vertex = *apex;
		edge->twin = &face->edges[0];
		face->edges[0].twin = edge;
		following->edges[1].twin = &face->edges[2];
		face->edges[2].twin = &following->edges[1];
		face->function_30a9d0();
		vertex_30ac50 *next_vertex = (vertex_30ac50 *)edge->next->vertex;
		edge = next_vertex->horizon_edge;
		next_vertex->horizon_edge = 0;
	}
	clear_marks();
	return horizon != edge ? 2 : result;
}

// the four faces over the first four vertices, wound outwards: when the
// first vertex lies in front of the last face, three corners swap vertices
// the faces and the face count are read through references, so that the end
// of the loop is built from the members again on every pass
// @retail 0x30ad20
void class_30a810::function_30ad20()
{
	face_30aac0 *const first_face = faces;
	face_30aac0 *const &base = faces;
	int &count = face_count;
	face_count = 4;
	tetrahedron_edge_30ad20 const *entry = g_tetrahedron_edges_413700;
	for (face_30aac0 *face = first_face; face != base + count; face++)
	{
		edge_30aac0 *const last = face->edges + 3;
		face->edges[0].next = &face->edges[1];
		face->edges[1].next = &face->edges[2];
		face->mark = 0;
		face->edges[2].next = &face->edges[0];
		for (edge_30aac0 *edge = face->edges; edge < last; edge++, entry++)
		{
			edge->face = face;
			edge->vertex = (vertex_30ac50 const *)(entry->vertex_offset + (char *)vertices);
			edge->twin = (edge_30aac0 *)((char *)faces->edges + entry->twin_face_offset + entry->twin_edge_offset);
		}
	}
	faces[3].function_30a9d0();
	vertex_30ac50 const *corners = vertices;
	face_30aac0 *tetrahedron = faces;
	__m128 const offset = _mm_mul_ps(_mm_sub_ps(corners[0].position, corners[1].position), tetrahedron[3].normal);
	float side;
	_mm_store_ss(&side, _mm_add_ss(_mm_shuffle_ps(offset, offset, 0xaa), _mm_add_ss(_mm_shuffle_ps(offset, offset, 0x55), offset)));
	if (side > 0.0f)
	{
		tetrahedron[2].edges[0].vertex = &corners[1];
		tetrahedron[1].edges[0].vertex = &corners[1];
		tetrahedron[0].edges[0].vertex = &corners[1];
		vertex_30ac50 const *const first_vertex = vertices;
		tetrahedron[3].edges[0].vertex = first_vertex;
		tetrahedron[1].edges[1].vertex = first_vertex;
		tetrahedron[0].edges[2].vertex = first_vertex;
	}
	face_30aac0 *face = faces;
	for (int count = 4; count != 0; count--)
	{
		face->function_30a9d0();
		face++;
	}
}

struct list_30a950;

class pool_30a950
{
public:
	void function_30a950(list_30a950 *out, long count);
};

// a list of faces that starts out in the room for 100 held inside it
struct new_faces_30ae70
{
	face_30aac0 **data;
	int size;
	long capacity_and_flags;
	face_30aac0 *embedded[100];

	new_faces_30ae70()
	{
		data = embedded;
		size = 0;
		capacity_and_flags = 0x80000064;
	}
	~new_faces_30ae70()
	{
		if ((capacity_and_flags & 0x80000000) == 0)
		{
			g_memory_2d88a0->release_block(data, (capacity_and_flags & 0x7fffffff) * sizeof(face_30aac0 *), 0x12);
		}
	}
};

// adds a vertex at the position, and a fan of new faces from it over the
// horizon; the object starts with the pool the faces come from
// @retail 0x30ae70
int class_30a810::function_30ae70(long count, edge_30aac0 *horizon, __m128 const *position)
{
	int index = vertex_count++;
	vertex_30ac50 *apex = &vertices[index];
	apex->position = *position;
	apex->horizon_edge = 0;
	new_faces_30ae70 new_faces;
	((pool_30a950 *)this)->function_30a950((list_30a950 *)&new_faces, count);
	return function_30ac50((face_list_30ac50 const *)&new_faces, (void const *)count, horizon, &apex);
}
