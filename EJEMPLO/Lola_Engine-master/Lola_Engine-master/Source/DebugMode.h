#ifndef __DRAWDEBUG_H__
#define __DRAWDEBUG_H__

#include "MathGeoLib\MathGeoLib.h"
#include "OcTree.h"
#include "openGL.h"

#define LINE_WIDTH 2
#define DEFAULT_LINE_WIDTH 1

class DebugMode
{
private:

	math::float3 *edges;
	math::vec *corners;	

public:	

	bool show_aabb;
	bool show_frustum;
	bool show_octree;

	DebugMode();
	~DebugMode();

	void DrawAABB(const math::AABB &bbox) const;
	void DrawOBB(const math::OBB &bbox) const;
	void DrawFrustum(const math::Frustum &frustrum) const;
	void DrawOcTree(const OcTree &quad) const;
	void DrawLineSegment(const math::LineSegment &line) const;

	bool IsActive() const;

	template <class PRIMITIVE>
	void DrawOcTree(const OcTree &octree, const PRIMITIVE &primitive) const
	{
		if (show_octree)
		{
			std::vector<math::AABB> boxes_intersecting_primitive;
			std::vector<math::AABB> boxes;
			octree.CollectRects(boxes_intersecting_primitive, primitive);
			octree.CollectRects(boxes);

			glLineWidth(LINE_WIDTH);
			glBegin(GL_LINES);

			glColor3f(0.0f, 1.0f, 0.0f);

			for (uint i = 0; i < boxes_intersecting_primitive.size();++i)
			{
				boxes_intersecting_primitive[i].ToEdgeList(edges);

				for (int i = 0; i < 24; ++i)
					glVertex3fv(edges[i].ptr());
			}

			glColor3f(1.0f, 0.0f, 0.0f);

			for (uint i = 0; i < boxes.size();++i)
			{
				boxes[i].ToEdgeList(edges);

				for (int i = 0; i < 24; ++i)
					glVertex3fv(edges[i].ptr());
			}

			glColor3f(1.0f, 1.0f, 1.0f);

			glEnd();
			glLineWidth(DEFAULT_LINE_WIDTH);
		}
	}
};

#endif // __DRAWDEBUG_H__
