#include <iostream>
#include <graphics.h>
#include "quat.hpp"
#include "rasterizer.hpp"

#define f 100.0
#define n 1.0
#define a -(f + n) / (f - n)
#define b -2*f*n / (f - n)

#define num_vertices 6
#define num_faces 8
#define sc(x) (int)round(x)

#define width 620
#define height 480

void cross(float u[], float v[], float *buff){
    buff[0] = u[1] * v[2] - v[1] * u[2];
    buff[1] = v[0] * u[2] - u[0] * v[2];
    buff[2] = u[0] * v[1] - v[0] * u[1];
}

float dot(float u[], float v[]){
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

bool is_visible(float p1[], float p2[], float p3[]){ // from origin
    float v1[3] = {p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]};
    float v2[3] = {p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2]};
    float to_origin[3] = {-p1[0], -p1[1], -p1[2]}, buff[3] = {0.0};
    cross(v1, v2, buff);
    return dot(buff, to_origin) > 0;
}

void rasterize(float pos[], int ind[], bool visible[], int col[]){
    for(int i = 0; i < num_vertices; i++){
        pos[3 * i] = (pos[3 * i] * 0.5 + 0.5) * width;
        pos[3*i + 1] = (pos[3*i + 1] * 0.5 + 0.5) * height;
    }

    for(int i = 0; i < num_faces; i++){
        if(!visible[i])
            continue;
        float v1[2], v2[2], v3[2];
        v1[0] = pos[3 * ind[3 * i]];
        v1[1] = pos[3 * ind[3 * i] + 1];

        v2[0] = pos[3 * ind[3 * i + 1]];
        v2[1] = pos[3 * ind[3 * i + 1] + 1];

        v3[0] = pos[3 * ind[3 * i + 2]];
        v3[1] = pos[3 * ind[3 * i + 2] + 1];

        int shape[12] = {sc(v1[0]), sc(v1[1]),
                         sc(v2[0]), sc(v2[1]),
                         sc(v3[0]), sc(v3[1]),
                         sc(v1[0]), sc(v1[1])};

        setfillstyle(SOLID_FILL, col[i]);
        fillpoly(4, shape);

    }
}

void cull_faces(float pos[], int ind[], bool visible[]){
    for(int i = 0; i < num_faces; i++){
        float p1[3], p2[3], p3[3];
        p1[0] = pos[3 * ind[3 * i]];
        p1[1] = pos[3 * ind[3 * i] + 1];        
        p1[2] = pos[3 * ind[3 * i] + 2];

        p2[0] = pos[3 * ind[3 * i + 1]];
        p2[1] = pos[3 * ind[3 * i + 1] + 1];
        p2[2] = pos[3 * ind[3 * i + 1] + 2];

        p3[0] = pos[3 * ind[3 * i + 2]];
        p3[1] = pos[3 * ind[3 * i + 2] + 1];
        p3[2] = pos[3 * ind[3 * i + 2] + 2];
        if(is_visible(p1, p2, p3))
            visible[i] = true;
    }
}

void vertex(float coords[], int indices[], int colors[]){
    float pos[3 * num_vertices];
    bool visible[num_faces] = {false};
    cull_faces(coords, indices, visible);
    for(int i = 0; i < num_vertices; i++){
        float x = coords[3*i], y = coords[3*i+1], z = coords[3*i+2];
        x = n * x / -z;
        y = n * y / -z;
        z = (a * z + b) / -z;
        pos[3 * i] = x, pos[3 * i + 1] = y, pos[3 * i + 2] = z; 
    }
    rasterize(pos, indices, visible, colors);
}

void process_keyboard(float coords[], float c[]){
    char ch = (char)getch();
    float axis[3] = {0.0};
    float theta = 0.1;

    if(ch == 75 || ch == 77){
        axis[1] = 1.0;
        if(ch == 77)
            theta *= -1.0;
    }else if(ch == 72 || ch == 80){
        axis[0] = 1.0;
        if(ch == 80)
            theta *= -1.0; 
    }else{
        return;
    }

    for(int i = 0; i < num_vertices; i++){
        float v[3] = {coords[3 * i] - c[0],
            coords[3 * i + 1] - c[1],
            coords[3 * i + 2] - c[2]};

        q_rotate(v, theta, axis, v);

        coords[3 * i] = c[0] + v[0];
        coords[3 * i + 1] = c[1] + v[1];
        coords[3 * i + 2] = c[2] + v[2];
    }
}

int main(){
    int win = initwindow(620, 480, "3d");

    float coords[] = {-0.5, -0.5, -2.0,
                      0.5, -0.5, -2.0,
                      0.0, -0.5, -2.8,
                     -0.5,  0.5, -2.0,
                      0.5,  0.5, -2.0,
                      0.0,  0.5, -2.8};

    int indices[] = {2, 1, 0, 0, 4, 3,
                     0, 1, 4, 2, 0, 5,
                     0, 3, 5, 1, 2, 4,
                     4, 2, 5, 3, 4, 5};

    int colors[] = {RGB(255, 0, 0), RGB(255, 156, 0),
                    RGB(255, 255, 0), RGB(0, 255, 0),
                    RGB(0, 191, 255), RGB(0, 0, 255),
                    RGB(128, 0, 255), RGB(255, 0, 0)};
 
    float centroid[3] = {-0.5, -0.5, -2.0};

    while(1){
        vertex(coords, indices, colors);
        if(kbhit()){
            process_keyboard(coords, centroid);
        }
        swapbuffers();
        clearviewport();
        Sleep(1);
    }

    return 0;
}