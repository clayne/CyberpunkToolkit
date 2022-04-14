#include "resources/version.h"

#include <TermAPI.hpp>
#include <palette.hpp>
#include <ParamsAPI2.hpp>
#include <envpath.hpp>
#include <fileio.hpp>

static struct {
	env::PATH PathVar;
	std::filesystem::path location;
	std::string executable_name;
	bool quiet{ false };

	/**
	 * @brief			Retrieve the name & location of the running executable.
	 * @param argv0		The value of argv[0]
	 */
	void init(char* argv0)
	{
		auto [myPath, myName] { PathVar.resolve_split(argv0) };
		location = myPath;
		executable_name = myName.replace_extension().generic_string();
	}

} Global;

struct PrintHelp {
	std::optional<std::string> topic;

	PrintHelp(const std::optional<std::string>& topic = std::nullopt) : topic{ topic } {}

	friend std::ostream& operator<<(std::ostream& os, const PrintHelp& h)
	{
		if (h.topic.has_value()) {
			const auto& topic{ str::strip_preceeding(h.topic.value(), '-', ' ', '\t', '\r', '\n') };
			if (str::equalsAny(topic, "p", "pack")) { // PACK
				os
					<< "  -p, --pack <PATH>    Packs the contents of '<PATH>' into an '.archive' file." << '\n'
					<< '\n'
					<< "USAGE:\n"
					<< "  " << Global.executable_name << " -p 'path/to/directory' -o 'path/to/MyModName.archive'" << '\n'
					<< '\n';
				if (!Global.quiet) os
					<< "  This argument requires an additional 'capture' argument to be valid. Capture arguments may be specified:\n"
					<< "  - Following the argument without a space, seperated using an equals sign '=', ex:\n"
					<< "    `-p='path/to/directory'`\n"
					<< "  - Following the argument after a space, ex:\n"
					<< "    `-p 'path/to/directory'`\n"
					<< "  This argument accepts optional contextual arguments, shown under the 'OPTIONS' header below.\n"
					<< '\n';
				os
					<< "OPTIONS:\n"
					<< "  -o, --out            Specifies the location of the created '.archive' file." << '\n'
					;
			}
			else if (str::equalsAny(topic, "u", "unpack")) { // UNPACK
				os
					<< "  -u, --unpack <PATH>  Unpacks the '.archive' file specified by '<PATH>'." << '\n'
					<< '\n';
				if (!Global.quiet) os
					<< "  This argument requires an additional 'capture' argument to be valid. Capture arguments may be specified:\n"
					<< "  - Following the argument without a space, seperated using an equals sign '=', ex:\n"
					<< "    `-p='path/to/directory'`\n"
					<< "  - Following the argument after a space, ex:\n"
					<< "    `-p 'path/to/directory'`\n"
					<< "  This argument accepts optional contextual arguments, shown under the 'OPTIONS' header below.\n"
					<< '\n';
				os
					<< "USAGE:\n"
					<< "  " << Global.executable_name << " -u 'path/to/directory' -o 'path/to/MyModName.archive'" << '\n'
					<< '\n'
					<< "OPTIONS:\n"
					<< "  -o, --out            Specifies the location of the created '.archive' file." << '\n'
					;
			}
			else throw make_exception("Couldn't find a help topic with the name '", topic, "'!");
		}
		else os
			<< "Cyberpunk Toolkit  v" << cyberkit_VERSION_EXTENDED << '\n'
			<< "  Commandline toolkit for working with Cyberpunk 2077 mods." << '\n'
			<< '\n'
			<< "USAGE:\n"
			<< "  " << Global.executable_name << " < <OPTIONS> | [OPTIONS] <MODE> > " << '\n'
			<< '\n'
			<< "  " << '\n'
			<< '\n'
			<< "OPTIONS:\n"
			<< "  -h, --help [MODE]    Shows this help display, then exits. Also accepts the name of a\n"
			<< "                        mode to show more specific help for that mode." << '\n'
			<< "  -v, --version        Shows the current version number, then exits." << '\n'
			<< "  -q, --quiet          Reduces the amount of console output." << '\n'
			<< "  -n, --no-color       Disables the usage of color in console output." << '\n'
			<< '\n'
			<< "MODES:\n"
			<< "  -p, --pack <PATH>    Packs the contents of '<PATH>' into an '.archive' file." << '\n'
			<< "  -u, --unpack <PATH>  Unpacks the '.archive' file specified by '<PATH>'." << '\n'
			;
		return os;
	}
};

inline std::stringstream read_archive(const std::filesystem::path& path)
{
	return std::move(file::read(path, file::openmode::binary));
}

int main(const int& argc, char** argv)
{
	term::palette<short> colors{};
	try {
		opt::ParamsAPI2 args{ argc, argv, 'h', "help" };
		Global.init(argv[0]);

		// arg:  q|quiet
		Global.quiet = args.check_any<opt::Flag, opt::Option>('q', "quiet");
		// arg:  n|no-color
		colors.setActive(!args.check_any<opt::Flag, opt::Option>('n', "no-color"));

		// arg:  h|help
		if (const bool& empty{ args.empty() }; empty || args.check_any<opt::Flag, opt::Option>('h', "help")) {
			std::cout << PrintHelp() << std::endl;
			if (empty) throw make_exception("No arguments specified, nothing to do!");
			return 0;
		}
		// arg:  v|version
		else if (args.check_any<opt::Flag, opt::Option>('v', "version")) {
			if (!Global.quiet) std::cout << Global.executable_name << "  ";
			std::cout << cyberkit_VERSION_EXTENDED << std::endl;
			return 0;
		}

		if (const auto& arg_pack{ args.typegetv_any<opt::Flag, opt::Option>('p', "pack") }; arg_pack.has_value()) {
			if (std::filesystem::path root_dir{ arg_pack.value() }; std::filesystem::is_directory(root_dir)) {

				if (!file::exists(root_dir)) // make sure the source directory actually exists
					throw make_exception("Directory doesn't exist: '", root_dir, "'!");

				// get the target archive
				std::filesystem::path out_archive{ args.castgetv_any<std::filesystem::path, opt::Flag, opt::Option>('o', "out").value_or(root_dir.filename().replace_extension(".archive")) };

				// create archive


			}
			else throw make_exception("Specified path is not a directory: '", root_dir.generic_string(), "'!");
		}
		else if (const auto& arg_unpack{ args.typegetv_any<opt::Flag, opt::Option>('u', "unpack") }; arg_unpack.has_value()) {
			std::string archive{ arg_unpack.value() };

			if (!file::exists(archive))
				throw make_exception("File doesn't exist: '", archive, "'!");

			// get the output location
			std::filesystem::path out_dir{ args.castgetv_any<std::filesystem::path, opt::Flag, opt::Option>('o', "out").value_or(file::getWorkingDir<std::filesystem::path>() / std::filesystem::path(archive).filename().replace_extension()) };

			// create directories in case they don't exist
			std::filesystem::create_directories(out_dir);

			// unpack archive


		}
		else throw make_exception("No mode specified; Nothing to do!");

		return 0;
	} catch (const std::exception& ex) {
		std::cerr << colors.get_fatal() << ex.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << colors.get_fatal() << "An undefined exception occurred!" << std::endl;
		return 1;
	}
}
