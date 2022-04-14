#include <TermAPI.hpp>
#include <palette.hpp>
#include <ParamsAPI2.hpp>
#include <envpath.hpp>

int main(const int& argc, char** argv)
{
	term::palette<short> colors{};
	try {
		opt::ParamsAPI2 args{ argc, argv };
		env::PATH PathVar;
		const auto& [myPath, myName] {PathVar.resolve_split(argv[0])};

		return 0;
	} catch (const std::exception& ex) {
		std::cerr << colors.get_error() << ex.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << colors.get_error() << ex.what() << std::endl;
		return 1;
	}
}
