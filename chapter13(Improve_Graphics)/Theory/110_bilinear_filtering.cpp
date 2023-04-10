/*
    이중 선형 필터링(bilinear filtering/bilinear interpolation)

    이중 선형 필터링에서는 uv 좌표에 가장 근접한 4개의 텍셀 중심을 찾는다.
    해당 uv 좌표의 샘플링된 색상은 이 가까운 4개 텍셀에 가중치를 부여한 평균값이다.

    이중 선형 필터링은 이미지를 확대할 때 좀 더 부드럽게 전환되지만 너무 확대하면
    이미지는 뿌옇게 보일것이다.
    필터링이 적용된 텍스처를 확대하면 동일한 색상을 가진 이웃 픽셀은 없고 여러 색이 혼합된것이 보인다.

    가중치가 부여된 평균을 계산하려면 색상을 3D 값으로 다루고 보간을 계산해야 한다.
    이중 선형 보간을 2개 별도의 축으로 나눠 진핸한다.

    텍셀 A, B, C, D에 근접한 점 P가 있을 때, u 방향에서 A와 B의 색상을 보간해 R을 얻는다.
    비슷하게 u 방향에서 C와 D 색상 보간을 계산해 S를 얻는다.
    그러면 두 점 R, S에서 색상을 얻는다.
    마지막으로 v 방향에서 R과 S의 색상을 보간해 P 지점의 최종 색상을 얻는다.

    A, B, C, D 및 P에 대한 텍스처 좌표가 주어지면 다음 방정식들을 사용해 이중 선형 필터링 보간을 계산한다.

    uFactor = 1 - (P.u - A.u) / (B.u - A.u) // u 좌표로 x좌표와 비슷, u 요소 방향의 가중치를 결정
    Rcolor = uFactor * Acolor + (1 - uFactor) * Bcolor
    Scolor = uFactor * Ccolor + (1 - uFactor) * Dcolor
    vFactor = 1 - (P.v - A.v) / (C.v - A.v) // v 좌표로 y좌표와 비슷, v 요소 방향의 가중치를 결정
    Pcolor = vFactor * Rcolor + (1 - vFactor) * Scolor, // 최종 색상 계산

    이런 계산은 이중 선형 필터링이 활성화돼 있다면 자동적으로 그래픽 카드에서 수행한다.
    현재 GPU는 초당 수백만 개의 계산을 빠르게 하기에 필터링 계산이 매우 빠르다.
*/

#include <stdint.h>
typedef struct
{
    uint32_t *pixels;
    unsigned int w;
    unsigned int h;
} image_t;
#define getByte(value, n) (value >> (n * 8) & 0xFF)

uint32_t getpixel(image_t *image, unsigned int x, unsigned int y)
{
    return image->pixels[(y * image->w) + x];
}

float max(float a, float b) { return (a < b) ? a : b; };
float lerp(float s, float e, float t) { return s + (e - s) * t; }
float blerp(float c00, float c10, float c01, float c11, float tx, float ty)
{
    return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}
void putpixel(image_t *image, unsigned int x, unsigned int y, uint32_t color)
{
    image->pixels[(y * image->w) + x] = color;
}
void scale(image_t *src, image_t *dst, float scalex, float scaley)
{
    int newWidth = (int)src->w * scalex;
    int newHeight = (int)src->h * scaley;
    int x, y;
    for (x = 0, y = 0; y < newHeight; x++)
    {
        if (x > newWidth)
        {
            x = 0;
            y++;
        }
        // float gx = x / (float)(newWidth) * (src->w - 1);
        // float gy = y / (float)(newHeight) * (src->h - 1);
        //  Image should be clamped at the edges and not scaled.
        float gx = max(x / (float)(newWidth) * (src->w) - 0.5f, src->w - 1);
        float gy = max(y / (float)(newHeight) * (src->h) - 0.5, src->h - 1);
        int gxi = (int)gx;
        int gyi = (int)gy;
        uint32_t result = 0;
        uint32_t c00 = getpixel(src, gxi, gyi);
        uint32_t c10 = getpixel(src, gxi + 1, gyi);
        uint32_t c01 = getpixel(src, gxi, gyi + 1);
        uint32_t c11 = getpixel(src, gxi + 1, gyi + 1);
        uint8_t i;
        for (i = 0; i < 3; i++)
        {
            //((uint8_t*)&result)[i] = blerp( ((uint8_t*)&c00)[i], ((uint8_t*)&c10)[i], ((uint8_t*)&c01)[i], ((uint8_t*)&c11)[i], gxi - gx, gyi - gy); // this is shady
            result |= (uint8_t)blerp(getByte(c00, i), getByte(c10, i), getByte(c01, i), getByte(c11, i), gx - gxi, gy - gyi) << (8 * i);
        }
        putpixel(dst, x, y, result);
    }
}