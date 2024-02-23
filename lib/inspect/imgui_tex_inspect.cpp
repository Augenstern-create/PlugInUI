// ImGuiTexInspect, a texture inspector widget for dear imgui

//-------------------------------------------------------------------------
// [SECTION] INCLUDES
//-------------------------------------------------------------------------
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_tex_inspect.h"

#include "imgui.h"
#include "imgui_internal.h"
#include <iostream>
#include "game/Data.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)  // 'sprintf' considered unsafe
#endif

namespace ImGuiTexInspect {

void DrawRotatedTriangleI(ImDrawList *drawList, ImVec2 center, float side, float rotationAngle, ImU32 color) {
    // 计算三角形的高度
    float height = side * 0.6f * sqrt(3.0f);

    // 计算底部的中心点坐标
    ImVec2 baseCenter = ImVec2(center.x, center.y + height / 2);

    // 计算三角形的顶点坐标
    ImVec2 p0(center.x, center.y - height / 2);
    ImVec2 p1(center.x - side / 2, center.y + height / 2);
    ImVec2 p2(center.x + side / 2, center.y + height / 2);

    // 将角度转换为弧度值
    rotationAngle += 90.0f;
    float rotationAngleRad = rotationAngle * 3.14159265358979323846f / 180.0f;

    // 计算旋转后的三角形顶点坐标
    ImVec2 rotatedP0(center.x + (p0.x - baseCenter.x) * cos(rotationAngleRad) - (p0.y - baseCenter.y) * sin(rotationAngleRad),
                     center.y + (p0.x - baseCenter.x) * sin(rotationAngleRad) + (p0.y - baseCenter.y) * cos(rotationAngleRad));
    ImVec2 rotatedP1(center.x + (p1.x - baseCenter.x) * cos(rotationAngleRad) - (p1.y - baseCenter.y) * sin(rotationAngleRad),
                     center.y + (p1.x - baseCenter.x) * sin(rotationAngleRad) + (p1.y - baseCenter.y) * cos(rotationAngleRad));
    ImVec2 rotatedP2(center.x + (p2.x - baseCenter.x) * cos(rotationAngleRad) - (p2.y - baseCenter.y) * sin(rotationAngleRad),
                     center.y + (p2.x - baseCenter.x) * sin(rotationAngleRad) + (p2.y - baseCenter.y) * cos(rotationAngleRad));

    // 在给定的ImDrawList中绘制旋转后的三角形
    drawList->AddTriangleFilled(rotatedP0, rotatedP1, rotatedP2, color);
}
void DrawLineFromPointWithAngle(ImDrawList *drawList, ImVec2 startPoint, float length, float angle, ImU32 color) {
    float angleRad = angle * 3.14159265358979323846f / 180.0f;
    ImVec2 endPoint(startPoint.x + length * cos(angleRad), startPoint.y + length * sin(angleRad));
    drawList->AddLine(startPoint, endPoint, color);
}

void DrawingLocation(ImDrawList *drawList, float size, float direction, ImVec2 location, ImU32 color) {
    DrawRotatedTriangleI(drawList, location, size * 2.0f, direction, color);
    drawList->AddCircleFilled(location, size, color);
}

float FiltrationValue(float value, float min, float max) { return value > max ? max : (value < min ? min : value); }
void DrawingPlayer(PlayerInfo player, ImVec2 location, float zoom) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    float size = gameData.readConfig.location_size * zoom;
    if (gameData.readConfig.is_drawing_names && player.Entity != gameData.Myself.PlayerPtr) {
        ImVec2 txtSize = ImGui::CalcTextSize(player.Name.c_str());
        ImVec2 Pos = ImVec2(location.x - txtSize.x * 0.5f, location.y + size * 2.0f);
        window->DrawList->AddRectFilled(ImVec2{Pos.x - 2.0f, Pos.y - 2.0f}, ImVec2(Pos.x + txtSize.x + 2.0f, Pos.y + txtSize.y + 2.0f), IM_COL32(0, 0, 0, 128));
        ImGui::RenderText(ImVec2{Pos.x, Pos.y}, player.Name.c_str());
    }
    if (gameData.readConfig.is_drawing_hp && player.TeamID != gameData.Myself.TeamID) {
        float HP = FiltrationValue(player.Health, 0.0f, 100.0f);
        ImVec2 HPPos1 = {location.x - 20.0f, location.y + 15.0f};
        ImVec2 HPPos2 = {location.x + 20.0f, location.y + 20.0f};
        window->DrawList->AddRectFilled(HPPos1, HPPos2, gameData.readConfig.hp_color);
    }
    if (gameData.readConfig.is_drawing_location) {
        ImColor color = player.TeamID == gameData.Myself.TeamID ? gameData.readConfig.team_location_color : gameData.readConfig.foes_location_color;
        color = player.Entity == gameData.Myself.PlayerPtr ? gameData.readConfig.my_location_color : color;
        DrawRotatedTriangleI(window->DrawList, location, size * 2.0f, player.AimOffsets.y, color);
        window->DrawList->AddCircleFilled(location, size, color);
    }

