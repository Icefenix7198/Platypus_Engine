#pragma once
#include "Globals.h"
#include "Application.h" //Esto no sirve de nada, no puede llamar a la app.
#include <String>
#include "ModuleEditor.h"

std::string consoleLog;
void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);
	
	for (int i = 0; tmp_string[i]!=NULL; i++)
	{
		consoleLog.push_back(tmp_string[i]);
	}
	consoleLog.push_back('\n');


	//stringTest;
	
}