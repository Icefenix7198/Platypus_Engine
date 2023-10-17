#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__

#include "Parson\parson.h"

#include "MathGeoLib\MathGeoLib.h"

enum RESOURCE_TYPE;

class JSONParser
{

private:

	JSON_Value *value_root = nullptr;
	JSON_Object *root = nullptr;
	JSON_Array *array_root = nullptr;

public:

	JSONParser();
	JSONParser(JSON_Object* node);
	JSONParser(const char *string);
	~JSONParser();

	// Sets
	JSONParser AddNode(const char *name_node);
	bool AddArray(const char *name_array);
	bool AddArray(const JSONParser &block);
	bool AddArray(const char *name_array, const JSONParser &block);

	bool AddBoolean(const char *node_name, bool boolean);
	bool AddInt(const char *int_name, int value);
	bool AddUUID(const char *uuid_name, long unsigned int value);
	bool AddFloat(const char *float_name, float value);
	bool AddString(const char *string_name, const char *string_value);

	template <class CONTAINER>
	bool AddPoints(const char *points_name, const CONTAINER &points, unsigned int num_elements)
	{
		JSON_Value *v = json_value_init_array();
		JSON_Array *array = json_value_get_array(v);

		const float *m = points.ptr();
		for (int i = 0; i < num_elements; ++i)
		{
			json_array_append_number(array, *m);
			++m;
		}

		return json_object_set_value(root, points_name, v) == JSONSuccess;
	}

	void ParseBuffer(char *buffer);

	// Gets
	JSONParser GetNode(const char *node_name) const;
	JSONParser GetArray(const char *array_name, int index = -1) const;
	int GetArrayCount(const char *array_name) const;

	bool GetBoolean(const char *name_boolean) const;
	int GetInt(const char *int_name) const;
	long unsigned int GetUUID(const char *uuid_name) const;
	float GetFloat(const char *float_name) const;
	const char *GetString(const char *string_name) const;

	template <class CONTAINER>
	void GetPoints(const char *points_name, CONTAINER &points, unsigned int num_elements) const
	{
		JSON_Array *array = json_object_get_array(root, points_name);

		float *m = points.ptr();
		for (int i = 0; i < num_elements; ++i)
		{
			*m = json_array_get_number(array, i);
			++m;
		}
	}

	// Utilities
	bool ValueExists(const char *node_name) const;
	void CreateResourceEntry(const char *filename, const char *imported_filename, long unsigned int id, RESOURCE_TYPE type);

	// Save
	void Save(char **buf);

	// Free
	void FreeBuffer(char **buff);

};

#endif __JSON_PARSER_H__