    if (gameData.readConfig.is_drawing_teamID) {
        std::string txt = std::to_string(player.TeamID);
        ImVec2 txtSize = ImGui::CalcTextSize(txt.c_str());
        ImGui::RenderText(ImVec2(location.x - txtSize.x * 0.5f, location.y - txtSize.y * 0.5f), txt.c_str());
    }
    if (player.Entity == gameData.Myself.PlayerPtr) {
        if (gameData.readConfig.is_drawing_sight) {
            DrawLineFromPointWithAngle(window->DrawList, location, 1000.0f, player.AimOffsets.y, gameData.readConfig.my_location_color);
        }
    }
}

// void DrawingPlayer(const char *label, float size, float direction, ImVec2 location, ImU32 color) {
//     ImGuiWindow *window = ImGui::GetCurrentWindow();
//     if (window->SkipItems) return;

//     if(gameData.readConfig.is_drawing_names){

//     }
//     ImVec2 txtSize = ImGui::CalcTextSize(label);
//     ImVec2 Pos = ImVec2(location.x - txtSize.x * 0.5f, location.y + size * 2.0f);
//     float HP = 100.0f;
//     if (window->SkipItems) return;
//     HP = HP > 100.0f ? 100.0f : HP;
//     HP = HP < 0.0f ? 0.0f : HP;
//     ImVec2 HPPos1 = {location.x - 20.0f, location.y + 15.0f};
//     ImVec2 HPPos2 = {location.x + 20.0f, location.y + 20.0f};
//     window->DrawList->AddRectFilled(HPPos1, HPPos2, IM_COL32(0, 255, 0, 255));
//     DrawRotatedTriangleI(window->DrawList, location, size * 2.0f, direction, color);
//     window->DrawList->AddCircleFilled(location, size, color);
//     ImGui::RenderText(ImVec2(location.x - size * 0.25f, location.y - size * 0.5f), "9");

//     window->DrawList->AddRectFilled(ImVec2{Pos.x - 2.0f, Pos.y - 2.0f}, ImVec2(Pos.x + txtSize.x + 2.0f, Pos.y + txtSize.y + 2.0f), IM_COL32(0, 0, 0, 128));

//     ImGui::RenderText(ImVec2{Pos.x, Pos.y}, label);
// }

//-------------------------------------------------------------------------
// [SECTION] GLOBAL STATE
//-------------------------------------------------------------------------

// Input mapping structure, default values listed in the comments.
struct InputMap {
    ImGuiMouseButton PanButton;  // LMB      enables panning when held
    InputMap();
};

InputMap::InputMap() { PanButton = ImGuiMouseButton_Left; }

// Settings configured via SetNextPanelOptions etc.
struct NextPanelSettings {
    InspectorFlags ToSet = 0;
    InspectorFlags ToClear = 0;
};

// Main context / configuration structure for imgui_tex_inspect
struct Context {
    InputMap Input;                        // Input mapping config
    ImGuiStorage Inspectors;               // All the inspectors we've seen
    Inspector *CurrentInspector;           // Inspector currently being processed
    NextPanelSettings NextPanelOptions;    // Options configured for next inspector panel
    float ZoomRate = 1.3f;                 // How fast mouse wheel affects zoom
    float DefaultPanelHeight = 600;        // Height of panel in pixels
    float DefaultInitialPanelWidth = 600;  // Only applies when window first appears
    int MaxAnnotations = 1000;             // Limit number of texel annotations for performance
};

Context *GContext = nullptr;

//-------------------------------------------------------------------------
// [SECTION] USER FUNCTIONS
//-------------------------------------------------------------------------

void Init() {
    // Nothing to do here.  But there might be in a later version. So client code should still call it!
}

void Shutdown() {
    // Nothing to do here.  But there might be in a later version. So client code should still call it!
}

