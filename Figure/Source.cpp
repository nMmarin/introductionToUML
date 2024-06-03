#include<Windows.h>
#include<iostream>
using namespace std;

//#define MIN_SIZE 50
//#define MAX_SIZE 800
//#define MIN_LINE_WIDTH 1
//#define MAX_LINE_WIDTH 25
//
//#define MAX_HORIZONTAL_RESOLUTION 800
//#define MAX_VERTICAL_RESOLUTION 600
 
namespace MyGeometry
{
	enum Color
	{
		RED = 0x000000FF,
		GREEN = 0x0000FF00,
		BLUE = 0x00FF0000,
		GREY = 0x00AAAAAA,
		YELLOW = 0x0000FFFF
	};
	class Shape
	{
	protected:
		//еще 1 способ инициализации
		static const int MIN_SIZE = 50;
		static const int MAX_SIZE = 800;
		static const int MIN_LINE_WIDTH = 1;
		static const int MAX_LINE_WIDTH = 25;
		static const int MAX_HORIZONTAL_RESOLUTION =800;
		static const int MAX_VERTICAL_RESOLUTION = 600;
		//

		unsigned int x;
		unsigned int y;
		unsigned int line_width;
		Color color;
	public:
		
		unsigned int get_x()const
		{
			return x;
		}
		unsigned int get_y()const
		{
			return y;
		}
		unsigned int get_line_width()const
		{
			return line_width;
		}
		unsigned int  set_size(unsigned int size)
		{
			return size<MIN_SIZE ? MIN_SIZE :
				size>MAX_SIZE ? MAX_SIZE :
				size;
		}
		void set_x(unsigned int x)
		{
			this->x = x < MAX_HORIZONTAL_RESOLUTION ? x : MAX_HORIZONTAL_RESOLUTION;
		}void set_y(unsigned int y)
		{
			this->y = y < MAX_VERTICAL_RESOLUTION ? y : MAX_VERTICAL_RESOLUTION;
		}
		void set_line_width(unsigned int line_width)
		{
			if (line_width < MIN_LINE_WIDTH)line_width = MIN_LINE_WIDTH;
			if (line_width > MAX_LINE_WIDTH)line_width = MAX_LINE_WIDTH;
			this->line_width = line_width;
		}
		//constructor
		Shape(unsigned int x, unsigned int y, unsigned line_width = 5, Color color = Color::GREY) :color(color)
		{
			set_x(x);
			set_y(y);
			set_line_width(line_width);
			this;
		}
		virtual ~Shape() {}
		//methods
		virtual double get_area()const = 0;
		virtual double get_perimetr()const = 0;
		virtual void draw()const = 0;
		virtual void info()const
		{
			cout << "Площадь фигуры: " << get_area() << endl;
			cout << "Периметр фигуры: " << get_perimetr() << endl;
			draw();
		}
	};
	class Rectangle :public Shape
	{
		double width;
		double height;
	public:
		double get_width()const
		{
			return width;
		}
		double get_height()const
		{
			return height;
		}
		void set_width(double width)
		{
			this->width = set_size(width);
		}
		void set_height(double height)
		{
			this->height =set_size(height);
		}
		//constructors:
		Rectangle(double width, double height, unsigned int x, unsigned int y, unsigned line_width, Color color) :
			Shape(x, y, line_width, color)
		{
			set_width(width);
			set_height(height);
			this;
		}
		~Rectangle() {};
		//methods:
		double get_area()const override
		{
			return width * height;
		}
		double get_perimetr()const override
		{
			return (width + height) * 2;
		}
		void draw()const override
		{
			/*for (size_t i = 0; i < height; i++)
			{
				for (size_t j = 0; j < width; j++)
				{
					cout << "*";
				}
				cout << endl;
			}*/
			//HWND-handler to window(дискриптор окна, нужен для того чтобы обращатьсч к окну) 
			HWND hwnd = GetConsoleWindow();//получаем дискриптор окна консоли.

			//для того, что бы рисовать, нужен контекст ус-ва(Dvice context,он етсь у каждого окна
			HDC hdc = GetDC(hwnd);
			//hdc-это то на чем мы будем рисовать.
			
			//Теперь нужно то,чем мы будем рисвоать:
			HPEN hPen = CreatePen(PS_SOLID,line_width,color);//карандаш рисует контур фигуры
			HBRUSH hBrush = CreateSolidBrush(color);//кисть заливает фигуру 
			//теперь нужно выбрать, чем мы будем рисовать:
			SelectObject(hdc, hPen);
			SelectObject(hdc, hBrush);
			::Rectangle(hdc, x, y, x + width, y + height);
			//удаляем объекты
			DeleteObject(hPen);
			DeleteObject(hBrush);
			ReleaseDC(hwnd, hdc);
			
		}
		void info()const override
		{
			cout << typeid(*this).name() << endl;
			cout << "Ширина: " << width << endl;
			cout << "Высота: " << height << endl;
			Shape::info();
		}
	};
}
void main()
{
	setlocale(LC_ALL, "");
	MyGeometry::Rectangle rect(100, 50, 100, 200, 8, MyGeometry::Color::RED);
	/*cout << "Ширина прямоугольника: " << rect.get_width() << endl;
	cout << "Высота прямоугольника: " << rect.get_height() << endl;
	cout << "Площадь прямоугольника: " << rect.get_area() << endl;
	cout << "Периметр прямоугольника: " << rect.get_perimetr() << endl;
	rect.draw();*/
	rect.info();
}