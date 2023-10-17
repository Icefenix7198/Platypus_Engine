#include "DebugMode.h"

#include "Globals.h"
#include <stack>

DebugMode::DebugMode()
{
	edges = new math::float3[24];
	corners = new math::float3[8];

	show_aabb = false;
	show_frustum = false;
	show_octree = false;
}

DebugMode::~DebugMode()
{
	RELEASE_ARRAY(edges);
	RELEASE_ARRAY(corners);
}

bool DebugMode::IsActive() const
{
	return show_aabb || show_frustum || show_octree;
}

void DebugMode::DrawAABB(const math::AABB &bbox) const
{
	if (show_aabb)
	{
		bbox.ToEdgeList(edges);

		glLineWidth(LINE_WIDTH);
		glBegin(GL_LINES);
		
		glColor3f(0.0f, 1.0f, 0.0f);

		for (int i = 0; i < 24; ++i)
			glVertex3fv(edges[i].ptr());

		glColor3f(1.0f, 1.0f, 1.0f);		

		glEnd();
		glLineWidth(DEFAULT_LINE_WIDTH);
	}	
}

void DebugMode::DrawOBB(const math::OBB &bbox) const
{
	bbox.ToEdgeList(edges);

	glLineWidth(LINE_WIDTH);
	glBegin(GL_LINES);
	
	glColor3f(0.5f, 1.0f, 0.5f);

	for (int i = 0; i < 24; ++i)
		glVertex3fv(edges[i].ptr());

	glColor3f(1.0f, 1.0f, 1.0f);	

	glEnd();
	glLineWidth(DEFAULT_LINE_WIDTH);
}

void DebugMode::DrawFrustum(const math::Frustum &frustrum) const
{
	if (show_frustum)
	{
		frustrum.GetCornerPoints(corners);

		glLineWidth(LINE_WIDTH);
		glBegin(GL_LINES);
		
		glColor3f(0.0f, 0.5f, 1.0f);

		// Near plane
		glVertex3fv(corners[0].ptr());
		glVertex3fv(corners[1].ptr());
		glVertex3fv(corners[0].ptr());
		glVertex3fv(corners[2].ptr());
		glVertex3fv(corners[2].ptr());
		glVertex3fv(corners[3].ptr());
		glVertex3fv(corners[1].ptr());
		glVertex3fv(corners[3].ptr());

		// Far plane
		glVertex3fv(corners[4].ptr());
		glVertex3fv(corners[5].ptr());
		glVertex3fv(corners[4].ptr());
		glVertex3fv(corners[6].ptr());
		glVertex3fv(corners[6].ptr());
		glVertex3fv(corners[7].ptr());
		glVertex3fv(corners[5].ptr());
		glVertex3fv(corners[7].ptr());

		// Left plane
		glVertex3fv(corners[1].ptr());
		glVertex3fv(corners[5].ptr());
		glVertex3fv(corners[3].ptr());
		glVertex3fv(corners[7].ptr());

		// Right plane
		glVertex3fv(corners[0].ptr());
		glVertex3fv(corners[4].ptr());
		glVertex3fv(corners[2].ptr());
		glVertex3fv(corners[6].ptr());

		glColor3f(1.0f, 1.0f, 1.0f);		

		glEnd();
		glLineWidth(DEFAULT_LINE_WIDTH);
	}	
}

void DebugMode::DrawOcTree(const OcTree &octree) const
{	
	if (show_octree)
	{
		std::vector<math::AABB> boxes;
		octree.CollectRects(boxes);

		glLineWidth(LINE_WIDTH);
		glBegin(GL_LINES);
		
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

void DebugMode::DrawLineSegment(const math::LineSegment &line) const
{
	glLineWidth(LINE_WIDTH);
	glBegin(GL_LINES);
	
	glColor3f(1.0f, 0.7f, 1.0f);

	glVertex3fv(&line.a.x);
	glVertex3fv(&line.b.x);

	glColor3f(1.0f, 1.0f, 1.0f);	

	glEnd();
	glLineWidth(DEFAULT_LINE_WIDTH);

}