Context *CreateContext() {
    GContext = IM_NEW(Context);
    SetCurrentContext(GContext);
    return GContext;
}

void DestroyContext(Context *ctx) {
    if (ctx == NULL) {
        ctx = GContext;
    }

    if (ctx == GContext) {
        GContext = NULL;
    }

    for (ImGuiStorage::ImGuiStoragePair &pair : ctx->Inspectors.Data) {
        Inspector *inspector = (Inspector *)pair.val_p;
        if (inspector) {
            IM_DELETE(inspector);
        }
    }

    IM_DELETE(ctx);
}

void SetCurrentContext(Context *context) { ImGuiTexInspect::GContext = context; }

void SetNextPanelFlags(InspectorFlags setFlags, InspectorFlags clearFlags) {
    SetFlag(GContext->NextPanelOptions.ToSet, setFlags);
    SetFlag(GContext->NextPanelOptions.ToClear, clearFlags);
}

bool BeginInspectorPanel(const char *title, ImTextureID texture, ImVec2 textureSize, InspectorFlags flags, SizeIncludingBorder sizeIncludingBorder) {
    const int borderWidth = 1;
    // Unpack size param.  It's in the SizeIncludingBorder structure just to make sure users know what they're requesting
    ImVec2 size = sizeIncludingBorder.Size;

    ImGuiWindow *window = ImGui::GetCurrentWindow();

    Context *ctx = GContext;

    const ImGuiID ID = window->GetID(title);
    const ImGuiIO &IO = ImGui::GetIO();

    // Create or find inspector
    bool justCreated = GetByKey(ctx, ID) == NULL;
    ctx->CurrentInspector = GetOrAddByKey(ctx, ID);
    Inspector *inspector = ctx->CurrentInspector;
    justCreated |= !inspector->Initialized;

    // Cache the basics
    inspector->ID = ID;
    inspector->Texture = texture;
    inspector->TextureSize = textureSize;
    inspector->Initialized = true;

    // Handle incoming flags. We keep special track of the
    // newly set flags because somethings only take effect
    // the first time the flag is set.
    InspectorFlags newlySetFlags = ctx->NextPanelOptions.ToSet;
    if (justCreated) {
        SetFlag(newlySetFlags, flags);
        inspector->MaxAnnotatedTexels = ctx->MaxAnnotations;
    }
    SetFlag(inspector->Flags, newlySetFlags);
    ClearFlag(inspector->Flags, ctx->NextPanelOptions.ToClear);
    ClearFlag(newlySetFlags, ctx->NextPanelOptions.ToClear);
    ctx->NextPanelOptions = NextPanelSettings();

    // Calculate panel size
    ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

    ImVec2 panelSize;
    if (justCreated) {
        panelSize = {size.x == 0 ? ImMax(ctx->DefaultInitialPanelWidth, contentRegionAvail.x) : size.x,
                     size.y == 0 ? ImMax(ctx->DefaultPanelHeight, contentRegionAvail.y) : size.y};
    } else {
        panelSize = {size.x == 0 ? contentRegionAvail.x : size.x, size.y == 0 ? contentRegionAvail.y : size.y};
    }

    inspector->PanelSize = panelSize;
    ImVec2 availablePanelSize = panelSize - ImVec2(borderWidth, borderWidth) * 2;

    {
        // Possibly update scale
        float newScale = -1;

        if (HasFlag(newlySetFlags, InspectorFlags_FillVertical)) {
            newScale = availablePanelSize.y / textureSize.y;
        } else if (HasFlag(newlySetFlags, InspectorFlags_FillHorizontal)) {
            newScale = availablePanelSize.x / textureSize.x;
        } else if (justCreated) {
            newScale = 1;
        }

        if (newScale != -1) {
            inspector->Scale = ImVec2(newScale, newScale);
            SetPanPos(inspector, ImVec2(0.5f, 0.5f));
        }
    }

    RoundPanPos(inspector);

    ImVec2 textureSizePixels = inspector->Scale * textureSize;   // Size whole texture would appear on screen
    ImVec2 viewSizeUV = availablePanelSize / textureSizePixels;  // Cropped size in terms of UV
    ImVec2 uv0 = inspector->PanPos - viewSizeUV * 0.5;
    ImVec2 uv1 = inspector->PanPos + viewSizeUV * 0.5;

    ImVec2 drawImageOffset{borderWidth, borderWidth};
    ImVec2 viewSize = availablePanelSize;

    if ((inspector->Flags & InspectorFlags_ShowWrap) == 0) {
        /* Don't crop the texture to UV [0,1] range.  What you see outside this
         * range will depend on API and texture properties */
        if (textureSizePixels.x < availablePanelSize.x) {
            // Not big enough to horizontally fill view
            viewSize.x = ImFloor(textureSizePixels.x);
            drawImageOffset.x += ImFloor((availablePanelSize.x - textureSizePixels.x) / 2);
            uv0.x = 0;
            uv1.x = 1;
            viewSizeUV.x = 1;
            inspector->PanPos.x = 0.5f;
        }
        if (textureSizePixels.y < availablePanelSize.y) {
            // Not big enough to vertically fill view
            viewSize.y = ImFloor(textureSizePixels.y);
            drawImageOffset.y += ImFloor((availablePanelSize.y - textureSizePixels.y) / 2);
            uv0.y = 0;
            uv1.y = 1;
            viewSizeUV.y = 1;
            inspector->PanPos.y = 0.5;
        }
    }

    inspector->ViewSize = viewSize;
    inspector->ViewSizeUV = viewSizeUV;

    /* We use mouse scroll to zoom so we don't want scroll to propagate to
     * parent window. For this to happen we must NOT set
     * ImGuiWindowFlags_NoScrollWithMouse.  This seems strange but it's the way
     * ImGui works.  Also we must ensure the ScrollMax.y is not zero for the
     * child window. */
    if (ImGui::BeginChild(title, panelSize, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove)) {
        // See comment above
        ImGui::GetCurrentWindow()->ScrollMax.y = 1.0f;

        // Keep track of size of area that we draw for borders later
        inspector->PanelTopLeftPixel = ImGui::GetCursorScreenPos();
        ImGui::SetCursorPos(ImGui::GetCursorPos() + drawImageOffset);
        inspector->ViewTopLeftPixel = ImGui::GetCursorScreenPos();

        ImGui::Image(texture, viewSize, uv0, uv1);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
        ImVec2 buttonPos(panelSize.x - 18.0f - 100.0f, 18.0f);
        ImGui::SetCursorPos(buttonPos);
        if (ImGui::Button("Locked players", ImVec2(100, 50))) {
            gameData.readConfig.is_drawing_lock_my = !gameData.readConfig.is_drawing_lock_my;
        }
        ImGui::PopStyleColor();
        float imageWidth = uv1.x - uv0.x;
        float imageHeight = uv1.y - uv0.y;
        float zoomFactorX = (viewSize.x / imageWidth) / viewSize.x;
        float zoomFactory = (viewSize.y / imageHeight) / viewSize.y;
        auto Playerlist = Data::GetPlayers();
        auto PlayerPos = gameData.mapRadar.rader_players;
        for (auto player : Playerlist) {
            auto it = PlayerPos.find(player.Entity);
            if (it != PlayerPos.end()) {
                auto pos = it->second;
                ImVec2 zoomedPoint = ImVec2((pos.x - uv0.x) * viewSize.x / imageWidth, (pos.y - uv0.y) * viewSize.y / imageHeight);
                ImVec2 screenPoint = ImVec2((18.0f) + zoomedPoint.x + drawImageOffset.x, (18.0f) + zoomedPoint.y + drawImageOffset.y);
                DrawingPlayer(player, ImVec2(screenPoint.x, screenPoint.y), 1.0f + uv0.y);
            } else {
                continue;
            }
        }
        // PlayerInfo player1;
        // player1.Name = "ces1";
        // player1.AimOffsets = {0.0f, 0.0f, 0.0f};
        // player1.TeamID = 1;
        // player1.Entity = 0;
        // ImVec2 screenPoints = ImVec2((18.0f) + 0.0f + drawImageOffset.x, (18.0f) + 0.0f + drawImageOffset.y);
        // DrawingPlayer(player1, screenPoints, 1.0f + uv0.y);

        ImGui::GetWindowDrawList()->AddCallback(ImDrawCallback_ResetRenderState, nullptr);

        /* Matrices for going back and forth between texel coordinates in the
         * texture and screen coordinates based on where texture is drawn.
         * Useful for annotations and mouse hover etc. */
        inspector->TexelsToPixels = GetTexelsToPixels(inspector->ViewTopLeftPixel, viewSize, uv0, viewSizeUV, inspector->TextureSize);
        inspector->PixelsToTexels = inspector->TexelsToPixels.Inverse();

        ImVec2 mousePos = ImGui::GetMousePos();
        if (gameData.readConfig.is_drawing_lock_my) {
            ImVec2 pos = gameData.Myself.AtlaseLocation;
            ImVec2 zoomedPoint = ImVec2((pos.x - uv0.x) * viewSize.x / imageWidth, (pos.y - uv0.y) * viewSize.y / imageHeight);
            ImVec2 screenPoint = ImVec2((18.0f) + zoomedPoint.x + drawImageOffset.x, (18.0f) + zoomedPoint.y + drawImageOffset.y);
            mousePos = screenPoint;
        }
        ImVec2 mousePosTexel = inspector->PixelsToTexels * mousePos;
        ImVec2 mouseUV = mousePosTexel / textureSize;
        mousePosTexel.x = Modulus(mousePosTexel.x, textureSize.x);
        mousePosTexel.y = Modulus(mousePosTexel.y, textureSize.y);

        bool hovered = ImGui::IsWindowHovered();

        {  // DRAGGING
            if (!gameData.readConfig.is_drawing_lock_my) {
                // start drag
                if (!inspector->IsDragging && hovered && IO.MouseClicked[ctx->Input.PanButton]) {
                    inspector->IsDragging = true;
                }
                // carry on dragging
                else if (inspector->IsDragging) {
                    ImVec2 uvDelta = IO.MouseDelta * viewSizeUV / viewSize;
                    inspector->PanPos -= uvDelta;
                    RoundPanPos(inspector);
                }

                // end drag
                if (inspector->IsDragging && (IO.MouseReleased[ctx->Input.PanButton] || !IO.MouseDown[ctx->Input.PanButton])) {
                    inspector->IsDragging = false;
                }
            } else {
                ImVec2 centerPos = (uv1 - uv0) * 0.5 + uv0;
                ImVec2 offset = ((gameData.Myself.AtlaseLocation - centerPos) * viewSize) * -1.0f;
                ImVec2 uvDelta = offset * viewSizeUV / viewSize;
                inspector->PanPos -= uvDelta;
                RoundPanPos(inspector);
            }
        }

        // ZOOM
        if (hovered && IO.MouseWheel != 0) {
            float zoomRate = ctx->ZoomRate;
            float scale = inspector->Scale.y;
            float prevScale = scale;

            bool keepTexelSizeRegular = scale > inspector->MinimumGridSize && !HasFlag(inspector->Flags, InspectorFlags_NoGrid);
            if (IO.MouseWheel > 0) {
                scale *= zoomRate;
                if (keepTexelSizeRegular) {
                    // It looks nicer when all the grid cells are the same size
                    // so keep scale integer when zoomed in
                    scale = ImCeil(scale);
                }
            } else {
                scale /= zoomRate;
                if (keepTexelSizeRegular) {
                    // See comment above. We're doing a floor this time to make
                    // sure the scale always changes when scrolling
                    scale = ImFloorSigned(scale);
                }
            }
            /* To make it easy to get back to 1:1 size we ensure that we stop
             * here without going straight past it*/
            if ((prevScale < 1 && scale > 1) || (prevScale > 1 && scale < 1)) {
                scale = 1;
            }
            SetScale(inspector, ImVec2(inspector->PixelAspectRatio * scale, scale));
            SetPanPos(inspector, inspector->PanPos + (mouseUV - inspector->PanPos) * (1 - prevScale / scale));
        }

        return true;
    } else {
        return false;
    }
}

