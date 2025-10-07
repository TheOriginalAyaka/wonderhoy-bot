//
// Created by Ayaka on 9/30/25.
//

#include "create_sticker.h"
#include "data/characters.h"

#include "SkCanvas.h"
#include "SkData.h"
#include "SkFontMgr.h"
#include "SkImage.h"
#include "SkPaint.h"
#include "SkPngEncoder.h"
#include "SkSurface.h"
#include "SkTextBlob.h"
#include "SkTypeface.h"
#include "SkFontMgr_empty.h"
#include "SkFontMetrics.h"

static void split_words(const std::string &text, std::vector<std::string> &out) {
    out.clear();
    out.reserve(16);
    const char *s = text.c_str();
    const char *p = s;
    while (*p) {
        while (*p && std::isspace(static_cast<unsigned char>(*p)))
            ++p;
        const char *start = p;
        while (*p && !std::isspace(static_cast<unsigned char>(*p)))
            ++p;
        if (p > start)
            out.emplace_back(start, static_cast<size_t>(p - start));
    }
}

static sk_sp<SkTypeface> get_typeface() {
    static sk_sp<SkTypeface> tf = [] {
        const sk_sp<SkFontMgr> fm = SkFontMgr_New_Custom_Empty();
        const auto font_path = "../assets/font.ttf";

        if (const auto fontData = SkData::MakeFromFileName(font_path)) {
            return fm->makeFromData(fontData);
        }
        return sk_sp<SkTypeface>(nullptr);
    }();

    return tf;
}

static bool parse_hex_color(const char *hex, SkColor *out) {
    if (!hex || hex[0] != '#')
        return false;

    char *end;
    const unsigned long value = std::strtoul(hex + 1, &end, 16);

    if (end - (hex + 1) != 6)
        return false;

    const unsigned int r = value >> 16 & 0xFF;
    const unsigned int g = value >> 8 & 0xFF;
    const unsigned int b = value & 0xFF;

    *out = SkColorSetRGB(r, g, b);
    return true;
}

