
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
void clrscr()
{
    system("@cls||clear");
}
// Define constants

#define PI acos(-1.00) // Value of PI using acos(-1)
#define TC 2           // Terminal correction factor
#define X TC * 78
// Width of the plane (based on terminal correction)
#define Y 40 // Height of the plane

int plane[Y][X]; // Define the plane array
int ox = X / 2;  // Origin x-coordinate in the plane
int oy = Y / 2;  // Origin y-coordinate in the plane

double shift_x = 0; // X-coordinate shift for the origin
double shift_y = 0; // Y-coordinate shift for the origin
double theta = 0;   // rotate plane by theta;

double scalex = TC * 1; // Scaling factor for the x-axis
double scaley = 1;      // Scaling factor for the y-axis

double get_rotx(double x, double y)
{
    x = x * cos(theta) + y * sin(theta);
    return x;
}

double get_roty(double x, double y)
{

    y = -x * sin(theta) + y * cos(theta);
    return y;
}

// Convert real x-coordinate to the plane array x-index
double getx(double x, double y)
{
    x = get_rotx(x, y);
    double cur_x = ox + (x + shift_x) * scalex;
    return cur_x;
}
// Convert real y-coordinate to the plane array y-index
double gety(double x, double y)
{
    y = get_roty(x, y);
    double cur_y = oy - (y + shift_y) * scaley;
    return cur_y;
}

// Check if the given index is valid
int is_valid(int curx, int cury)
{
    return (curx >= 0 && curx < X && cury >= 0 && cury < Y);
}

// Mark points
void update(double point_x, double point_y)
{
    int plot_x = getx(point_x, point_y);
    int plot_y = gety(point_x, point_y);
    if (is_valid(plot_x, plot_y))
    {

        plane[plot_y][plot_x] = 1;
    }
}

// straight line of y = mx + c
void line(double m, double x, double c, double dx, double dy, double rotate)
{
    double cur_y = m * x + c;
    theta += rotate;
    shift_x += dx;
    shift_y += dy;
    update(x, cur_y);
    shift_x -= dx;
    shift_y -= dy;
    theta -= rotate;
}

// Generate a cosine wave
void cos_wave(double x, double a, double lambda, double delta, double dx, double dy, double rotate)
{
    double k = (2 * PI) / lambda;
    double cur_y = a * cos(k * x + delta * PI / 180);
    theta += rotate;
    shift_x += dx;
    shift_y += dy;
    update(x, cur_y);
    theta -= rotate;
    shift_x -= dx;
    shift_y -= dy;
}

// Generate a sine wave
void sin_wave(double x, double a, double lambda, double delta, double dx, double dy, double rotate)
{
    double k = (2 * PI) / lambda;
    double cur_y = a * sin(k * x + delta * PI / 180);
    theta += rotate;
    shift_x += dx;
    shift_y += dy;
    update(x, cur_y);
    shift_x -= dx;
    shift_y -= dy;
    theta -= rotate;
}

// Draw a circle with center (h, k) and radius r
void circle(double x, double h, double k, double r, double dx, double dy, double rotate)
{
    double r2 = r * r;
    double x_h2 = (x - h) * (x - h);

    if (r2 < x_h2)
    {
        return;
    }

    double pos_root = sqrt(r2 - x_h2);
    double neg_root = -pos_root;

    pos_root += k;
    neg_root += k;
    theta += rotate;
    shift_x += dx;
    shift_y += dy;
    update(x, pos_root);
    update(x, neg_root);
    shift_x -= dx;
    shift_y -= dy;
    theta -= rotate;
}

// parabola opening along the y-axis with vertex (h, k)
void parabolaY(double x, double h, double k, double a, double dx, double dy, double rotate)
{
    double cur_y = a * (x - h) * (x - h) + k;
    theta += rotate;
    shift_x += dx;
    shift_y += dy;
    update(x, cur_y);
    shift_x -= dx;
    shift_y -= dy;
    theta -= rotate;
}

// parabola opening along the x-axis with vertex (h, k)
void parabolaX(double x, double h, double k, double a, double dx, double dy, double rotate)
{
    double x_h_a = (x - h) / a;

    if (x_h_a < 0)
    {
        return;
    }

    double pos_root = sqrt(x_h_a);
    double neg_root = -pos_root;

    pos_root += k;
    neg_root += k;
    theta += rotate;
    shift_x += dx;
    shift_y += dy;
    update(x, pos_root);
    update(x, neg_root);
    shift_x -= dx;
    shift_y -= dy;
    theta -= rotate;
}

// Fill the plane array with zeros to initialize it
void fill_plane()
{
    for (int j = 0; j < Y; j++)
    {
        for (int i = 0; i < X; i++)
        {
            plane[j][i] = 0;
        }
    }
}

// Display the plane array on the terminal
void show_plane(char point, char empty, int show_axis)
{
    char one_line[(Y + 1) * (X + 1)];
    int idx = 0;

    for (int j = 0; j < Y; j++)
    {
        for (int i = 0; i < X; i++)
        {
            if (i == (int)getx(0, 0) && j == (int)gety(0, 0) && show_axis)
            {

                one_line[idx] = '0';
                idx++;
                continue;
            }

            if (plane[j][i] == 1)
            {
                one_line[idx] = point;
                idx++;
            }
            else
            {
                if (i == (int)getx(0, 0) && show_axis)
                {
                    one_line[idx] = '|';
                    idx++;
                    continue;
                }
                if (j == (int)gety(0, 0) && show_axis)
                {
                    one_line[idx] = '-';
                    idx++;
                    continue;
                }
                one_line[idx] = empty;
                idx++;
            }
        }
        one_line[idx] = '\n';
        idx++;
    }
    one_line[idx] = '\0';
    printf("%s", one_line);
}

