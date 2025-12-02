
import std;

import pop;

#include "Crypt.hpp"
#include "util.hpp"

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
			if (rd) {
				cr.encrypt_block((UC*)buff.data(), rd);
				os.write((char*)buff.data(), rd);
			}
			if (rd<BL)
				return cr.nextcount();
		}
	}

	i = 0;
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
	const std::string& ext)
{	
	bool report = true;
	std::size_t rem = 0;
	std::string nfn;

	pop<std::istream> ifs;
	pop<std::ostream> ofs;

	if (str == "-"s) {
		ifs.borrow(std::cin);
		report = false;
	} else {
		ifs.create<std::ifstream>(source + "/" + str, std::fstream::binary);
		rem = std::filesystem::file_size(str);
	}

	if (target == "-"s) {
		ofs.borrow(std::cout);
		report = false;
	} else {
		if (str == "-"s)
			nfn = target + "/" + "output.decrypt";
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
			if (rd) {
				cr.decrypt_block((UC*)buff.data(), rd);
				os.write((char*)buff.data(), rd);
			}
			if (rd<BL)
				return cr.nextcount();
		}
	}

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
	const std::string& ext)
{
	if (str.find_first_of("*?") != std::string::npos)
	{
		long long acc = 0;

		for (const auto& entry : std::filesystem::directory_iterator(source)) {

			auto p = entry.path();

			if (strmat(str, p.filename().string()))
				acc += decrypt(cr, p.filename().string(), source, target, ext);

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
		ifs.borrow(std::cin);
		report = false;
	} else {
		auto fn = source + "/" + str;
		ifs.create<std::ifstream>(fn, std::fstream::binary);
		rem = std::filesystem::file_size(fn);
	}

	if (target == "-"s) {
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
	int i, n = std::ssize(args);
	for (i = 0; i < n; ++i) {
		if (args[i] == "-t"s) {
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

	if (target != "-"s)
		std::println("passes {}", cr.passcount());

	long long acc = 0;
	for (auto&& f : files) {
		if (de == deenc)
			acc += decrypt(cr, f, source, target, ext);
		else if (de == enc)
			acc += encrypt(cr, f, source, target, ext);
	}

	if (target != "-"s)
		std::println("tokens {}", pritty(acc));

	return {};
}

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
	extern void test();
	test();
	return 0;
	/*
	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i)
		args.push_back(argv[i]);

	auto res = Main(args);
	res.PrintIf(std::cerr);
	return res.Ok() ? 0 : -1;
	*/
}