std::string create_sticker(const char *text, int character_id, Options *options) {
    if (!text || !character_id)
        return {};

    const Character *character = get_character(character_id);

    const std::string img_path = "../assets/img/" + character->img;
    const char *img_path_cstr = img_path.c_str();

    auto img_data = SkData::MakeFromFileName(img_path_cstr);
    if (!img_data)
        return {};

    auto src = SkImages::DeferredFromEncodedData(img_data);
    if (!src)
        return {};

    constexpr int width = 296;
    constexpr int height = 256;
    auto info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType,
                                  kPremul_SkAlphaType);
    auto surface =
            SkSurfaces::Raster(info);
    if (!surface)
        return {};

    SkCanvas *canvas = surface->getCanvas();
    canvas->clear(SK_ColorTRANSPARENT);

    const auto src_size = src->dimensions();

    const float h_ratio =
            static_cast<float>(width) / static_cast<float>(src_size.width());
    const float v_ratio =
            static_cast<float>(height) / static_cast<float>(src_size.height());
    const float ratio = std::min(h_ratio, v_ratio);
    const float draw_w = static_cast<float>(src_size.width()) * ratio;
    const float draw_h = static_cast<float>(src_size.height()) * ratio;
    const float center_shift_x = (width - draw_w) * 0.5f;
    const float center_shift_y = (height - draw_h) * 0.5f;

    const auto dest = SkRect::MakeXYWH(center_shift_x, center_shift_y, draw_w, draw_h);
    SkSamplingOptions sampling(SkFilterMode::kLinear, SkMipmapMode::kLinear);
    canvas->drawImageRect(src, dest, sampling, nullptr);

    int font_size = options->font_size.value_or(45);
    int text_x = options->horizontal.value_or(width / 2);
    int text_y = options->vertical.value_or(55);
    const float rotation = options->rotation.value_or(-1.0f) / 10.0f;

    sk_sp<SkTypeface> tf = nullptr;
    tf = get_typeface();
    if (!tf)
        return {};

    SkFont font(tf, static_cast<SkScalar>(font_size));

    SkPaint paint_stroke, paint_fill;
    paint_stroke.setAntiAlias(true);
    paint_stroke.setStyle(SkPaint::kStroke_Style);
    paint_stroke.setStrokeWidth(9);
    paint_stroke.setColor(SK_ColorWHITE);

    paint_fill.setAntiAlias(true);
    paint_fill.setStyle(SkPaint::kFill_Style);
    SkColor fill_color = SK_ColorBLACK;
    (void) parse_hex_color(character->color.c_str(), &fill_color);
    paint_fill.setColor(fill_color);

    const std::string text_str(text);
    std::vector<std::string> lines;

    SkRect text_bounds{};
    font.measureText(text_str.c_str(), text_str.size(), SkTextEncoding::kUTF8, &text_bounds);
    const float text_width = text_bounds.width();

    SkFontMetrics metrics{};
    font.getMetrics(&metrics);
    int line_gap = static_cast<int>(std::ceil(metrics.fDescent - metrics.fAscent));

    if (text_width > width) {
        std::vector<std::string> words;
        split_words(text_str, words);

        if (words.empty()) {
            lines.push_back(text_str);
        } else {
            std::string current_line;
            for (const auto &word: words) {
                std::string test_line = current_line;
                if (!test_line.empty()) {
                    test_line += ' ';
                }
                test_line += word;

                SkRect test_bounds{};
                font.measureText(test_line.c_str(), test_line.size(), SkTextEncoding::kUTF8, &test_bounds);

                if (test_bounds.width() <= width) {
                    current_line = test_line;
                } else {
                    if (!current_line.empty()) {
                        lines.push_back(current_line);
                    }
                    current_line = word;
                }
            }

            if (!current_line.empty()) {
                lines.push_back(current_line);
            }
        }

        int widest_px = 0;
        for (const auto &line: lines) {
            SkRect _bounds{};
            font.measureText(line.c_str(), line.size(), SkTextEncoding::kUTF8, &_bounds);
            widest_px = std::max(widest_px, static_cast<int>(std::ceil(_bounds.width())));
        }

        if (widest_px > width) {
            font_size = (font_size * width) / widest_px;
            if (font_size < 8) font_size = 8;
            font.setSize(static_cast<SkScalar>(font_size));

            font.getMetrics(&metrics);
            line_gap = static_cast<int>(std::ceil(metrics.fDescent - metrics.fAscent));
        }
    } else {
        lines.push_back(text_str);
    }

    canvas->save();
    canvas->translate(static_cast<SkScalar>(text_x), static_cast<SkScalar>(text_y));
    canvas->rotate(rotation * 57.2958f);

    if (lines.size() > 1) {
        int k = 0;
        for (auto &line: lines) {
            SkRect bounds{};
            font.measureText(line.c_str(), line.size(), SkTextEncoding::kUTF8,
                             &bounds);
            float x_offset = -bounds.width() / 2;

            auto blob = SkTextBlob::MakeFromText(line.c_str(), line.size(), font,
                                                 SkTextEncoding::kUTF8);
            canvas->drawTextBlob(blob, x_offset, static_cast<SkScalar>(k),
                                 paint_stroke);
            canvas->drawTextBlob(blob, x_offset, static_cast<SkScalar>(k), paint_fill);
            k += line_gap;
        }
    } else {
        SkRect bounds{};
        font.measureText(text_str.c_str(), text_str.size(), SkTextEncoding::kUTF8,
                         &bounds);
        float x_offset = -bounds.width() / 2;

        auto blob = SkTextBlob::MakeFromText(text_str.c_str(), text_str.size(), font,
                                             SkTextEncoding::kUTF8);
        canvas->drawTextBlob(blob, x_offset, 0, paint_stroke);
        canvas->drawTextBlob(blob, x_offset, 0, paint_fill);
    }

    canvas->restore();

    auto image = surface->makeImageSnapshot();
    if (!image)
        return {};

    auto data =
            SkPngEncoder::Encode(nullptr, image.release(), SkPngEncoder::Options());
    if (!data)
        return {};

    return {
        static_cast<const char *>(data->data()),
        data->size()
    };
}
