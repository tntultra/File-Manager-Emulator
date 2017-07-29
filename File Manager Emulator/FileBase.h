#pragma once

#include <memory>
#include <string>



enum class FILE_TYPE
{
	REGULAR_FILE,
	HARD_LINK,
	SOFT_LINK
};

class tFileBase
{
	virtual std::string get_name_impl() noexcept = 0;
	virtual FILE_TYPE get_type_impl() noexcept = 0;
	virtual std::unique_ptr<tFileBase> clone_impl() = 0;
public:
	virtual ~tFileBase() = default;
	
	FILE_TYPE get_type() noexcept
	{
		return get_type_impl();
	}

	std::string name() noexcept {
		return get_name_impl();
	}
	std::unique_ptr<tFileBase> clone()
	{
		return clone_impl();
	}
};


