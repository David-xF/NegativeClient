#pragma once

#define MC_VFUNC(return_type, class_type, function_name, ...)     \
	int flag_##function_name;                                     \
	return_type(*function_name)(class_type* _this, ##__VA_ARGS__)