bool BeginInspectorPanel(const char *name, ImTextureID texture, ImVec2 textureSize, InspectorFlags flags) {
    return BeginInspectorPanel(name, texture, textureSize, flags, SizeIncludingBorder{{0, 0}});
}

void EndInspectorPanel() {
    const ImU32 innerBorderColour = 0xFFFFFFFF;
    const ImU32 outerBorderColour = 0xFF888888;
    Inspector *inspector = GContext->CurrentInspector;

    // Draw out border around whole inspector panel
    ImGui::GetWindowDrawList()->AddRect(inspector->PanelTopLeftPixel, inspector->PanelTopLeftPixel + inspector->PanelSize, outerBorderColour);

    // Draw innder border around texture.  If zoomed in this will completely cover the outer border
    ImGui::GetWindowDrawList()->AddRect(inspector->ViewTopLeftPixel - ImVec2(1, 1), inspector->ViewTopLeftPixel + inspector->ViewSize + ImVec2(1, 1),
                                        innerBorderColour);

    ImGui::EndChild();
}

void ReleaseInspectorData(ImGuiID ID) {
    Inspector *inspector = GetByKey(GContext, ID);

    if (inspector == NULL) return;
    /* In a later version we will remove inspector from the inspector table
     * altogether. For now we reset the whole inspector structure to prevent
     * clients relying on persisted data.
     */
    *inspector = Inspector();
}

