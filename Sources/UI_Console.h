#ifndef UI_CONSOLE_CLASS
#define UI_CONSOLE_CLASS

#include "UI.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

class UI_Console : public UI
{
public:
	UI_Console();
	~UI_Console();

	void Render() override;

	void AddLog(const char* fmt, ...) IM_FMTARGS(2);

private:
	char                  InputBuf[256];
	ImVector<char*>       Items;
	bool                  ScrollToBottom;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImVector<const char*> Commands;

	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
	static void  Strtrim(char* str) { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

	//Functions
	void ClearLog();
	void ExecCommand(const char* command_line);
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
		UI_Console* console = (UI_Console*)data->UserData;
		return console->TextEditCallback(data);
	}
	int TextEditCallback(ImGuiInputTextCallbackData* data);
};

#endif // !UI_CONSOLE_CLASS