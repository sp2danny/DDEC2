
import std;

import pop;

import util;

using namespace std::literals;

#include "Crypt.hpp"

#include <fcntl.h>
#include <io.h>
#include <stdio.h>

class Result
{
public:
	Result() = default;
	Result(std::string errmsg) : ok(false), errmsg(errmsg) {}

	bool Ok() const { return ok; }
	void ThrowIf() const { if (!ok) throw errmsg; }
	void PrintIf(std::ostream& out) const {
		if (!ok) out << errmsg << std::endl;
	}
	std::optional<std::string> Error() { if (ok) return std::nullopt; else return errmsg; }
private:
	bool ok = true;
	std::string errmsg = ""s;
};

Result usage()
{
	return { "usage: de [-t target] [-s source] [-e ext] d|e file[s]"s };
}

bool old = true;

long long acc_sz = 0;

long long encrypt
(
	Crypt cr,
	std::istream& is,
	std::ostream& os,
	std::size_t rem,
	bool prog,
	std::string_view str
)
{
	auto sz = rem;
	auto i = sz-sz;
	constexpr UL BL = cr.maxblock();
	std::vector<std::byte> buff;
	buff.resize(BL);

	int sh=0, m=1;
	while (true) {
		if (((sz/BL)>>sh) < 400) break;
		++sh;
		m = (m << 1) | 1;
	}

	if (!rem) {
		while (true) {
			is.read((char*)buff.data(), BL);
			auto rd = is.gcount();
			acc_sz += rd;
			if (rd) {
				cr.encrypt_block((UC*)buff.data(), rd);
				os.write((char*)buff.data(), rd);
			}
			if (rd<BL)
				return cr.nextcount();
		}
	}

	i = 0;
	acc_sz += sz;
	while (rem > 0)
	{
		if (rem >= BL) {
			is.read((char*)buff.data(), BL);
			cr.encrypt_block((UC*)buff.data(), BL);
			os.write((char*)buff.data(), BL);
			rem -= BL;
			if ((i&m)==0) [[unlikely]]
				if (prog) std::cout << str << " : " << (((sz-rem)*100)/sz) << " %\r" << std::flush;
			++i;
		} else {
			is.read((char*)buff.data(), rem);
			cr.encrypt_block((UC*)buff.data(), (int)rem);
			os.write((char*)buff.data(), rem);
			break;
		}
	}
	if (prog) std::cout << str << "        \n";
	return cr.nextcount();
}

long long encrypt(
	Crypt cr, 
	const std::string& str, 
	const std::string& source,
	const std::string& target, 
	const std::string& ext,
	[[maybe_unused]] bool newmode,
	bool overwrite
)
{
	if (str.find_first_of("*?") != std::string::npos)
	{
		long long acc = 0;

		for (const auto& entry : std::filesystem::directory_iterator(source)) {

			auto p = entry.path();

			if (strmat(str, p.filename().string()))
				acc += encrypt(cr, p.filename().string(), source, target, ext, newmode, overwrite);

		}

		return acc;
	}

	bool report = true;
	std::size_t rem = 0;
	std::string nfn;

	pop<std::istream> ifs;
	pop<std::ostream> ofs;

	if (str == "-"s) {
		//std::cin.setf(std::ios_base::binary);
		_setmode(_fileno(stdin), _O_BINARY);
		ifs.borrow(std::cin);
		report = false;
	} else {
		ifs.create<std::ifstream>(source + "/" + str, std::fstream::binary);
		rem = std::filesystem::file_size(source + "/" + str);
	}

	if (target == "-"s) {
		//std::cout.setf(std::ios_base::binary);
		_setmode(_fileno(stdout), _O_BINARY);
		ofs.borrow(std::cout);
		report = false;
	} else {
		if (str == "-"s)
			nfn = target + "/" + "output.encrypt";
		else
			nfn = target + "/" + str + ".encrypt";
		if (!ext.empty())
			nfn = replace_ext(nfn, ext);
		ofs.create<std::ofstream>(nfn, std::fstream::binary);
	}

	if (ifs && ofs)
	{
		return encrypt(std::move(cr), *ifs, *ofs, rem, report, str);
	} else {
		std::cerr << "error\n";
		return 0;
	}
}

long long decrypt(Crypt cr, std::istream& is, std::ostream& os, std::size_t rem, bool prog, const std::string& str)
{
	auto sz = rem;
	constexpr UL BL = cr.maxblock();
	std::vector<std::byte> buff;
	buff.resize(BL);

	if (!rem) {
		while (true) {
			is.read((char*)buff.data(), BL);
			auto rd = is.gcount();
			acc_sz += rd;
			if (rd) {
				cr.decrypt_block((UC*)buff.data(), rd);
				os.write((char*)buff.data(), rd);
			}
			if (rd<BL)
				return cr.nextcount();
		}
	}

	acc_sz += sz;
	int i=0, sh=0, m=1;
	while (true) {
		if (((sz/BL)>>sh) < 400) break;
		++sh;
		m = (m << 1) | 1;
	}

	while (rem > 0)
	{
		if (rem >= BL) {
			is.read((char*)buff.data(), BL);
			cr.decrypt_block((UC*)buff.data(), BL);
			os.write((char*)buff.data(), BL);
			rem -= BL;
			if ((i&m)==0) [[unlikely]]
				if (prog) std::cout << str << " : " << (((sz-rem)*100)/sz) << " %\r" << std::flush;
			++i;
		} else {
			is.read((char*)buff.data(), rem);
			cr.decrypt_block((UC*)buff.data(), (int)rem);
			os.write((char*)buff.data(), rem);
			break;
		}
	}
	if (prog) std::cout << str << "        " << std::endl;
	return cr.nextcount();
}