ImGuiID CurrentInspector_GetID() { return GContext->CurrentInspector->ID; }

void SetZoomRate(float rate) { GContext->ZoomRate = rate; }

//-------------------------------------------------------------------------
// [SECTION] Life Cycle
//-------------------------------------------------------------------------

Inspector::~Inspector() {}
//-------------------------------------------------------------------------
// [SECTION] Scaling and Panning
//-------------------------------------------------------------------------
void RoundPanPos(Inspector *inspector) {
    if ((inspector->Flags & InspectorFlags_ShowWrap) > 0) {
        /* PanPos is the point in the center of the current view. Allow the
         * user to pan anywhere as long as the view center is inside the
         * texture.*/
        inspector->PanPos = ImClamp(inspector->PanPos, ImVec2(0, 0), ImVec2(1, 1));
    } else {
        /* When ShowWrap mode is disabled the limits are a bit more strict. We
         * try to keep it so that the user cannot pan past the edge of the
         * texture at all.*/
        ImVec2 absViewSizeUV = Abs(inspector->ViewSizeUV);
        inspector->PanPos = ImMax(inspector->PanPos - absViewSizeUV / 2, ImVec2(0, 0)) + absViewSizeUV / 2;
        inspector->PanPos = ImMin(inspector->PanPos + absViewSizeUV / 2, ImVec2(1, 1)) - absViewSizeUV / 2;
    }

    /* If inspector->scale is 1 then we should ensure that pixels are aligned
     * with texel centers to get pixel-perfect texture rendering*/
    ImVec2 topLeftSubTexel = inspector->PanPos * inspector->Scale * inspector->TextureSize - inspector->ViewSize * 0.5f;

    if (inspector->Scale.x >= 1) {
        topLeftSubTexel.x = Round(topLeftSubTexel.x);
    }
    if (inspector->Scale.y >= 1) {
        topLeftSubTexel.y = Round(topLeftSubTexel.y);
    }
    inspector->PanPos = (topLeftSubTexel + inspector->ViewSize * 0.5f) / (inspector->Scale * inspector->TextureSize);
}

