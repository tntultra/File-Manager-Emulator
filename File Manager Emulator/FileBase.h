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
	virtual std::string get_name_impl() const noexcept = 0;
	virtual FILE_TYPE get_type_impl() const noexcept = 0;
public:
	virtual ~tFileBase() = default;
	
	FILE_TYPE get_type () const
	{
		return get_type_impl();
	}

	std::string name() const noexcept;
};


