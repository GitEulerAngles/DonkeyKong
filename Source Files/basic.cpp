#include "basic.h"
//Vector2
template<typename type_v2>
type_v2 normalize(type_v2 i, bas::vector2 <type_v2> v) {
    if (i < v.x)
        i = v.x;
    else if (i > v.y)
        i = v.y;
    return i;
}
template<typename type_v2> bas::vector2 <type_v2> normVectors2(bas::vector2 <type_v2> v, SDL_Rect barrier) {
    v.x = normalize(v.x, { barrier.x, barrier.w });
    v.y = normalize(v.y, { barrier.y, barrier.h });

    return v;
}
bas::vector2 <int> getFrameCoordinate(int Frame, bas::vector2 <int> FrameDimensions, SDL_Rect Image) {
    float width = (float)Image.w / FrameDimensions.x;
    float coord = (float)Frame / width;
    int frameY = floor(coord) * FrameDimensions.y;
    float height = (float)frameY / Image.h;
    bas::vector2<int> vector2Coord((coord - floor(coord)) * width, (height - floor(height)) * Image.h);
    return bas::vector2<int>(vector2Coord.x * FrameDimensions.x + Image.x, vector2Coord.y + Image.y);
}
//Textures
void PutPixel32_nolock(SDL_Surface* surface, int x, int y, Uint32 color)
{
    Uint8* pixel = (Uint8*)surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint32));
    *((Uint32*)pixel) = color;
}
void PutPixel32(SDL_Surface* surface, int x, int y, Uint32 color)
{
    if (SDL_MUSTLOCK(surface))
        SDL_LockSurface(surface);
    PutPixel32_nolock(surface, x, y, color);
    if (SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);
}
void DrawLine(int x0, int y0, int x1, int y1, SDL_Surface* surface) {
    bas::vector2<int> normalizedVec1 = normVectors2<int>(bas::vector2<int>(x0, y0), { 0,0,32,48 });
    bas::vector2<int> normalizedVec2 = normVectors2<int>(bas::vector2<int>(x1, y1), { 0,0,32,48 });

    x0 = normalizedVec1.x; y0 = normalizedVec1.y;
    x1 = normalizedVec2.x; y1 = normalizedVec2.y;

    int dx, dy, p, x, y;

    dx=x1-x0;
    dy=y1-y0;

    x=x0;
    y=y0;

    p=2*dy-dx;

    while (x<x1)
    {
        if (p>=0)
        {
            PutPixel32_nolock(surface, x, y, 0x000000);
            y++;
            p=p+2*dy-2*dx;
        }
        else
        {
            PutPixel32_nolock(surface, x, y, 0x000000);
            p=p+2*dy;
        }
        x++;
    }
}
void DrawCircle(SDL_Surface* surface, int x, int y, int radius) {
    int offsetx, offsety, d;

    offsetx = 0;
    offsety = radius;
    d = radius -1;

    while (offsety >= offsetx) {

        DrawLine(x - offsety, y + offsetx, x + offsety, y + offsetx, surface);
        DrawLine(x - offsetx, y + offsety, x + offsetx, y + offsety, surface);
        DrawLine(x - offsetx, y - offsety, x + offsetx, y - offsety, surface);
        DrawLine(x - offsety, y - offsetx, x + offsety, y - offsetx, surface);

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

SDL_Texture* bas::texture::loadTexture(std::string path, SDL_Renderer* r) {
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return newTexture;
    }
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(r, loadedSurface);
    if (newTexture == NULL)
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return newTexture;
}
SDL_Texture* bas::dynamictexture::loadTexture(std::string path, SDL_Renderer* r) {
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return newTexture;
    }

    if (reFrame) {
        DrawCircle(loadedSurface, circleDeletion.x, circleDeletion.y, circleDeletion.z);
        reFrame = false;
    }

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(r, loadedSurface);
    if (newTexture == NULL)
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return newTexture;
}
void         bas::texture::loadMedia(SDL_Renderer* r, std::string path) {
    //Load PNG texture
    gTexture = loadTexture(path, r);
    if (gTexture == NULL)
    {
        printf("Failed to load texture image!\n");
    }
}
void         bas::dynamictexture::loadMedia(SDL_Renderer* r, std::string path) {
    //Load PNG texture
    gTexture = loadTexture(path, r);
    if (gTexture == NULL)
    {
        printf("Failed to load texture image!\n");
    }
}
void         bas::texture::drawTexture(SDL_Renderer* r) {
    SDL_RenderCopy(r, gTexture, &imageCoords, &screenCoords);
}
void         bas::dynamictexture::drawTexture(SDL_Renderer* r) {
    SDL_RenderCopy(r, gTexture, &imageCoords, &screenCoords);
    SDL_DestroyTexture(gTexture);
}
void         bas::animatedtexture::updateAnimation() {
    if (seconds.getTime() < secondsNextFrame)
        return;

    seconds.restartTime();
    Frame++;

    if (Frame >= numOfFrames)
        Frame = 0;

    vector2<int> coords = getFrameCoordinate(Frame, frameWidth, animationCoords);

    setImageCoords({ coords.x, coords.y, frameWidth.x, frameWidth.y });
}
