// ImGuiTexInspect, a texture inspector widget for dear imgui

#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_tex_inspect.h"

namespace ImGuiTexInspect {
struct Context;
struct Transform2D;
//-------------------------------------------------------------------------
// [SECTION] INIT & SHUTDOWN
//-------------------------------------------------------------------------
void Init();
void Shutdown();

Context *CreateContext();
void DestroyContext(Context *);
void SetCurrentContext(Context *);

//-------------------------------------------------------------------------
// [SECTION] BASIC USAGE
//-------------------------------------------------------------------------

enum InspectorAlphaMode {
    InspectorAlphaMode_ImGui,       // Alpha is transparency so you see the ImGui panel background behind image
    InspectorAlphaMode_Black,       // Alpha is used to blend over a black background
    InspectorAlphaMode_White,       // Alpha is used to blend over a white background
    InspectorAlphaMode_CustomColor  // Alpha is used to blend over a custom colour.
};

typedef ImU64 InspectorFlags;
enum InspectorFlags_ {
    InspectorFlags_ShowWrap = 1 << 0,              // Draw beyong the [0,1] uv range. What you see will depend on API
    InspectorFlags_NoForceFilterNearest = 1 << 1,  // Normally we force nearest neighbour sampling when zoomed in. Set to disable this.
    InspectorFlags_NoGrid = 1 << 2,                // By default a grid is shown at high zoom levels
    InspectorFlags_NoTooltip = 1 << 3,             // Disable tooltip on hover
    InspectorFlags_FillHorizontal = 1 << 4,        // Scale to fill available space horizontally
    InspectorFlags_FillVertical = 1 << 5,          // Scale to fill available space vertically
    InspectorFlags_NoAutoReadTexture = 1 << 6,     // By default texture data is read to CPU every frame for tooltip and annotations
    InspectorFlags_FlipX = 1 << 7,                 // Horizontally flip the way the texture is displayed
    InspectorFlags_FlipY = 1 << 8,                 // Vertically flip the way the texture is displayed
};

/* Use one of these Size structs if you want to specify an exact size for the inspector panel.
 * E.g.
 * BeginInspectorPanel("MyPanel", texture_1K, ImVec2(1024,1024), 0, SizeExcludingBorder(ImVec2(1024,1024)));
 *
 * However, most of the time the default size will be fine. E.g.
 *
 * BeginInspectorPanel("MyPanel", texture_1K, ImVec2(1024,1024));
 */
struct SizeIncludingBorder {
    ImVec2 Size;
    SizeIncludingBorder(ImVec2 size) : Size(size) {}
};
struct SizeExcludingBorder {
    ImVec2 size;
    SizeExcludingBorder(ImVec2 size) : size(size) {}
};
/* BeginInspectorPanel
 * Returns true if panel is drawn.  Note that flags will only be considered on the first call */
bool BeginInspectorPanel(const char *name, ImTextureID, ImVec2 textureSize, InspectorFlags flags = 0);
bool BeginInspectorPanel(const char *name, ImTextureID, ImVec2 textureSize, InspectorFlags flags, SizeIncludingBorder size);
bool BeginInspectorPanel(const char *name, ImTextureID, ImVec2 textureSize, InspectorFlags flags, SizeExcludingBorder size);

/* EndInspectorPanel
 * Always call after BeginInspectorPanel and after you have drawn any required annotations*/
void EndInspectorPanel();

/* ReleaseInspectorData
 * ImGuiTexInspect keeps texture data cached in memory.  If you know you won't
 * be displaying a particular panel for a while you can call this to release
 * the memory. It won't be allocated again until next time you call
 * BeginInspectorPanel.  If id is NULL then the current (most recent) inspector
 * will be affected.  Unless you have a lot of different Inspector instances
 * you can probably not worry about this. Call CurrentInspector_GetID to get
 * the ID of an inspector.
 */
void ReleaseInspectorData(ImGuiID id);

/* CurrentInspector_InvalidateTextureCache
 * If using the InspectorFlags_NoAutoReadTexture flag then call this to
 * indicate your texture has changed context.
 */
void CurrentInspector_InvalidateTextureCache();

/* CurrentInspector_GetID
 * Get the ID of the current inspector.  Currently only used for calling
 * ReleaseInspectorData.
 */
ImGuiID CurrentInspector_GetID();

//-------------------------------------------------------------------------
// [SECTION] CONTEXT-WIDE SETTINGS
//-------------------------------------------------------------------------
/* SetZoomRate
 * factor should be greater than 1.  A value of 1.5 means one mouse wheel
 * scroll will increase zoom level by 50%. The factor used for zooming out is
 * 1/factor. */
void SetZoomRate(float factor);

//-------------------------------------------------------------------------
// [SECTION] INTERNAL
//-------------------------------------------------------------------------

struct Transform2D {
    ImVec2 Scale;
    ImVec2 Translate;

