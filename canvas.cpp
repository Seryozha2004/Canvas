#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

const int CANVAS_HEIGHT = 20;
const int CANVAS_WIDTH = 60;

class Shape {
public:	
	virtual ~Shape() {}
	virtual void Draw(vector<string>& grid) const = 0;
};

class Rectangle : public Shape {
private:
	int x, y, w, h;
public:
	Rectangle(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
	void Draw(vector<string>& grid) const override 
	{
		for(int i=y; i<y+h; i++) 
		{
			if(i<0 || i>=(int)grid.size()) continue;
			for(int j=x; j<x+w; j++) 
			{
				if(j<0 || j>=(int)grid[0].size()) continue;
				grid[i][j]='#';
			}
		}
	}
};

class Circle : public Shape {
private:
	int x, y, radius;
public:
	Circle(int x, int y, int radius) : x(x), y(y), radius(radius) {}
	void Draw(vector<string>& grid) const override 
	{
		for(int row=0; row<(int)grid.size(); row++)
		{
			for(int col=0; col<(int)grid[0].size(); col++)
			{
				double dx = x - col;
				double dy = y - row;
				double dist = sqrt(dx * dx + dy * dy);
				if(dist<=radius) 
				{
					grid[row][col] = 'o';
				}
			}
		}
	}	
};

class Cross : public Shape {
private:
	int x, y, size;
public:
	Cross(int x, int y, int size) : x(x), y(y), size(size) {}
	void Draw(vector<string>& grid) const override
	{
		for(int dy=-size; dy<=size; dy++)
		{
			int r = y + dy;
			int c = x;
			if(r>=0 && r<(int)grid.size() && c>=0 && c<(int)grid[0].size())
			{
				grid[r][c] = '+';
			}
		}
		for(int dx=-size; dx<=size; dx++)
		{
			int r = y;
			int c = x + dx;
			if(r>=0 && r<(int)grid.size() && c>=0 && c<(int)grid[0].size())
			{
				grid[r][c] = '+';
			}
		}
	}
};

class Canvas {
private:
	int width, height;
	vector<Shape*> shapes;
public:
	Canvas(int w = CANVAS_WIDTH, int h = CANVAS_HEIGHT) : width(w), height(h) {}
	~Canvas() 
	{
		for(auto s : shapes) delete s;
	}
	void AddShape(Shape* s) 
	{
		shapes.push_back(s);
	}
	void Draw() const 
	{
		vector<string> grid(height, string(width, '.'));
		for(auto s : shapes) 
		{
			s->Draw(grid);
		}
		for(const string& row : grid) 
		{
			cout<<row<<endl;
		}
	}
};

vector<string> split(const string& s)
{
	vector<string> tokens;
	int pos = 0;
	while(true)
	{
		while(pos<s.size() && s[pos]==' ') pos++;
		if(pos >= s.size()) break;
		int start = pos;
		while(pos<s.size() && s[pos]!=' ') pos++;
		tokens.push_back(s.substr(start, pos-start));
	}
	return tokens;
}

int main()
{
	Canvas canvas;
	string line;
	cout<<"Commands:\n"
	    <<" add rectangle x y width height\n"
	    <<" add circle x y radius\n"
	    <<" add cross x y size\n"
	    <<" show\n"
	    <<" quit\n";

	while(true)
	{
		cout<<"> ";
		if(!getline(cin, line)) break;
		auto words = split(line);
		if(words.empty()) continue;
		string cmd=words[0];
		if(cmd == "quit" || cmd == "exit") break;
		else if(cmd == "add")
		{
			if(words.size() < 2)
			{
				cout<<"Usage: add [rectangle | circle | cross] parameters\n";
				continue;
			}
			string shape = words[1];
			if(shape == "rectangle")
			{
				if(words.size() != 6)
				{
					cout<<"Usage: add rectangle x y width height\n";
					continue;
				}
				int x = stoi(words[2]);
				int y = stoi(words[3]);
				int w = stoi(words[4]);
				int h = stoi(words[5]);
				if(w<=0 || h<=0)
				{
					cout<<"Width and Height must be positive\n";
					continue;
				}
				canvas.AddShape(new Rectangle(x, y, w, h));
			}
			else if(shape == "circle")
			{
				if(words.size() != 5)
				{
					cout<<"Usage: add circle x y radius\n";
					continue;
				}
				int x = stoi(words[2]);
				int y = stoi(words[3]);
				int r = stoi(words[4]);
				if(r<=0)
				{
					cout<<"Radius must be positive\n";
					continue;
				}
				canvas.AddShape(new Circle(x, y ,r));
			}
			else if(shape == "cross")
			{
				if(words.size() != 5)
				{
					cout<<"Usage: add cross x y size\n";
					continue;
				}
				int x = stoi(words[2]);
				int y = stoi(words[3]);
				int s = stoi(words[4]);
				if(s<=0)
				{
					cout<<"Size must be positive\n";
					continue;
				}
				canvas.AddShape(new Cross(x, y, s));
			}
			else
			{
				cout<<"Unknown shape\n";
				continue;
			}

		}
		else if(cmd == "show") canvas.Draw();
		else 
		{
			cout<<"Unknown command\n";
			continue;
		}
	}
	return 0;
}
