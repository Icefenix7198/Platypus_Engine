#include "aasimp.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "Application.h"
#include "ModuleScene.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

void aasimp::LoadTexture(const char* path)
{

	Texture* texture = nullptr;

	ILuint new_image_id = 0;
	ilGenImages(1, &new_image_id);
	ilBindImage(new_image_id);

	//Init DevIL to work with OpenGL
	/*if(!ilutRenderer(ILUT_OPENGL))
	{
		LOG("Error initializing DevIL for OpenGL")
	}*/
	//ilutRenderer(ILUT_OPENGL);

	if (ilLoadImage(path)) 
	{
		//iluFlipImage(); 
		std::string str;
		str.assign(path);
		/*ILubyte* textdata = ilGetData();*/
		texture = new Texture(str, ilutGLBindTexImage(), (int)ilGetInteger(IL_IMAGE_WIDTH), (int)ilGetInteger(IL_IMAGE_HEIGHT));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		//Assign how the texture will be managed / scaled/ things
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		vecTextures.push_back(texture);

		if(App->scene->selectedGO != App->scene->root)
		{
			auto go = App->scene->selectedGO;
			if(go->HasComponent(ComponentType::MATERIAL))
			{
				ComponentMaterial* cm = (ComponentMaterial*)go->GetComponentByType(ComponentType::MATERIAL);
				cm->textureBuffer = texture->id;
				cm->name = texture->name;
				cm->width = texture->width;
				cm->heigth = texture->heigth;
			}
			else
			{
				ComponentMaterial* cm = (ComponentMaterial*)go->CreateComponent(ComponentType::MATERIAL);
				cm->textureBuffer = texture->id;
			}
		}
		/*if (has_been_dropped && App->objects->GetSelectedObject() != nullptr) {
			ApplyTextureToSelectedObject(texture);
		}*/

		LOG("Texture successfully loaded: %s", path);
	}
	else {
		LOG("Error while loading image in %s", path);
		LOG("Error: %s", ilGetString(ilGetError()));
	}

	//Clean memory
	ilDeleteImages(1, &new_image_id);

}


