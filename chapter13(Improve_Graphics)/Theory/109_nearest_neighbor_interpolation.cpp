/*
    최근접 이웃 필터링(nearest neighbor filtering)/ 최근접 이웃 보간법(nearest neighbor interpolation)

    높은 텍셀 밀도 때문에 텍셀이 소실된 것처럼 보이는 이유

    텍스처는 왼쪽 상단 구석(0,0) 그리고 오른쪽 하단 구석 (1,1)의 범위를 갖는 UV좌푤르 사용한다.
    4x4 텍셀로 구성된 사각형 텍스처가 있다면, 왼쪽 상당 텍셀의 중심 UV 좌표는 (0.125, 0.125)다.
    텍스처의 중심은 (0.5, 0.5) uv좌표에 속한다.

    1:2의 텍셀 밀도로 텍스처 영역 (0, 0)부터 (0.5, 0.5)까지를 그린다면
    텍스처의 4분의 1이 화면상에서 2배 크기로 나타나는 것을 의미한다.

    프래그먼트 셰이더는 화면을 그릴 떄 각 픽셀의 중심에 해당하는 uv 좌표를 얻는다.
    그럼 이제 왼쪽 상단 픽셀은 텍스처 uv 좌표 (0.0625, 0.0625)를 샘플링한다.
    그러나 원본 이미지에선 어떤 텍셀의 중심값도 이 좌표에 직접 해당되지 않는다.
    이 때문에 필터링 알고리즘을 사용한다.

    필터링 알고맂므은 uv 좌표 값으로 어떤 색을 거려야 하는지를 선택하는데 도움을 준다.

    최근접 이웃 필터링에서는 해당 uv 좌표가 가장 가까운 텍셀을 선택한다.
    ex) (0.0625, 0.0625) 좌표는 (0.125, 0.125)에 있는 텍셀과 가장 가까우므로 최근접
        이웃 필터링은 해당 픽셀에 대해 이 텍셀의 색상을 선택한다.

    결국 모든 텍셀은 텍셀 밀도에 비례해서 재조정된다.
    최근접 이웃 필터링에서 화면상의 텍스처 크기를 늘리면 인식되는 각 텍셀의 크기도 커져서
    이미지의 픽셀화를 쉽게 확인할 수 있다.
*/
#include <vector>
std::vector<double> interp1(std::vector<double> &x, std::vector<double> &y, std::vector<double> &x_new)
{

    std::vector<double> y_new;
    y_new.reserve(x_new.size());

    std::vector<double> dx, dy, slope, intercept;
    dx.reserve(x.size());
    dy.reserve(x.size());
    slope.reserve(x.size());
    intercept.reserve(x.size());
    for (int i = 0; i < x.size(); ++i)
    {
        if (i < x.size() - 1)
        {
            dx.push_back(x[i + 1] - x[i]);
            dy.push_back(y[i + 1] - y[i]);
            slope.push_back(dy[i] / dx[i]);
            intercept.push_back(y[i] - x[i] * slope[i]);
        }
        else
        {
            dx.push_back(dx[i - 1]);
            dy.push_back(dy[i - 1]);
            slope.push_back(slope[i - 1]);
            intercept.push_back(intercept[i - 1]);
        }
    }

    for (int i = 0; i < x_new.size(); ++i)
    {
        int idx = findNearestNeighbourIndex(x_new[i], x);
        y_new.push_back(slope[idx] * x_new[i] + intercept[idx]);
    }

    return y_new;
}

int findNearestNeighbourIndex(double value, std::vector<double> &x)
{
    float dist = FLT_MAX;
    int idx = -1;
    for (int i = 0; i < x.size(); ++i)
    {
        float newDist = value - x[i];
        if (newDist > 0 && newDist < dist)
        {
            dist = newDist;
            idx = i;
        }
    }

    return idx;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef uint32_t Pixel;

typedef struct {
    uint32_t width, height;
    Pixel **pixels;
} Image;

void makeBigger(Image *img, int scale)
{
    uint32_t i = 0, j = 0;
    Image *tmp = (Image*)malloc(sizeof(Image));
    tmp->height = img->height * scale;
    tmp->width = img->width * scale;

    tmp->pixels = (Pixel**)malloc(sizeof(Pixel*) * tmp->height);
    for (i = 0; i < tmp->height; i++) {
        tmp->pixels[i] = (Pixel*)malloc(sizeof(Pixel) * tmp->width);
        for (j = 0; j < tmp->width; j++) {
            tmp->pixels[i][j] = img->pixels[i / scale][j / scale];
        }
    }

    for (i = 0; i < img->height; i++)
        free(img->pixels[i]);
    free(img->pixels);

    img->width = tmp->width;
    img->height = tmp->height;
    img->pixels = tmp->pixels;
    free(tmp);
}


void printImage(Image *img)
{
    printf("Width: %d, Height: %d\n", img->width, img->height);
    for (uint32_t i = 0; i < img->height; i++) {
        for (uint32_t j = 0; j < img->width; j++)
            printf("%3d", img->pixels[i][j]);
        printf("\n");
    }
    printf("\n");
}


int main()
{
    uint32_t i = 0, j = 0, k = 1;
    Image img;
    // Initialize the image
    img.height = 2;
    img.width = 3;
    img.pixels = (Pixel**)malloc(sizeof(Pixel*) * img.height);
    for (i = 0; i < img.height; i++) {
        img.pixels[i] = (Pixel*)malloc(sizeof(Pixel) * img.width);
        for (j = 0; j < img.width; j++)
            img.pixels[i][j] = k++;
    }

    printImage(&img);
    makeBigger(&img, 2);
    printImage(&img);

    // Destroy the image
    for (i = 0; i < img.height; i++)
        free(img.pixels[i]);
    free(img.pixels);

    printf("\nDone.\n");
    return 0;
}