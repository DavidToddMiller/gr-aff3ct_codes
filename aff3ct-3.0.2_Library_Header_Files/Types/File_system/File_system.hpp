#ifndef ARGUMENT_TYPE_FILE_SYSTEM_HPP_
#define ARGUMENT_TYPE_FILE_SYSTEM_HPP_

#include <stdexcept>
#include <algorithm>
#include <string>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <processenv.h> // GetEnvironmentVariableA
#else
#include <cstdlib> // std::getenv
#endif

#include "Tools/utilities.hpp"
#include "Types/Argument_type_limited.hpp"

#include <iostream>

namespace cli
{

enum class openmode : uint8_t {read, write, read_write};

template <typename Read_F>
std::string modify_path(const std::string& val)
{
	std::string binary_path = cli::get_binary_path();
	if (!binary_path.empty())
	{
		std::string basedir, filename;
		cli::split_path(binary_path, basedir, filename);

#if defined(_WIN32) || defined(_WIN64)
		// remove the extension in the filename
		std::string ext = ".exe";
		filename.replace(filename.find(ext), ext.length(), "");
#endif

		std::string env_path = "";
#if defined(_WIN32) || defined(_WIN64)
		char buff[4096];
		auto n_chars_read = (size_t)GetEnvironmentVariableA("AFF3CT_CLI_PATH", buff, 4096);
		if (n_chars_read > 0)
		{
			if (n_chars_read >= 4096)
				throw std::runtime_error("'buff' is not large enough to store the contents of the 'AFF3CT_CLI_PATH' "
				                         "environement variable.");
			env_path = buff;
		}
#else
		if (std::getenv("AFF3CT_CLI_PATH") != nullptr)
			env_path = std::getenv("AFF3CT_CLI_PATH");
#endif
		std::vector<std::string> paths = {
			"../../",
			"../../../",
			"../share/" + filename + "/",
			"../../share/" + filename + "/",
			"/usr/share/" + filename + "/",
			"/usr/share/" + filename + "/",
			"/usr/local/share/" + filename + "/",
		};

		if (!env_path.empty())
		{
#if defined(_WIN32) || defined(_WIN64)
			auto env_paths = split(env_path, ';');
#else
			auto env_paths = split(env_path, ':');
#endif
			for (auto &p : env_paths)
#if defined(_WIN32) || defined(_WIN64)
				if (p[p.length()-1] != '/' && p[p.length()-1] != '\\')
#else
				if (p[p.length()-1] != '/')
#endif
					p.append("/");
			paths.insert(paths.begin(), env_paths.begin(), env_paths.end());
		}

		for (auto &path : paths)
		{
			std::string full_path = "";
#if defined(_WIN32) || defined(_WIN64)
			if (path.length() >= 3 && (path[1] != ':' || path[2] != '\\'))
				full_path = basedir + "\\";
#else
			if (path.length() >= 1 && path[0] != '/')
				full_path = basedir + "/";
#endif
			full_path += path + val;
#if defined(_WIN32) || defined(_WIN64)
			if (path[0] == '/')
				continue;
			std::replace(full_path.begin(), full_path.end(), '/', '\\');
#endif
			if (Read_F::check(full_path))
				return full_path;
		}
	}

	return "";
}

std::string openmode_to_string(const openmode& mode);

struct Is_file
{
	static bool check(const std::string& filename);
};

struct Is_folder
{
	static bool check(const std::string& foldername);
};

struct Is_path
{
	static bool check(const std::string& path);
};

struct No_check
{
	static bool check(const std::string&);
};

template <typename T = std::string, typename Read_F = No_check, typename Write_F = No_check, typename RW_F = No_check, typename... Ranges>
class File_system_type : public Argument_type_limited<T,Ranges...>
{
protected:
	const std::string name;
	const openmode    mode;

public:
	template <typename r, typename... R>
	explicit File_system_type(const std::string& name, const openmode& mode, const r* range, const R*... ranges)
	: Argument_type_limited<T,Ranges...>(generate_title(name, mode), range, ranges...), name(name), mode(mode)
	{ }

	explicit File_system_type(const std::string& name, const openmode& mode)
	: Argument_type_limited<T,Ranges...>(generate_title(name, mode)), name(name), mode(mode)
	{ }

	virtual ~File_system_type() {};

	virtual File_system_type<T, Read_F, Write_F, RW_F, Ranges...>* clone() const
	{
		auto* clone = new File_system_type<T, Read_F, Write_F, RW_F, Ranges...>(name, mode);

		return dynamic_cast<File_system_type<T, Read_F, Write_F, RW_F, Ranges...>*>(this->clone_ranges(clone));
	}

	template <typename... NewRanges>
	File_system_type<T, Read_F, Write_F, RW_F, Ranges..., NewRanges...>* clone(NewRanges*... new_ranges)
	{
		auto* clone = new File_system_type<T, Read_F, Write_F, RW_F, Ranges..., NewRanges...>(name, mode);

		this->clone_ranges(clone);

		clone->add_ranges(new_ranges...);

		return clone;
	}

	static std::string generate_title(const std::string& name, const openmode& mode)
	{
		return name + " [" + openmode_to_string(mode) + "]";
	}

	virtual T convert(const std::string& val) const
	{
		return val;
	}

	virtual void check(const std::string& val) const
	{
		auto str_val = this->convert(val);

		if (str_val.empty())
			throw std::runtime_error("shall be a " + name + " name");

		switch(mode)
		{
			case openmode::read :
				if(!Read_F::check(str_val))
					if (modify_path<Read_F>(str_val).empty())
						throw std::runtime_error("does not name an existing " + name);
				break;

			case openmode::write :
				if(!Write_F::check(str_val))
					throw std::runtime_error("does not name a " + name);
				break;

			case openmode::read_write : // nothing to check
				if(!RW_F::check(str_val))
					throw std::runtime_error("does not name a " + name);
				break;
		}

		this->check_ranges(str_val);
	}
};

}

#include "File.hpp"
#include "Folder.hpp"
#include "Path.hpp"

#endif /* ARGUMENT_TYPE_FILE_SYSTEM_HPP_ */