long long decrypt(
	Crypt cr, 
	const std::string& str, 
	const std::string& source,
	const std::string& target, 
	const std::string& ext,
	[[maybe_unused]] bool newmode,
	bool overwrite
)
{
	if (str.find_first_of("*?") != std::string::npos)
	{
		long long acc = 0;

		for (const auto& entry : std::filesystem::directory_iterator(source)) {

			auto p = entry.path();

			if (strmat(str, p.filename().string()))
				acc += decrypt(cr, p.filename().string(), source, target, ext, newmode, overwrite);
		}

		return acc;
	}

	bool report = true;
	std::size_t rem = 0;
	std::string nfn;

	pop<std::istream> ifs;
	pop<std::ostream> ofs;

	pop<std::size_t> pi{ to_borrow{}, &rem };

	if (str == "-"s) {
		//std::cin.setf(std::ios_base::binary);
		_setmode(_fileno(stdin), _O_BINARY);
		ifs.borrow(std::cin);
		report = false;
	} else {
		auto fn = source + "/" + str;
		ifs.create<std::ifstream>(fn, std::fstream::binary);
		rem = std::filesystem::file_size(fn);
	}

	if (target == "-"s) {
		//std::cout.setf(std::ios_base::binary);
		_setmode(_fileno(stdout), _O_BINARY);
		ofs.borrow(std::cout);
		report = false;
	} else {
		if (str == "-"s)
			nfn = target + "/" + "output.decrypt";
		else
			if (str.ends_with(".encrypt"))
				nfn = target + "/" + str.substr(0, str.length()-8);
			else
				nfn = target + "/" + str + ".decrypt";
		if (!ext.empty())
			nfn = replace_ext(nfn, ext);
		ofs.create<std::ofstream>(nfn, std::fstream::binary);
	}

	if (ifs && ofs)
	{
		return decrypt(std::move(cr), *ifs, *ofs, rem, report, str);
	} else {
		std::cerr << "error\n";
		return 0;
	}
}

Result Main(const std::vector<std::string>& args)
{
	using namespace std::literals;
	enum { none, deenc, enc } de = none;
	bool hp = false;
	std::vector<std::string> files;
	std::string target = ".";
	std::string source = ".";
	std::string ext;
	bool stats = false;
	bool newmode = false;
	bool overwrite = false;
	int i, n = std::ssize(args);
	for (i = 0; i < n; ++i) {
		if (args[i] == "--version"s) {
			std::println("ver 1.0.02");
		}
		else if (args[i] == "--stats"s) {
			stats = true;
		}
		else if (args[i] == "--newmode"s) {
			newmode = true;
		}
		else if (args[i] == "--overwrite"s) {
			overwrite = true;
		}
		else if (args[i] == "-t"s) {
			target = args[++i];
		}
		else if (args[i] == "-s"s) {
			source = args[++i];
		}
		else if (args[i] == "-e"s) {
			ext = args[++i];
		}
		else if (args[i] == "-p"s) {
			pwd = args[++i];
			hp = true;
		}
		else if (args[i] == "-o"s) {
			old = true;
		}
		else if (args[i] == "-n"s) {
			old = false;
		}
		else if (de == none) {
			if (args[i] == "d"s) {
				de = deenc;
			}
			else if (args[i] == "e"s) {
				de = enc;
			}
			else {
				return usage();
			}
		}
		else {
			files.push_back(args[i]);
		}
	}
	if (files.empty())
		return usage();
	if (de == none)
		return usage();

	if (!hp) {
		if (target == "-"s)
			getpwd("", false);
		else
			getpwd("Password:", true);
	}

	Crypt cr{ pwd, old };
	for (auto& c : pwd) c = 0;

	auto pc = cr.passcount();

	auto tp1 = std::chrono::high_resolution_clock::now();

	long long acc = 0;
	for (auto&& f : files) {
		if (de == deenc)
			acc += decrypt(cr, f, source, target, ext, newmode, overwrite);
		else if (de == enc)
			acc += encrypt(cr, f, source, target, ext, newmode, overwrite);
	}

	auto tp2 = std::chrono::high_resolution_clock::now();

	auto tok = pritty(acc);

	if (stats) {

		auto& out = (target == "-"s) ? std::cerr : std::cout;

		typedef std::chrono::duration<float> fsec;
		fsec fs = tp2 - tp1;

		std::println(out, "Passes: {}", pc);
		std::println(out, "Tokens: {}", tok);
		std::println(out, "Size: {}", acc_sz);
		std::println(out, "Time {} ", fs);

		float sz = acc_sz / 1024.0f;

		std::println(out, "Speed: {} kb/s", sz/fs.count());

	}

	return {};
}

int main(int argc, char** argv)
{
	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i)
		args.push_back(argv[i]);

	auto res = Main(args);
	res.PrintIf(std::cerr);
	return res.Ok() ? 0 : -1;
}


