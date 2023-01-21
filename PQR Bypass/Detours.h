#pragma once
#include <string>
#include <map>


bool isPartOf(char* a, char* b);
char* removeFirstSendAddon(char* lua);
int GetCharMutiNumber(char* txt, const char* Sunstring);
std::map<std::string, MemoryOperation*> _Detours{};

int __cdecl FrameScriptExecuteDetour(char* a1, char* a2, int a3)
{
	if (isPartOf((char*)"SendAddonMessage", a1))
	{
		if (GetCharMutiNumber(a1, "SendAddonMessage") > 1)
		{
			char* newLua = removeFirstSendAddon(a1);
			a1 = newLua;
			delete[] newLua;
		}
	}

	//---------------- return to the original function ----------------
	auto det = _Detours["FrameScriptExecute"];
	det->Restore();
	auto res = ((int(__cdecl*)(char*, char*, int))det->target)(a1, a2, a3);
	det->Apply();
	return res;
}

bool isPartOf(char* a, char* b) {
	if (std::strstr(b, a) != NULL) {    //Strstr says does b contain a
		return true;
	}
	return false;
}

inline int GetCharMutiNumber(char* txt, const char* Sunstring)
{
	char* first = strstr(txt, Sunstring);
	int count = 0;
	while (first != NULL) {
		count++;
		if (count == 2) {
			break;
		}
		first = strstr(first + 1, Sunstring);
	}
	return count;
}

inline char* removeFirstSendAddon(char* lua)
{
	std::string str(lua);
	std::size_t pos = str.find("SendAddonMessage");
	pos = str.find("SendAddonMessage", pos + 1);
	std::size_t end = str.find("end", pos);
	std::size_t end2 = str.find("end", pos + 15);
	int new_len = end2 - pos;
	char* new_string = new char[new_len + 1];
	memcpy(new_string, &lua[pos], new_len);
	new_string[new_len] = '\0';
	return new_string;
}
