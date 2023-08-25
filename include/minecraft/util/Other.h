#pragma once

#include <code/code.h>

template<typename... Args>
static void mc_swprintf(const wchar_t* outputPointer, uint32_t size, const wchar_t* inputString, Args... args) {
	code::Func<void, 0x382C8C0, const wchar_t*, uint32_t, const wchar_t*, Args...>()(outputPointer, size, inputString, args...);
}

static float degToRad(float deg) {
    return (deg * M_PI / 180.0f);
}
static float radToDeg(float rad) {
    return (rad * 180.0f / M_PI);
}

char* wcharToChar(const wchar_t* str, int length = 0) {
	bool found = length != 0;
	while (!found) {
		if (str[length++] == 0x0) {
			found = true;
			length--;
		}
		if (length >= 250) return (char*) "";
	}

	char* newStr = _new<char>(length + 1);
	for (int i = 0; i < length; i++) {
		newStr[i] = str[i];
	}

	newStr[length] = 0;
	return newStr;
}

wchar_t* charToWChar(const char* str, int length = 0) {
	bool found = length != 0;
	while (!found) {
		if (str[length++] == 0x0) {
			found = true;
			length--;
		}
		if (length >= 250) return (wchar_t*) u"";
	}

	wchar_t* newStr = _new<wchar_t>(length + 1);
	for (int i = 0; i < length; i++) {
		newStr[i] = str[i];
	}

	newStr[length] = 0;
	return newStr;
}