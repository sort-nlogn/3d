#pragma once

void fill_flat_bottom(double v1[], double v2[], double v3[]){
    double slope1 = (v2[0] - v1[0]) / (v2[1] - v1[1]);
    double slope2 = (v3[0] - v1[0]) / (v3[1] - v1[1]);

    double x1 = v1[0], x2 = v1[0];

    for(int y = v1[1]; y <= v2[1]; y++){
        line(x1, y, x2, y);
        x1 += slope1;
        x2 += slope2;
    }
}

void fill_flat_top(double v1[], double v2[], double v3[]){
    double slope1 = (v3[0] - v1[0]) / (v3[1] - v1[1]);
    double slope2 = (v3[0] - v2[0]) / (v3[1] - v2[1]);

    double x1 = v3[0], x2 = v3[0];

    for(int y = v3[1]; y >= v1[1]; y--){
        line(x1, y, x2, y);
        x1 -= slope1;
        x2 -= slope2;
    }
}

void rasterize_triangle(double v1[], double v2[], double v3[]){
    if(v1[1] > v2[1])
        swap(v1, v2);
    if(v1[1] > v3[1])
        swap(v1, v3);
    if(v2[1] > v3[1])
        swap(v2, v3);

    if(v1[1] == v2[1]){
        fill_flat_top(v1, v2, v3);
    }else if(v2[1] == v3[1]){
        fill_flat_bottom(v1, v2, v3);
    }else{
        double v4[] = {v1[0] + ((v2[1] - v1[1]) / 
                       (v3[1] - v1[1])) * (v3[0] - v1[0]), 
                       v2[1]};
        fill_flat_bottom(v1, v2, v4);
        fill_flat_top(v2, v4, v3);
    }
}