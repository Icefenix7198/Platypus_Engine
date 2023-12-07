#include "JSONHelper.h"
#include "Globals.h"
#include "Application.h"

//Internal use only
void JSONHelper::PopulateArray(JSON_Value* _array, float* value, unsigned int size)
{
	JSON_Array* jsArray = json_value_get_array(_array);
	for (unsigned int i = 0; i < size; i++)
	{
		json_array_append_number(jsArray, value[i]);
	}

}
void JSONHelper::WriteFloat(JSON_Object* obj, const char* name, float value)
{
	json_object_set_number(obj, name, value);
}
float JSONHelper::ReadFloat(JSON_Object* obj, const char* name)
{
	return json_object_get_number(obj, name);
}
void JSONHelper::WriteString(JSON_Object* obj, const char* name, const char* value)
{
	json_object_set_string(obj, name, value);
}
const char* JSONHelper::ReadString(JSON_Object* obj, const char* name)
{
	return json_object_get_string(obj, name);
}
void JSONHelper::WriteBool(JSON_Object* obj, const char* name, bool value)
{
	json_object_set_boolean(obj, name, value);
}
bool JSONHelper::ReadBool(JSON_Object* obj, const char* name)
{
	return json_object_get_boolean(obj, name);
}
void JSONHelper::WriteInt(JSON_Object* obj, const char* name, int value)
{
	json_object_set_number(obj, name, value);
}
int JSONHelper::ReadInt(JSON_Object* obj, const char* name)
{
	return json_object_get_number(obj, name);
}
void JSONHelper::WriteVector3(JSON_Object* obj, const char* name, float* value)
{
	JSON_Value* goArray = json_value_init_array();
	PopulateArray(goArray, value, 3);
	json_object_set_value(obj, name, goArray);
}
float3 JSONHelper::ReadVector3(JSON_Object* obj, const char* name)
{
	JSON_Array* vecArray = json_object_dotget_array(obj, name);
	return float3(json_array_get_number(vecArray, 0), json_array_get_number(vecArray, 1), json_array_get_number(vecArray, 2));
}
void JSONHelper::WriteQuaternion(JSON_Object* obj, const char* name, float* value)
{
	JSON_Value* goArray = json_value_init_array();
	PopulateArray(goArray, value, 4);
	json_object_set_value(obj, name, goArray);
}

//TODO: Make some safer code and remove duplicated code on readvector and readquat
Quat JSONHelper::ReadQuaternion(JSON_Object* obj, const char* name)
{
	JSON_Array* vecArray = json_object_dotget_array(obj, name);
	return Quat(json_array_get_number(vecArray, 0), json_array_get_number(vecArray, 1), json_array_get_number(vecArray, 2), json_array_get_number(vecArray, 3));
}

void JSONHelper::CreateMetaModel(const char* filePath)
{
	//JSON_Value* root_value = json_value_init_object();
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);
	
	//If there is no failure loading
	if (root_value != nullptr && root_object != nullptr)
	{	
		char* serialized_string = NULL;
		//Crear path
		json_object_set_string(root_object, "FilePath", filePath);
		json_object_set_string(root_object, "name", "John Smith");
		json_object_set_number(root_object, "UUID", 25);
		//Dot set hace que si lo pones en un punto te lo ponga dentro de un {} del punto antes del 
		json_object_dotset_string(root_object, "address.city", "Cupertino");
		//Jason_parse_string lo mete en un array
		json_object_dotset_value(root_object, "contact.emails", json_parse_string("[\"email@example.com\",\"email2@example.com\"]"));
		serialized_string = json_serialize_to_string_pretty(root_value);
		puts(serialized_string);
		std::string nameMeta;
		nameMeta += ASSETS_MODELS;
		nameMeta += App->fileSystem->GetNameFromPath(filePath, false);
		nameMeta += ".meta";
		json_serialize_to_file(root_value, nameMeta.c_str());
		json_free_serialized_string(serialized_string);
		json_value_free(root_value);
	}
}

Config::Config(JSON_Object* _nObj)
{
	nObj = _nObj;
}

// ---------------------------------------------------------------------------------- //
//Internal use only
void Config::PopulateArray(JSON_Value* _array, float* value, unsigned int size)
{
	JSON_Array* jsArray = json_value_get_array(_array);
	for (unsigned int i = 0; i < size; i++)
	{
		json_array_append_number(jsArray, value[i]);
	}

}


void Config::WriteFloat(const char* name, float value)
{
	json_object_set_number(nObj, name, value);
}
float Config::ReadFloat(const char* name)
{
	return json_object_get_number(nObj, name);
}
void Config::WriteString(const char* name, const char* value)
{
	json_object_set_string(nObj, name, value);
}
const char* Config::ReadString(const char* name)
{
	return json_object_get_string(nObj, name);
}
void Config::WriteBool(const char* name, bool value)
{
	json_object_set_boolean(nObj, name, value);
}
bool Config::ReadBool(const char* name)
{
	return json_object_get_boolean(nObj, name);
}
void Config::WriteInt(const char* name, int value)
{
	json_object_set_number(nObj, name, value);
}

int Config::ReadInt(const char* name)
{
	return json_object_get_number(nObj, name);
}

void Config::WriteVector3(const char* name, float* value)
{
	JSON_Value* goArray = json_value_init_array();
	PopulateArray(goArray, value, 3);
	json_object_set_value(nObj, name, goArray);
}
float3 Config::ReadVector3(const char* name)
{
	JSON_Array* vecArray = json_object_dotget_array(nObj, name);
	return float3(json_array_get_number(vecArray, 0), json_array_get_number(vecArray, 1), json_array_get_number(vecArray, 2));
}
void Config::WriteQuat(const char* name, float* value)
{
	JSON_Value* goArray = json_value_init_array();
	PopulateArray(goArray, value, 4);
	json_object_set_value(nObj, name, goArray);
}

//TODO: Make some safer code and remove duplicated code on readvector and readquat
Quat Config::ReadQuat(const char* name)
{
	JSON_Array* vecArray = json_object_dotget_array(nObj, name);
	return Quat(json_array_get_number(vecArray, 0), json_array_get_number(vecArray, 1), json_array_get_number(vecArray, 2), json_array_get_number(vecArray, 3));
}

JSON_Array* Config::ReadArray(const char* name)
{
	return json_object_get_array(nObj, name);
}

JSON_Object* Config::ReadObject(const char* name)
{
	return json_object_get_object(nObj, name);
}

JSON_Value* Config::ReadValue(const char* name)
{
	return json_object_get_value(nObj, name);
}