// Shift the origin by a specified value
void shift_origin(double x, double y)
{
    shift_x += x;
    shift_y += y;
}

// Draw a hyperbola with center (h, k)
void hyperbola(double x, double h, double k, double a, double b, double dx, double dy, double rotate)
{
    double a2 = a * a;
    double b2 = b * b;
    double x_h2 = (x - h) * (x - h);

    if (x_h2 < a2)
    {
        return;
    }

    double y_term = ((x_h2 / a2) - 1) * b2;
    double pos_root = sqrt(y_term);
    double neg_root = -pos_root;

    pos_root += k;
    neg_root += k;
    theta += rotate;
    shift_x += dx;
    shift_y += dy;
    update(x, pos_root);
    update(x, neg_root);
    shift_x -= dx;
    shift_y -= dy;
    theta -= rotate;
}

// Plot a logarithmic curve with scale a and base modifier b
void logarithmic(double x, double a, double b, double dx, double dy, double rotate)
{
    if (x <= 0)
    {
        return;
    }
    double cur_y = a * log(b * x);
    theta += rotate;
    shift_x += dx;
    shift_y += dy;
    update(x, cur_y);
    shift_x -= dx;
    shift_y -= dy;
    theta -= rotate;
}

int main()
{
    printf("Enter to start");
    char _;
    scanf("%c", &_);
    fill_plane();
    double dx = 1 / scalex;

    clock_t t, tmp;
    t = clock();
    int fps = 120;
    double dt = (1.00 / fps);
    double dtheta = 0.1 * PI / 180;
    double cx = 1;
    double cy = 1;
    double dscale = 1.01;
    // theta = PI / 4;
    while (1)
    {
        tmp = clock() - t;
        double time_taken = ((double)tmp) / CLOCKS_PER_SEC;

        if (time_taken >= dt)
        {

            clrscr();
            shift_origin(cx, cy);
            // real time origin shift!

            fill_plane();
            theta -= dtheta;
            // rotation of axis in real time!
            // scalex *= dscale;
            // scaley *= dscale;
            // so many possiblities! let us see, if i can make it 3d! but need to cook some math first! :: bye!!!!
            if ((scalex >= 5 || scaley >= 5) || (scalex <= 1 || scaley <= 1))
            {
                dscale = 1 / dscale;
            }
            if (fabs(shift_x) * 2 >= ox)
            {
                cx *= -1;
                // cy *= -1;
                dtheta *= -1;
            }
            if (fabs(shift_y) >= oy)
            {
                cy *= -1;
                // cx *= -1;
                dtheta *= -1;
            }
            // this is collision! so its off now, now bouncyness
            // now if i toggle the collision mechanism!
            for (double i = -X / 2.0 - shift_x; i < X / 2.0 - shift_x; i += dx)
            {

                int cnt = 5;
                for (int k = 0; k < cnt; k++)
                {

                    circle(i, 0, 0, k, 0, 0, 0);
                    // sin_wave(i, (k + 1) * 4, 20, 0, 0, 0, (180 / cnt) * k * PI / 180);
                    line(0, i, 0, 0, 0, (180 / cnt) * k * PI / 180);
                    // sin_wave(i, 4, 10, 0, 0, 0, 0);
                    // guess what'll happen?
                }
                // Example 1: Line
                // line(0, i, 0, 0, 0, 0);
                // line(0, i, 0, 0, 0, PI / 2);

                // Example 2: Cosine Wave
                // cos_wave(i, 5, 10, 0, 0, 0, 0);
                // i also included coordinate shif!
                // also added axis rotation
                // cosine wave with amplitude 5 , lambda 20 and phase difference 0...

                // Example 3: Sine Wave
                // sin_wave(i, 10, 5, 0, 0, 0, 0);
                // they also overlaps :)
                // Example 4: Circle
                // circle(i, 0, 0, 5, 0, 0, 0);
                // lets make a bouncy ball :)
                // circle :) shift by (10,10)

                // Example 5: Parabola (Y-axis)
                // parabolaY(i, 0, 0, 0.1, 0, 0, 0);
                // parabola!

                // Example 6: Parabola (X-axis)
                // parabolaX(i, 0, 0, 0.1, 0, 0, 0);

                // Example 7: Hyperbola
                // hyperbola(i, 0, 0, 5, 3, 0, 0, 0);
                // hyperbola rotation

                // Example 8: Logarithmic Curve
                // logarithmic(i, 5, 1, 0, 0, 0);
                // now the cool part! they can move :") "
            }
            // dtheta++;
            show_plane('0', ' ', 0); // axis off!
            t = clock();
            printf("x_shif:%lf  y_shift:%lf fps:%lf rotation:%lf", shift_x, shift_y, 1 / time_taken, theta);
        }
    }

    return 0;
}

// console graphics from scratch... no external library used! ::
