
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
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "frame.hpp"
#include "lzv.hpp"
#include "Crypt.h"
#include "bitmap.hpp"

using namespace std::literals;

namespace
{
	Frame fr1;
	Frame fr2;
	
	DiffFrame df;

	std::string key = "abcdef123456"s;
	std::string crfn = base + "stream/64.czs"s;

	UL DICTSZ = 65'000;

	UC W = 64, H = 48;

	std::unique_ptr<decrypt_source> cr_s;
	std::unique_ptr<std::iostream> crypt_stream;

	TP t1, t2;
	UL i;
	
	std::vector<std::string> Params;

	GtkWidget* image;
	GtkWidget* window;
	GdkPixbuf* pixbuf;

	Frame* curr;
	Frame* prev;

	bool did_delta = false;
	RGB_Image img;
	
	bool want_more = true;
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

gboolean delete_event(GtkWidget* widget, GdkEvent* event, gpointer data)
{
	(void)widget;
	(void)event;
	(void)data;

	gtk_main_quit();
	return TRUE;
}

void SetPixbuf(GdkPixbuf* pb, RGB_Image& img)
{
	
	int ww, hh, rowstride, n_channels;
	guchar *pixels, *p;

	n_channels = gdk_pixbuf_get_n_channels (pb);

	g_assert (gdk_pixbuf_get_colorspace (pb) == GDK_COLORSPACE_RGB);
	g_assert (gdk_pixbuf_get_bits_per_sample (pb) == 8);

	ww = std::min<int>( img.w , gdk_pixbuf_get_width  (pb) );
	hh = std::min<int>( img.h , gdk_pixbuf_get_height (pb) );

	rowstride = gdk_pixbuf_get_rowstride (pb);
	pixels = gdk_pixbuf_get_pixels (pb);

	int x,y;
	for (y=0; y<hh; ++y)
	{
		for (x=0; x<ww; ++x)
		{
			p = pixels + y * rowstride + x * n_channels;
			RGB rgb = img.pix[x+y*img.w];
			p[0] = rgb.b;
			p[1] = rgb.g;
			p[2] = rgb.r;
		}
	}
}

gboolean idle_func([[maybe_unused]] gpointer data)
{
	if (!want_more) return TRUE;
	
	curr = (i % 2) ? &fr1 : &fr2;
	prev = (i % 2) ? &fr2 : &fr1;

	if (i)
	{
		if (!cr_s->have(1)) { want_more=false; return TRUE; }
		did_delta = cr_s->get(1);
	}
	if (did_delta)
	{
		lzv_decoder_template lz(DICTSZ, *cr_s);
		bool ok = df.LoadT(lz);
		if (!ok) { want_more=false; return TRUE; }
		df.mkFrame(*prev);
		assignFrame(*curr, df);
	} else {
		bool ok = curr->Load(*cr_s);
		if (!ok) { want_more=false; return TRUE; }
	}

	++i;

	std::cout << "decoded frame " << i << ", size " << int(curr->w) << "x" << int(curr->h) << (did_delta?" (delta)":"        ") << "\r" << std::flush;
	
	RGB_Image img;
	FromFrame(*curr, img);
	
	{
		auto fn = "stage/test-out"s + std::to_string(i) + ".bmp"s;
		std::ofstream ofs(fn, std::fstream::binary | std::fstream::out);
		SaveBMP(img, ofs);
	}

	SetPixbuf(pixbuf, img);

	while (true)
	{
		t2 = hrc::now();
		if ((t2-t1) > 35ms) break;
	}
	t1 = t2;
	
	gtk_widget_queue_draw(image);

	return TRUE;
}

void Main()
{

	if (hasparam("-key"))
	{
		int i = paramnum("-key");
		key = Params[i+1];
	}
	if (hasparam("-zds"))
	{
		int i = paramnum("-zds");
		DICTSZ = atoi(Params[i + 1].c_str());
	}
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
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(delete_event), nullptr);

	gtk_container_set_border_width(GTK_CONTAINER(window), 8);

	pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, false, 8, W*10, H*10);

	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(window), image);

	gtk_widget_show_all(window);
	gtk_widget_show(window);
	gtk_idle_add(&idle_func, nullptr);

	fr1.resize(W, H);
	fr2.resize(W, H);
	df.resize(W, H);

	crfn = "./stream/64.czs"s;
	std::cout << "attempting to load " << crfn << std::endl;

	crypt_stream = std::make_unique<std::fstream>(crfn, std::fstream::binary | std::fstream::in);
	cr_s = std::make_unique<decrypt_source>(key, *crypt_stream);

	t1 = hrc::now();
	i=0;

	gtk_main();
}

int main(int argc, char** argv)
{
	gtk_init(&argc, &argv);
	Params.insert(Params.begin(), argv+1, argv+argc);
	Main();
}