void SetPanPos(Inspector *inspector, ImVec2 pos) {
    inspector->PanPos = pos;
    RoundPanPos(inspector);
}

void SetScale(Inspector *inspector, ImVec2 scale) {
    scale = ImClamp(scale, inspector->ScaleMin, inspector->ScaleMax);

    inspector->ViewSizeUV *= inspector->Scale / scale;

    inspector->Scale = scale;
}

void SetScale(Inspector *inspector, float scaleY) { SetScale(inspector, ImVec2(scaleY * inspector->PixelAspectRatio, scaleY)); }
//-------------------------------------------------------------------------
// [SECTION] INSPECTOR MAP
//-------------------------------------------------------------------------

Inspector *GetByKey(const Context *ctx, ImGuiID key) { return (Inspector *)ctx->Inspectors.GetVoidPtr(key); }

Inspector *GetOrAddByKey(Context *ctx, ImGuiID key) {
    Inspector *inspector = GetByKey(ctx, key);
    if (inspector) {
        return inspector;
    } else {
        inspector = IM_NEW(Inspector);
        ctx->Inspectors.SetVoidPtr(key, inspector);
        return inspector;
    }
}

const ImGuiCol disabledUIColorIds[] = {ImGuiCol_FrameBg, ImGuiCol_FrameBgActive, ImGuiCol_FrameBgHovered, ImGuiCol_Text, ImGuiCol_CheckMark};

// Calculate a transform to convert from texel coordinates to screen pixel coordinates
Transform2D GetTexelsToPixels(ImVec2 screenTopLeft, ImVec2 screenViewSize, ImVec2 uvTopLeft, ImVec2 uvViewSize, ImVec2 textureSize) {
    ImVec2 uvToPixel = screenViewSize / uvViewSize;

    Transform2D transform;
    transform.Scale = uvToPixel / textureSize;
    transform.Translate.x = screenTopLeft.x - uvTopLeft.x * uvToPixel.x;
    transform.Translate.y = screenTopLeft.y - uvTopLeft.y * uvToPixel.y;
    return transform;
}

}  // namespace ImGuiTexInspect
