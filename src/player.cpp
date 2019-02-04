
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <string_view>
#include <iostream>
#include <fstream>
#include <memory>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "frame.hpp"
#include "lzv.hpp"
#include "Crypt.h"

std::vector<std::string> Params;

namespace
{
	Frame fr1;
	Frame fr2;
}

#define OFS(fn) unmove(std::ofstream{fn, std::fstream::binary | std::fstream::out})
#define IFS(fn) unmove(std::ifstream{fn, std::fstream::binary | std::fstream::in})

using hrc = std::chrono::high_resolution_clock;

bool hasparam(std::string_view str)
{
	for(auto&& x : Params)
		if (x==str)
			return true;
	return false;
}

int paramnum(std::string_view str)
{
	int i = 0;
	for (auto&& x : Params)
	{
		if (x == str)
			return i;
		++i;
	}
	return -1;
}

gboolean idle_func(gpointer data);

extern gboolean delete_event(GtkWidget* widget, GdkEvent* event, gpointer data);

GtkWidget* window;

void Main()
{

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(delete_event), nullptr);

	gtk_container_set_border_width(GTK_CONTAINER(window), 8);

	GtkWidget* image = gtk_image_new();
	gtk_widget_show(image);

	gtk_widget_show_all(window);
	gtk_widget_show(window);
	gtk_idle_add(&idle_func, nullptr);
	gtk_main();


	using namespace std;

	DiffFrame df;

	Params = { 
		"-w", "102", "-h", "76"
		"-fps", "50",
		"-zds", "65000", "-key", "qwerty123456",
	};

	std::string key = "abcdef123456"s;
	std::string crfn = base + "stream/64.czs"s;

	if (hasparam("-key"))
	{
		int i = paramnum("-key");
		key = Params[i+1];
	}

	UL DICTSZ = 65'000;
	if (hasparam("-zds"))
	{
		int i = paramnum("-zds");
		DICTSZ = atoi(Params[i + 1].c_str());
	}

	UC W = 64, H = 48;
	if (hasparam("-w"))
	{
		int i = paramnum("-w");
		W = (UC)std::stoi(Params[i + 1]);
	}
	if (hasparam("-h"))
	{
		int i = paramnum("-h");
		H = (UC)std::stoi(Params[i + 1]);
	}

	fr1.resize(W, H);
	fr2.resize(W, H);
	df.resize(W, H);

	unique_ptr<decrypt_source> cr_s;
	unique_ptr<std::iostream> crypt_stream;

	crypt_stream = make_unique<std::fstream>(crfn, std::fstream::binary | std::fstream::in);
	cr_s = make_unique<decrypt_source>(key, *crypt_stream);

	[[maybe_unused]] TP t1, t2;
	t1 = hrc::now();

	UL i;
	for (i=0; true; ++i)
	{
		[[maybe_unused]] Frame* curr = (i % 2) ? &fr1 : &fr2;
		[[maybe_unused]] Frame* prev = (i % 2) ? &fr2 : &fr1;

		bool did_delta = i;

		RGB_Image img;

		if (i)
		{
			if (!cr_s->have(1)) break;
			did_delta = cr_s->get(1);
			if (did_delta)
			{
				lzv_decoder_template<decltype(*cr_s)> lz(DICTSZ, *cr_s);
				bool ok = df.LoadT(lz);
				if (!ok) break;
			} else {
				bool ok = curr->Load(*cr_s);
				if (!ok) break;
			}
		} else {
			bool ok = curr->Load(*cr_s);
			if (!ok) break;
		}

		if (i && did_delta)
		{
			df.mkFrame(*prev);
			assignFrame(*curr, df);
		}
		
		//curr->

	}




}

int main()
{
	Main();
}







