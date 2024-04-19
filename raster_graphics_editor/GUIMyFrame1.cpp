#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_staticText1->SetLabel(_(L"Jasno\u015B\u0107"));
 m_b_threshold->SetLabel(_(L"Pr\u00F3g 128"));
 this->SetBackgroundColour(wxColor(192, 192, 192));
 m_scrolledWindow->SetScrollbars(25, 25, 52, 40);
 m_scrolledWindow->SetBackgroundColour(wxColor(192, 192, 192));
}

void GUIMyFrame1::m_scrolledWindow_update( wxUpdateUIEvent& event )
{
 Repaint();
}

void GUIMyFrame1::m_b_grayscale_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy = Img_Cpy.ConvertToGreyscale();  
}

void GUIMyFrame1::m_b_blur_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy = Img_Cpy.Blur(5);
}

void GUIMyFrame1::m_b_mirror_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy = Img_Cpy.Mirror();
}

void GUIMyFrame1::m_b_replace_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.Replace(254, 0, 0, 0, 0, 255);
}

void GUIMyFrame1::m_b_rescale_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.Rescale(320, 240);
}

void GUIMyFrame1::m_b_rotate_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy = Img_Cpy.Rotate(30 * (M_PI / 180.0), wxPoint(Img_Cpy.GetSize().GetWidth() / 2, Img_Cpy.GetSize().GetHeight() / 2));
}

void GUIMyFrame1::m_b_rotate_hue_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.RotateHue(0.5);
}

void GUIMyFrame1::m_b_mask_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.SetMaskFromImage(Img_Mask, 0, 0, 0);
}

void GUIMyFrame1::m_s_brightness_scroll( wxScrollEvent& event )
{
// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
// Brightness(...), ktora zmienia jasnosc. W tym miejscu nic nie
// zmieniamy. Do uzupelnienia pozostaje funkcja Brightness(...)
Brightness(m_s_brightness->GetValue() - 100);
Repaint();
}

void GUIMyFrame1::m_s_contrast_scroll( wxScrollEvent& event )
{
// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
// Contrast(...), ktora zmienia kontrast. W tym miejscu nic nie
// zmieniamy. Do uzupelnienia pozostaje funkcja Contrast(...)
Contrast(m_s_contrast->GetValue() - 100);
Repaint();
}

int getColor(unsigned char* image, int y, int x, int h, int w, int color)
{
	if (((y * w + x) * 3 + color) >= 0 && ((y * w + x) * 3 + color) < w * h * 3)
		return (int)image[(y * w + x) * 3 + color];
	else
		return 0;
}

int PrewittKernel(unsigned char* image, int x, int y, int h, int w, int color)
{
	int sum = 0;
	sum -= getColor(image, y - 1, x - 1, h, w, color);
	sum -= getColor(image, y, x - 1, h, w, color);
	sum -= getColor(image, y + 1, x - 1, h, w, color);
	sum += getColor(image, y - 1, x + 1, h, w, color);
	sum += getColor(image, y, x + 1, h, w, color);
	sum += getColor(image, y + 1, x + 1, h, w, color);
	return sum;
}


void GUIMyFrame1::m_b_prewitt_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	wxImage Img_Tmp = Img_Cpy.Copy();

	unsigned char* array_RGB_Copy = Img_Cpy.GetData();
	unsigned char* array_RGB_Img = Img_Tmp.GetData();
	int width = Img_Tmp.GetWidth();
	int height = Img_Tmp.GetHeight();

	int gPrewitt;
	int red_value, green_value, blue_value;

	for (size_t y = 1; y < height - 1; y++)
	{
		for (size_t x = 1; x < width - 1; x++)
		{
			gPrewitt = PrewittKernel(array_RGB_Img, x, y, height, width, 0) / 3;

			red_value = abs(gPrewitt);
			red_value = (red_value < 0) ? 0 : red_value;
			red_value = (red_value > 255) ? 255 : red_value;

			array_RGB_Copy[(y * width + x) * 3 + 0] = red_value;

			

			gPrewitt = PrewittKernel(array_RGB_Img, x, y, height, width, 1) / 3;

			green_value = abs(gPrewitt);
			green_value = (green_value < 0) ? 0 : green_value;
			green_value = (green_value > 255) ? 255 : green_value;

			array_RGB_Copy[(y * width + x) * 3 + 1] = green_value;

			

			gPrewitt = PrewittKernel(array_RGB_Img, x, y, height, width, 2) / 3;

			blue_value = abs(gPrewitt);
			blue_value = (blue_value < 0) ? 0 : blue_value;
			blue_value = (blue_value > 255) ? 255 : blue_value;

			array_RGB_Copy[(y * width + x) * 3 + 2] = blue_value;
		}
	}
}

void GUIMyFrame1::m_b_threshold_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	unsigned char* array_RGB_Img = Img_Cpy.GetData();
	size_t size = 3 * Img_Cpy.GetWidth() * Img_Cpy.GetHeight(); //3 - one pixel have 3 colors


	for (size_t i = 0; i < size; i++)
	{
		if (array_RGB_Img[i] > 128)
		{
			array_RGB_Img[i] = 255;
		}
		else
		{
			array_RGB_Img[i] = 0;
		}
	}
}


void GUIMyFrame1::Contrast(int value)
{
	Img_Cpy = Img_Org.Copy();
	unsigned char* array_RGB_Img = Img_Cpy.GetData();
	size_t size = 3 * Img_Cpy.GetWidth() * Img_Cpy.GetHeight(); //3 - one pixel have 3 colors

	value = value * 255 / 100;
	double contrast_improvement_factor = (259.0 * (255.0 + value)) / (255.0 * (259.0 - value));

	for (size_t i = 0; i < size; i++)
	{
		int changed_color = contrast_improvement_factor * (array_RGB_Img[i] - 128) + 128;

		if (changed_color > 255)
		{
			array_RGB_Img[i] = 255;
		}
		else if (changed_color < 0)
		{
			array_RGB_Img[i] = 0;
		}
		else
		{
			array_RGB_Img[i] = changed_color;
		}
	}
}

void GUIMyFrame1::Repaint()
{
 wxBitmap bitmap(Img_Cpy);          // Tworzymy tymczasowa bitmape na podstawie Img_Cpy
 wxClientDC dc1(m_scrolledWindow);   // Pobieramy kontekst okna
 wxBufferedDC dc(&dc1);
 dc.Clear();
 m_scrolledWindow->DoPrepareDC(dc); // Musimy wywolac w przypadku wxScrolledWindow, zeby suwaki prawidlowo dzialaly
 dc.DrawBitmap(bitmap, 0, 0, true); // Rysujemy bitmape na kontekscie urzadzenia
}

void GUIMyFrame1::Brightness(int value)
{
	Img_Cpy = Img_Org.Copy();
	unsigned char* array_RGB_Img = Img_Cpy.GetData();
	size_t size = 3 * Img_Cpy.GetWidth() * Img_Cpy.GetHeight(); //3 - one pixel have 3 colors

	for (size_t i = 0; i < size; i++)
	{
		if (array_RGB_Img[i] + value > 255)
		{
			array_RGB_Img[i] = 255;
		}
		else if (array_RGB_Img[i] + value < 0)
		{
			array_RGB_Img[i] = 0;
		}
		else
		{
			array_RGB_Img[i] = array_RGB_Img[i] + value;
		}
	}
}