    /* Transform a vector by this transform.  Scale is applied first */
    ImVec2 operator*(const ImVec2 &rhs) const { return ImVec2(Scale.x * rhs.x + Translate.x, Scale.y * rhs.y + Translate.y); }

    /* Return an inverse transform such that transform.Inverse() * transform * vector == vector*/
    Transform2D Inverse() const {
        ImVec2 inverseScale(1 / Scale.x, 1 / Scale.y);
        return {inverseScale, ImVec2(-inverseScale.x * Translate.x, -inverseScale.y * Translate.y)};
    }
};

//-------------------------------------------------------------------------
// [SECTION] UTILITIES
//-------------------------------------------------------------------------
// Returns true if a flag is set
template <typename TSet, typename TFlag>
static inline bool HasFlag(TSet set, TFlag flag) {
    return (set & flag) == flag;
}

// Set flag or flags in set
template <typename TSet, typename TFlag>
static inline void SetFlag(TSet &set, TFlag flags) {
    set = static_cast<TSet>(set | flags);
}

// Clear flag or flags in set
template <typename TSet, typename TFlag>
static inline void ClearFlag(TSet &set, TFlag flag) {
    set = static_cast<TSet>(set & ~flag);
}

// Proper modulus operator, as opposed to remainder as calculated by %
template <typename T>
static inline T Modulus(T a, T b) {
    return a - b * ImFloorSigned(a / b);
}

// Defined in recent versions of imgui_internal.h.  Included here in case user is on older
// imgui version.
static inline float ImFloorSigned(float f) { return (float)((f >= 0 || (int)f == f) ? (int)f : (int)f - 1); }

static inline float Round(float f) { return ImFloorSigned(f + 0.5f); }

static inline ImVec2 Abs(ImVec2 v) { return ImVec2(ImAbs(v.x), ImAbs(v.y)); }

struct Inspector {
    ImGuiID ID;
    bool Initialized = false;

    // Texture
    ImTextureID Texture = ImTextureID{};
    ImVec2 TextureSize = {0, 0};  // Size in texels of texture
    float PixelAspectRatio = 1;   // Values other than 1 not supported yet

    // View State
    bool IsDragging = false;       // Is user currently dragging to pan view
    ImVec2 PanPos = {0.5f, 0.5f};  // The UV value at the center of the current view
    ImVec2 Scale = {1, 1};         // 1 pixel is 1 texel

    ImVec2 PanelTopLeftPixel = {0, 0};  // Top left of view in ImGui pixel coordinates
    ImVec2 PanelSize = {0, 0};          // Size of area allocated to drawing the image in pixels.

    ImVec2 ViewTopLeftPixel = {0, 0};  // Position in ImGui pixel coordinates
    ImVec2 ViewSize = {0, 0};          // Rendered size of current image. This could be smaller than panel size if user has zoomed out.
    ImVec2 ViewSizeUV = {0, 0};        // Visible region of the texture in UV coordinates

    /* Conversion transforms to go back and forth between screen pixels  (what ImGui considers screen pixels) and texels*/
    Transform2D TexelsToPixels;
    Transform2D PixelsToTexels;

    // Configuration
    InspectorFlags Flags = 0;

    // Background mode
    InspectorAlphaMode AlphaMode = InspectorAlphaMode_ImGui;
    ImVec4 CustomBackgroundColor = {0, 0, 0, 1};

    // Scaling limits
    ImVec2 ScaleMin = {0.02f, 0.02f};
    ImVec2 ScaleMax = {500, 500};

    // Grid
    float MinimumGridSize = 4;  // Don't draw the grid if lines would be closer than MinimumGridSize pixels

    // Annotations
    ImU32 MaxAnnotatedTexels = 0;

    ~Inspector();
};

//-------------------------------------------------------------------------
// [SECTION] INTERNAL FUNCTIONS
//-------------------------------------------------------------------------

Inspector *GetByKey(const Context *ctx, ImGuiID key);
Inspector *GetOrAddByKey(Context *ctx, ImGuiID key);

void SetPanPos(Inspector *inspector, ImVec2 pos);
void SetScale(Inspector *inspector, ImVec2 scale);
void SetScale(Inspector *inspector, float scaleY);
void RoundPanPos(Inspector *inspector);

ImU8 *GetBuffer(Inspector *inspector, size_t bytes);

/* GetTexelsToPixels
 * Calculate a transform to convert from texel coordinates to screen pixel coordinates
 * */
Transform2D GetTexelsToPixels(ImVec2 screenTopLeft, ImVec2 screenViewSize, ImVec2 uvTopLeft, ImVec2 uvViewSize, ImVec2 textureSize);

}  // namespace ImGuiTexInspect
