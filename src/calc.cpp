#include <stdio.h>
#include <immintrin.h>
#include <assert.h>
#include "calc.h"

void
calc_no_avx(sf::Image *image, int x_tl, int y_tl, const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix)
{
        assert(image);
        assert(bk_pix);
        assert(fr_pix);

        for (int y = y_tl; y < (WINDOW_HEIGHT + BACK_HEIGHT) / 2; y++) {
                for (int x = x_tl; x < (WINDOW_WIDTH + BACK_WIDTH) / 2; x++) {
                        sf::Color *fr_c = (sf::Color *) &fr_pix[((y - y_tl) * (int) BACK_WIDTH + (x - x_tl)) * 4];
                        sf::Color *bk_c = (sf::Color *) &bk_pix[(y * (int) WINDOW_WIDTH + x) * 4];
                        sf::Color color;

                        color.r = (bk_c->r * (255 - fr_c->a) + fr_c->r * fr_c->a) >> 8;
                        color.g = (bk_c->g * (255 - fr_c->a) + fr_c->g * fr_c->a) >> 8;
                        color.b = (bk_c->b * (255 - fr_c->a) + fr_c->b * fr_c->a) >> 8;
                        color.a = (bk_c->a * (255 - fr_c->a) + fr_c->a * fr_c->a) >> 8;

                        image->setPixel(x, y, color);
                }
        }
}

const __m256i xff_256i = _mm256_set1_epi16(0xff);

const __m256i al_mask = _mm256_set_epi8(
        // .. A0 .. A0 .. A0 .. A0
        0x80, 14, 0x80, 14, 0x80, 14, 0x80, 14,
        // .. A1 .. A1 .. A1 .. A1
        0x80,  6, 0x80,  6, 0x80,  6, 0x80,  6,
        // .. A2 .. A2 .. A2 .. A2
        0x80, 14, 0x80, 14, 0x80, 14, 0x80, 14,
        // .. A3 .. A3 .. A3 .. A3
        0x80,  6, 0x80,  6, 0x80,  6, 0x80,  6);

const __m256i sum_mask = _mm256_set_epi8(
        15, 13, 11, 9, 7, 5, 3, 1,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        15, 13, 11, 9, 7, 5, 3, 1);
void
calc_avx_4pix(sf::Image *image, int x_tl, int y_tl, const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix)
{
        assert(image);
        assert(bk_pix);
        assert(fr_pix);

        sf::Color sf_color[4];

        for (int y = y_tl; y < (WINDOW_HEIGHT + BACK_HEIGHT) / 2; y++) {
                for (int x = x_tl; x < (WINDOW_WIDTH + BACK_WIDTH) / 2; x += 4) {
                        // Load next 4 pixels.
                        __m256i bk = _mm256_cvtepu8_epi16(_mm_load_si128(
                                (__m128i *) &bk_pix[(y * (int) WINDOW_WIDTH + x) * 4]));
                        __m256i fr = _mm256_cvtepu8_epi16(_mm_load_si128(
                                (__m128i *) &fr_pix[((y - y_tl) * (int) BACK_WIDTH + (x - x_tl)) * 4]));

                        // Extract alpha-channel values.
                        __m256i al_ch = _mm256_shuffle_epi8(fr, al_mask);

                        fr = _mm256_mullo_epi16(fr, al_ch);
                        bk = _mm256_mullo_epi16(bk, _mm256_sub_epi16(xff_256i, al_ch));

                        __m256i sum = _mm256_add_epi16(fr, bk);

                        sum = _mm256_shuffle_epi8(sum, sum_mask);

                        __m128i color = _mm_add_epi8(
                                _mm256_extracti128_si256(sum, 1), _mm256_extracti128_si256(sum, 0));

                        _mm_store_si128 ((__m128i *) sf_color, color);
                        for (int i = 0; i < 4; i++) {
                                image->setPixel(x + i, y, sf_color[i]);
                        }
                }
        }
}

void
calc_avx_8pix(sf::Image *image, int x_tl, int y_tl, const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix)
{
        assert(image);
        assert(bk_pix);
        assert(fr_pix);

        sf::Color sf_color[8];

        for (int y = y_tl; y < (WINDOW_HEIGHT + BACK_HEIGHT) / 2; y++) {
                for (int x = x_tl; x < (WINDOW_WIDTH + BACK_WIDTH) / 2 - 4; x += 8) {
                        // Load next 8 pixels.
                        __m256i bk_l = _mm256_loadu_si256(
                                (__m256i *) &bk_pix[(y * (int) WINDOW_WIDTH + x) * 4]);
                        __m256i fr_l = _mm256_loadu_si256(
                                (__m256i *) &fr_pix[((y - y_tl) * (int) BACK_WIDTH + (x - x_tl)) * 4]);

                        __m256i bk_h = _mm256_cvtepu8_epi16(_mm256_extractf128_si256(bk_l, 1));
                        __m256i fr_h = _mm256_cvtepu8_epi16(_mm256_extractf128_si256(fr_l, 1));
                        bk_l = _mm256_cvtepu8_epi16(_mm256_extractf128_si256(bk_l, 0));
                        fr_l = _mm256_cvtepu8_epi16(_mm256_extractf128_si256(fr_l, 0));

                        // Extract alpha-channel values.
                        __m256i al_ch_l = _mm256_shuffle_epi8(fr_l, al_mask);
                        __m256i al_ch_h = _mm256_shuffle_epi8(fr_h, al_mask);

                        fr_l = _mm256_mullo_epi16(fr_l, al_ch_l);
                        bk_l = _mm256_mullo_epi16(bk_l, _mm256_sub_epi16(xff_256i, al_ch_l));
                        fr_h = _mm256_mullo_epi16(fr_h, al_ch_h);
                        bk_h = _mm256_mullo_epi16(bk_h, _mm256_sub_epi16(xff_256i, al_ch_h));

                        __m256i sum_l = _mm256_add_epi16(fr_l, bk_l);
                        __m256i sum_h = _mm256_add_epi16(fr_h, bk_h);

                        sum_l = _mm256_shuffle_epi8(sum_l, sum_mask);
                        sum_h = _mm256_shuffle_epi8(sum_h, sum_mask);

                        __m256i color = _mm256_setr_m128i(
                                _mm_add_epi64(_mm256_extracti128_si256(sum_l, 1), _mm256_extracti128_si256(sum_l, 0)),
                                _mm_add_epi64(_mm256_extracti128_si256(sum_h, 1), _mm256_extracti128_si256(sum_h, 0)));

                        _mm256_store_si256((__m256i *) sf_color, color);
                        for (int i = 0; i < 8; i++) {
                                image->setPixel(x + i, y, sf_color[i]);
                        }
                }
        }
}

