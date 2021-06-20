#include "FontLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stb_truetype.h>
#include <stb_image_write.h>
#include <algorithm>

namespace Engine {

    std::unordered_map<std::string, Ref<Font>> FontLoader::fonts;

    Ref<Font> FontLoader::LoadFont(const std::string& fontname, const std::string& fontPath)
    {
        /* load font file */
        long size;
        unsigned char* fontBuffer;

        FILE* fontFile = fopen(fontPath.c_str(), "rb");
        fseek(fontFile, 0, SEEK_END);
        size = ftell(fontFile); /* how long is the file ? */
        fseek(fontFile, 0, SEEK_SET); /* reset */

        fontBuffer = (unsigned char*) malloc(size);

        fread(fontBuffer, size, 1, fontFile);
        fclose(fontFile);

        // prepare font texture    
        stbtt_fontinfo fontInfo;
        stbtt_InitFont(&fontInfo, fontBuffer, 0);
        unsigned char temp_bitmap[512 * 512];

        stbtt_pack_context packContext;

        int result = stbtt_PackBegin(&packContext, temp_bitmap, 512, 512, 0, 1, &packContext);

        stbtt_packedchar packedChars[96];
        stbtt_PackFontRange(&packContext, fontBuffer, 0, 100, 32, 96, packedChars);

        FontEntry fontEntries[96];

        float sSize = 1.f / 512.f;
        float tSize = 1.f / 512.f;

        float maxDistance = 0;

        for (size_t i = 0; i < 96; i++)
        {
            fontEntries[i].character = i + 32;
            fontEntries[i].advance = packedChars[i].xadvance;
            fontEntries[i].s0 = packedChars[i].x0 * sSize;
            fontEntries[i].s1 = packedChars[i].x1 * sSize;
            fontEntries[i].t0 = packedChars[i].y0 * tSize;
            fontEntries[i].t1 = packedChars[i].y1 * tSize;

            fontEntries[i].advance = packedChars[i].xadvance;
            
            fontEntries[i].x0 = packedChars[i].x0;
            fontEntries[i].x1 = packedChars[i].x1;
            fontEntries[i].y0 = packedChars[i].y0;
            fontEntries[i].y1 = packedChars[i].y1;

            fontEntries[i].xOff0 = packedChars[i].xoff;
            fontEntries[i].xOff1 = packedChars[i].xoff2;
            fontEntries[i].yOff0 = packedChars[i].yoff;
            fontEntries[i].yOff1 = packedChars[i].yoff2;

            maxDistance = std::max(packedChars[i].yoff2 - packedChars[i].yoff, maxDistance);
        }

        auto atlas = CreateRef<Texture>(temp_bitmap, 512, 512, TextureFormat::RED);

        auto fontRef = CreateRef<Font>(atlas, fontEntries, 96);

        fontRef->maxHeightDistance = maxDistance;

        stbtt_PackEnd(&packContext);
        free(fontBuffer);

        fonts.insert({ fontname, fontRef });

        return fontRef;
    }

    Ref<Font> FontLoader::GetFont(const std::string& fontname) {
        return fonts[fontname];
    }


    Font::Font(Ref<Texture> atlas, FontEntry* fontEntries, int numOfEntries)
    {
        for (size_t i = 0; i < numOfEntries; i++)
        {
            entries[fontEntries[i].character] = fontEntries[i];
        }

        fontAtlas = atlas;
    }

    FontEntry& Font::GetCharEntry(unsigned char c)
    {
        return entries[c];
    }

    Ref<Texture> Font::GetAtlasTexture()
    {
        return fontAtlas;
    }

    float Font::GetScaleForSize(int fontSize) {
        return fontSize / maxHeightDistance;
    }

}