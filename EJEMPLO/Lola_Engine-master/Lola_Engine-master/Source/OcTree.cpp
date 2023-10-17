#include "OcTree.h"

bool Intersects(const GameObject *a, const math::AABB &b_bbox)
{
	math::AABB a_bbox;
	if (a->GetAABB(a_bbox))
		return a_bbox.Intersects(b_bbox);

	return false;
}

// ---- OCTREE NODE ----

OcTreeNode::OcTreeNode(math::AABB bbox) : box(bbox)
{
	for (int i = 0; i < 8; ++i)
		childs[i] = nullptr;
}

OcTreeNode::~OcTreeNode()
{
	if (HasChildren())
	{
		for (int i = 0; i < 8; ++i)
			delete childs[i];
	}	
}

bool OcTreeNode::Insert(GameObject* go)
{
	bool item_inserted = false;

	//Whether all children share this gameobject, it's directly inserted
	/*if (SharedByMoreThanXChild(go, 8))
	{
		objects.push_back(go);
		item_inserted = true;		
	}*/
	/*else if (HasChildren())
	{
		for (int i = 0; i < 8; ++i)
		{
			if (childs[i]->Insert(go))
			{
				item_inserted = true;
				break;
			}				
		}
	}*/
	//else 
	if(Intersects(go, box))
	{
		if (objects.size() < OCTREE_MAX_ITEMS || SharedByMoreThanXChild(go, 2))
		{
			objects.push_back(go);
			item_inserted = true;
		}
		else
		{
			if (!HasChildren())
			{
				SubdivideNode();

				// Items on this node will be moved downwards if they are not shared by more than one child.
				for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
				{
					if (!SharedByMoreThanXChild(*it, 2))
					{
						for (int i = 0; i < 8; ++i)
						{
							if (childs[i]->Insert(*it))
								break;
						}

						it = objects.erase(it);
					}					
					else
						++it;
				}
			}			

			// Now, it's time to insert the GO.
			for (int i = 0; i < 8; ++i)
			{
				if (childs[i]->Insert(go))
				{
					item_inserted = true;
					break;
				}
			}
		}
	}	

	return item_inserted;
}

bool OcTreeNode::SharedByMoreThanXChild(GameObject *go, unsigned int X) const
{
	math::AABB go_box;
	unsigned int shared_childs = 0;

	if (HasChildren() && go->GetAABB(go_box))
	{
		for (int i = 0; i < 8; ++i)
		{
			if (childs[i]->box.Intersects(go_box))
				++shared_childs;
		}
	}

	return (shared_childs >= X) ? true : false;
}

int OcTreeNode::CenterOnChild(GameObject *go) const
{
	math::AABB bbox;
	if (HasChildren() && go->GetAABB(bbox))
	{
		for (int i = 0; i < 8; ++i)
		{
			if (childs[i]->box.Contains(bbox.CenterPoint()))
				return i;
		}
	}

	return -1;
}

void OcTreeNode::CollectRects(std::vector<math::AABB> &boxes)
{
	boxes.push_back(box);

	if (HasChildren())
	{
		for (int i = 0; i < 8; ++i)
			childs[i]->CollectRects(boxes);
	}	
}

bool OcTreeNode::HasChildren() const
{
	return childs[0] != nullptr;
}

void OcTreeNode::SubdivideNode()
{
	math::vec half_size = (box.maxPoint - box.minPoint) / 2.0f;
	math::AABB initial_bbox = math::AABB(box.minPoint, { box.minPoint.x + half_size.x, box.minPoint.y + half_size.y, box.minPoint.z + half_size.z });

	uint index = 0;

	for (int dir_x = 0; dir_x < 2; ++dir_x)
	{
		for (int dir_y = 0; dir_y < 2; ++dir_y)
		{
			for (int dir_z = 0; dir_z < 2; ++dir_z)
			{
				math::AABB sub_cube = initial_bbox;
				sub_cube.Translate({ half_size.x * dir_x, half_size.y * dir_y, half_size.z * dir_z });
				childs[index++] = new OcTreeNode(sub_cube);
			}
		}
	}
}

// ---- OCTREE ----

OcTree::OcTree() : root(nullptr)
{}

OcTree::~OcTree()
{
	Clear();
}

void OcTree::SetBoundaries(const math::AABB &r)
{
	if (root != nullptr)
		delete root;

	root = new OcTreeNode(r);
}

bool OcTree::Insert(GameObject* go)
{
	if (root != nullptr)
	{
		math::AABB bbox;
		if (go->GetAABB(bbox) && root->box.Intersects(bbox))
			return root->Insert(go);
	}
	return false;
}

void OcTree::Clear()
{
	if (root != nullptr)
	{
		delete root;
		root = nullptr;
	}
}

void OcTree::CollectRects(std::vector<math::AABB> &boxes) const
{
	if (root != nullptr)
		root->CollectRects(boxes);
}