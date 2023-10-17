#include "JSONParser.h"

#include <stdio.h>
#include <stdlib.h>

JSONParser::JSONParser()
{
	value_root = json_value_init_object();
	root = json_value_get_object(value_root);
}

JSONParser::JSONParser(JSON_Object* node) : root(node)
{ }

JSONParser::JSONParser(const char *string)
{
	value_root = json_parse_string(string);
	root = json_value_get_object(value_root);
}

JSONParser::~JSONParser()
{
	if (value_root != nullptr)
		json_value_free(value_root);
}

void JSONParser::ParseBuffer(char *buffer)
{
	if (buffer != nullptr)
	{
		value_root = json_parse_string(buffer);
		root = json_value_get_object(value_root);
	}	
}

// SETS
JSONParser JSONParser::AddNode(const char *node_name)
{
	json_object_set_value(root, node_name, json_value_init_object());
	return GetNode(node_name);
}

bool JSONParser::AddArray(const char *name_array)
{
	JSON_Value *v = json_value_init_array();
	array_root = json_value_get_array(v);

	return json_object_set_value(root, name_array, v) == JSONSuccess;
}

bool JSONParser::AddArray(const JSONParser &block)
{
	if(array_root)
		return json_array_append_value(array_root, json_value_deep_copy(block.value_root)) == JSONSuccess;
	return false;
}

bool JSONParser::AddArray(const char *name_array, const JSONParser &block)
{
	if (AddArray(name_array))
		return AddArray(block);
	return false;
}

bool JSONParser::AddBoolean(const char *name_boolean, bool boolean)
{
	return json_object_set_boolean(root, name_boolean, boolean) == JSONSuccess;
}

bool JSONParser::AddInt(const char *name_int, int value)
{
	return json_object_set_number(root, name_int, (int)value) == JSONSuccess;
}

bool JSONParser::AddUUID(const char *uuid_name, long unsigned int value)
{
	char uuid[20]; sprintf_s(uuid, 20, "%lu", value);
	return json_object_set_string(root, uuid_name, uuid) == JSONSuccess;
}

bool JSONParser::AddFloat(const char *float_name, float value)
{
	return json_object_set_number(root, float_name, value) == JSONSuccess;
}

bool JSONParser::AddString(const char *string_name, const char *string_value)
{
	return json_object_set_string(root, string_name, string_value) == JSONSuccess;
}

// GETS
JSONParser JSONParser::GetNode(const char *name_node) const
{
	return JSONParser(json_object_get_object(root, name_node));
}

JSONParser JSONParser::GetArray(const char *array_name, int index) const
{
	JSON_Array* array = json_object_get_array(root, array_name);
	if (array != nullptr)
		return JSONParser(json_array_get_object(array, index));
	return JSONParser((JSON_Object*) nullptr);
}

int JSONParser::GetArrayCount(const char *array_name) const
{
	int size = 0;
	JSON_Array *array = json_object_get_array(root, array_name);
	size = json_array_get_count(array);
	return size;	
}

bool JSONParser::GetBoolean(const char *name_boolean) const
{
	if (ValueExists(name_boolean))
		return (json_object_get_boolean(root, name_boolean) != 0) ? true : false;
	else
		return false; 
}

int JSONParser::GetInt(const char *int_name) const
{
	return (int)json_object_get_number(root, int_name);
}

long unsigned int JSONParser::GetUUID(const char *uuid_name) const
{
	if(json_object_has_value(root, uuid_name) != 0)
		return strtoul(json_object_get_string(root, uuid_name), nullptr, 0);
	return 0;
}

float JSONParser::GetFloat(const char *float_name) const
{
	return json_object_get_number(root, float_name);
}

const char *JSONParser::GetString(const char *string_name) const
{
	return (json_object_get_string(root, string_name));
}

// Buffer must be deleted calling FreeBuffer(char **buff)
void JSONParser::Save(char **buff)
{	
	size_t bytes_size = json_serialization_size(value_root);
	*buff = new char[bytes_size];
	*buff = json_serialize_to_string_pretty(value_root);
	//json_serialize_to_buffer_pretty(value_root, *buff, bytes_size);
}

// Utilities
bool JSONParser::ValueExists(const char *node_name) const
{
	return (json_object_has_value(root, node_name) != 0) ? true : false;
}

void JSONParser::CreateResourceEntry(const char *filename, const char *imported_filename, long unsigned int id, RESOURCE_TYPE type)
{
	AddString("File", filename);
	AddString("Imported File", imported_filename);
	AddInt("Type", type);
	AddUUID("ID", id);	
}


void JSONParser::FreeBuffer(char **buff)
{
	if (*buff != nullptr)
	{
		delete[] *buff;
		*buff = nullptr;
	}
}
