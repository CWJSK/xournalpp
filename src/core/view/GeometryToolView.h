/*
 * Xournal++
 *
 * A GeometryTool view
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <cmath>

#include <cairo.h>  // for cairo_t

#include "model/GeometryTool.h"  // for GeometryTool
#include "view/overlays/BaseStrokeToolView.h"

class Stroke;
class XojPageView;
class OverlayBase;

/**
 * @brief A class that renders a geometry tool
 */

namespace xoj::view {
class Repaintable;

constexpr double rad(double n) { return n * M_PI / 180.; }
constexpr double rad(int n) { return rad(static_cast<double>(n)); }
constexpr double deg(double a) { return a * 180.0 / M_PI; }
inline double cathete(double h, double o) { return std::sqrt(std::pow(h, 2) - std::pow(o, 2)); }

class GeometryToolView: public ToolView, public xoj::util::Listener<GeometryToolView> {

public:
    GeometryToolView(const GeometryTool* s, Repaintable* parent);
    virtual ~GeometryToolView();

    /**
     * Listener interface
     */
    static constexpr struct FlagDirtyRegionRequest {
    } FLAG_DIRTY_REGION = {};
    virtual void on(FlagDirtyRegionRequest, const Range& rg) = 0;

    static constexpr struct UpdateValuesRequest {
    } UPDATE_VALUES = {};
    virtual void on(UpdateValuesRequest, double h, double rot, cairo_matrix_t m) = 0;

    static constexpr struct FinalizationRequest {
    } FINALIZATION_REQUEST = {};
    /**
     * @brief Called before the corresponding GeometryTool's destruction
     * @param rg The bounding box of the entire geometry tool
     */
    virtual void deleteOn(FinalizationRequest, const Range& rg) = 0;

    /**
     * @brief draws the geometry tool and temporary stroke to a cairo context
     * @param cr the cairo context
     */
    void draw(cairo_t* cr) const override;

    bool isViewOf(const OverlayBase* overlay) const override;

private:
    /**
     * @brief draws the GeometryTool to a cairo context
     * @param cr the cairo context drawn to
     */
    virtual void drawGeometryTool(cairo_t* cr) const = 0;

    /**
     * @brief draws the temporary stroke to a cairo context
     * @param cr the cairo context drawn to
     */
    void drawTemporaryStroke(cairo_t* cr) const;

protected:
    /**
     * @brief the underlying GeometryTool
     */
    const GeometryTool* s;

    /**
     * @brief The stroke drawn aligned to the longest side of the GeometryTool or ending at the midpoint of the longest
     * side of the GeometryTool
     */
    Stroke* stroke = nullptr;

    /**
     * @brief renders text centered and possibly rotated at the current position on a cairo context
     * @param cr the cairo context
     * @param text the text string
     * @param angle the rotation angle
     */
    void showTextCenteredAndRotated(cairo_t* cr, std::string text, double angle) const;
};
};  // namespace xoj::view