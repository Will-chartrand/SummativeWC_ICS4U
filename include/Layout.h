#ifndef LAYOUT_H
#define LAYOUT_H

#include <iostream>

class Layout
{
    public:
        Layout();
        Layout(int, int, int, int);
        virtual ~Layout();

        std::wstring mapString;

        int getWidth();
        int getHeight();
        void displayLayout(char [], int&, int&);


    protected:

    private:
        int width;
        int height;
        int xpos;
        int ypos;
};

#endif // LAYOUT_H
