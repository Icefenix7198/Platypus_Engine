#include "ModuleFontManager.h"

#include "ResourceTexture.h"

ModuleFontManager::ModuleFontManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	sprintf_s(name, SHORT_STRING, "Font_Manager");
}

// Destructor
ModuleFontManager::~ModuleFontManager()
{}

// This Function Gets The First Power Of 2 >= The
// Int That We Pass It.
int next_p2(int a)
{
	int rval = 1;
	// rval<<=1 Is A Prettier Way Of Writing rval*=2;
	while (rval<a) rval <<= 1;
	return rval;
}

// Called before render is available
bool ModuleFontManager::Awake(JSONParser &config)
{
	bool ret = true;

	//int height = 16;

	//// Create And Initilize A FreeType Font Library.
	//if(FT_Init_FreeType(&library))
	//	DEBUG("FT_Init_FreeType failed");

	//// This Is Where We Load In The Font Information From The File.
	//// Of All The Places Where The Code Might Die, This Is The Most Likely,
	//// As FT_New_Face Will Fail If The Font File Does Not Exist Or Is Somehow Broken.
	//if (FT_New_Face(library, "OpenSans-Regular.ttf", 0, &face))
	//	DEBUG("FT_New_Face failed (there is probably a problem with your font file)");

	//// For Some Twisted Reason, FreeType Measures Font Size
	//// In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
	//// h Pixels High, We Need To Request A Size Of h*64.
	//// (h << 6 Is Just A Prettier Way Of Writing h*64)
	//FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);

	return ret;
}

// Called before the first frame
bool ModuleFontManager::Start()
{
	return true;
}

// Update all guis
UPDATE_STATUS ModuleFontManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called after all Updates
UPDATE_STATUS ModuleFontManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleFontManager::CleanUp()
{
	DEBUG("Freeing Font Manager");

	//// We Don't Need The Face Information Now That The Display
	//// Lists Have Been Created, So We Free The Assosiated Resources.
	//FT_Done_Face(face);

	//// Ditto For The Font Library.
	//FT_Done_FreeType(library);

	return true;
}

void ModuleFontManager::CreateTextureFromText(ResourceTexture *res)
{
	//// Load The Glyph For Our Character.
	//if (FT_Load_Glyph(face, FT_Get_Char_Index(face, 'X'), FT_LOAD_DEFAULT))  //
	//	DEBUG("FT_Load_Glyph failed");

	//FT_Render_Glyph(face->glyph, FT_Render_Mode::FT_RENDER_MODE_NORMAL);

	//// Move The Face's Glyph Into A Glyph Object.
	//FT_Glyph glyph;
	//if (FT_Get_Glyph(face->glyph, &glyph))
	//	DEBUG("FT_Get_Glyph failed");

	//// Convert The Glyph To A Bitmap.
	//FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	//FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//// This Reference Will Make Accessing The Bitmap Easier.
	//FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	//// Use Our Helper Function To Get The Widths Of
	//// The Bitmap Data That We Will Need In Order To Create
	//// Our Texture.
	//int width = next_p2(bitmap.width);
	//int height = next_p2(bitmap.rows);

	//// Allocate Memory For The Texture Data
	//unsigned char* expanded_data = new unsigned char[2 * width * height];
	//char *own_data = new char[width * height];

	//for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{
	//		own_data[(i + j * width)] = (i >= bitmap.width || j >= bitmap.rows) ? -128 : bitmap.buffer[i + bitmap.width * j] - 128;
	//	}
	//}

	////memcpy(own_data, bitmap.buffer, width * height);

	//// Here We Fill In The Data For The Expanded Bitmap.
	//// Notice That We Are Using A Two Channel Bitmap (One For
	//// Channel Luminosity And One For Alpha), But We Assign
	//// Both Luminosity And Alpha To The Value That We
	//// Find In The FreeType Bitmap.
	//// We Use The ?: Operator To Say That Value Which We Use
	//// Will Be 0 If We Are In The Padding Zone, And Whatever
	//// Is The FreeType Bitmap Otherwise.
	///*for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{
	//		expanded_data[2 * (i + j * width)] = 255;
	//		expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
	//	}
	//}*/

	//res->texture_data = own_data;
	//res->texture_size = width * height;
	//res->LoadToMemory();

	////MaterialImporter::Save(&expanded_data, 2 * width * height);

	//// Ditto For The Glyph.
	//FT_Done_Glyph(glyph);

}
