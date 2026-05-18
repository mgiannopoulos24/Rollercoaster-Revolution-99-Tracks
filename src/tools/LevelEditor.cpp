#include "LevelEditor.h"

#include "physics/CoasterPhysics.h"
#include "physics/TrackSegment.h"
#include "physics/Train.h"
#include "render/Renderer.h"
#include "track/TrackBuilder.h"
#include "track/TrackElements.h"
#include "track/TrackLayout.h"
#include "track/TrackValidator.h"

#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>

namespace rr99 {

LevelEditor::LevelEditor()
    : m_layout(std::make_unique<TrackLayout>())
    , m_elements(std::make_unique<TrackElements>()) {
    newTrack(10);
}

bool LevelEditor::initialize() {
    m_statusMessage = "Level Editor ready. [1-8] select segment type, [S]ave, [L]oad, [P]lay test";
    m_statusTimer = 5.0f;
    return true;
}

void LevelEditor::tick(float dt) {
    if (m_statusTimer > 0.0f) {
        m_statusTimer -= dt;
        if (m_statusTimer <= 0.0f) {
            m_statusMessage.clear();
        }
    }

    if (m_playTesting) {
        m_playTestSpeed -= CoasterPhysics::GRAVITY * 0.016f * 0.5f;
        if (m_playTestSpeed < 0.0f) {
            m_playTestSpeed = 0.0f;
            stopPlayTest();
            m_statusMessage = "Play test: train stopped";
            m_statusTimer = 2.0f;
        }
    }
}

void LevelEditor::draw(Renderer& renderer) {
    int screenW = renderer.getWidth();
    int screenH = renderer.getHeight();

    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 30, 30, 35, 255);
    SDL_RenderClear(renderer.getSDLRenderer());

    int toolbarH = 40;
    int palW = 140;
    int propsW = 200;

    drawToolbar(renderer, 0, 0, screenW, toolbarH);

    int canvasY = toolbarH;
    int canvasH = screenH - canvasY;

    drawPalette(renderer, 0, canvasY, palW, canvasH);
    drawCanvas(renderer, palW, canvasY, screenW - palW - propsW, canvasH);
    drawProperties(renderer, screenW - propsW, canvasY, propsW, canvasH);

    if (!m_statusMessage.empty()) {
        SDL_Color white = { 255, 255, 255, 255 };
        renderer.drawText(10, screenH - 25, m_statusMessage, white, 18);
    }
}

void LevelEditor::drawToolbar(Renderer& renderer, int x, int y, int width, int height) {
    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 50, 50, 55, 255);
    SDL_Rect bar = { x, y, width, height };
    SDL_RenderFillRect(renderer.getSDLRenderer(), &bar);

    const char* tools[] = { "Select", "Place", "Token", "Power", "Hazard", "Delete" };
    EditorTool toolValues[] = { EditorTool::Select,       EditorTool::PlaceSegment, EditorTool::PlaceToken,
                                EditorTool::PlacePowerUp, EditorTool::PlaceHazard,  EditorTool::Delete };
    int numTools = sizeof(tools) / sizeof(tools[0]);

    int btnW = 70;
    int btnH = height - 8;
    int btnY = y + 4;

    for (int i = 0; i < numTools; ++i) {
        int btnX = x + 8 + i * (btnW + 4);
        bool isActive = m_currentTool == toolValues[i];

        if (isActive) {
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 80, 140, 200, 255);
        } else {
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 70, 70, 80, 255);
        }
        SDL_Rect btn = { btnX, btnY, btnW, btnH };
        SDL_RenderFillRect(renderer.getSDLRenderer(), &btn);

        SDL_Color textColor = { 220, 220, 220, 255 };
        renderer.drawText(btnX + 5, btnY + btnH / 2 - 8, tools[i], textColor, 14);
    }

    const char* trains[] = { "Light", "Medium", "Heavy" };
    int trainBtnX = x + width - 220;
    for (int i = 0; i < 3; ++i) {
        int bx = trainBtnX + i * 70;
        bool isActive = m_trainPreview == i;
        if (isActive) {
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 60, 180, 75, 255);
        } else {
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 70, 70, 80, 255);
        }
        SDL_Rect btn = { bx, btnY, 66, btnH };
        SDL_RenderFillRect(renderer.getSDLRenderer(), &btn);
        SDL_Color tc = { 220, 220, 220, 255 };
        renderer.drawText(bx + 5, btnY + btnH / 2 - 8, trains[i], tc, 13);
    }
}

void LevelEditor::drawPalette(Renderer& renderer, int x, int y, int width, int height) {
    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 40, 40, 45, 255);
    SDL_Rect bg = { x, y, width, height };
    SDL_RenderFillRect(renderer.getSDLRenderer(), &bg);

    SDL_Color titleColor = { 200, 200, 200, 255 };
    renderer.drawText(x + 10, y + 10, "Segments", titleColor, 16);

    int numTypes = static_cast<int>(SegmentType::COUNT);
    int slotH = 50;
    int startY = y + 35;

    for (int i = 0; i < numTypes; ++i) {
        int sy = startY + i * (slotH + 4);
        bool isActive = m_brushType == i;
        bool isHover = false;

        if (isActive) {
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 80, 140, 200, 255);
        } else if (isHover) {
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 60, 60, 70, 255);
        } else {
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 50, 50, 55, 255);
        }
        SDL_Rect slot = { x + 6, sy, width - 12, slotH };
        SDL_RenderFillRect(renderer.getSDLRenderer(), &slot);

        drawSegmentPreview(renderer, x + 10, sy + 5, i, true);

        SDL_Color tc = { 180, 180, 180, 255 };
        renderer.drawText(x + 50, sy + slotH / 2 - 8, TrackSegment::getTypeName(static_cast<SegmentType>(i)), tc, 12);
    }
}

void LevelEditor::drawCanvas(Renderer& renderer, int x, int y, int width, int height) {
    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 25, 25, 30, 255);
    SDL_Rect bg = { x, y, width, height };
    SDL_RenderFillRect(renderer.getSDLRenderer(), &bg);

    drawGrid(renderer, x, y, width, height);

    int centerX = x + width / 2;
    int trackY = y + height / 2;

    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 100, 180, 255, 255);

    const int segSpacing = m_canvasZoom;
    int segCount = static_cast<int>(m_editorSegments.size());

    for (int i = 0; i < segCount; ++i) {
        auto& seg = m_editorSegments[i];
        int sx = centerX + (i - segCount / 2) * segSpacing;
        int sy = trackY + static_cast<int>(seg.heightDelta * 1.5f);

        bool isSelected = (i == m_selectedSegment);
        bool valid = true;

        SDL_Color color;
        if (isSelected) {
            color = { 255, 200, 50, 255 };
        } else if (!valid) {
            color = { 200, 80, 80, 255 };
        } else {
            auto segType = static_cast<SegmentType>(seg.typeIndex);
            switch (segType) {
                case SegmentType::Straight:
                    color = { 100, 180, 255, 255 };
                    break;
                case SegmentType::Curve:
                    color = { 100, 255, 150, 255 };
                    break;
                case SegmentType::Drop:
                    color = { 255, 150, 100, 255 };
                    break;
                case SegmentType::Jump:
                    color = { 255, 200, 100, 255 };
                    break;
                case SegmentType::Loop:
                    color = { 255, 100, 200, 255 };
                    break;
                case SegmentType::Tunnel:
                    color = { 150, 100, 150, 255 };
                    break;
                case SegmentType::WaterSplash:
                    color = { 100, 150, 255, 255 };
                    break;
                case SegmentType::FallingTrack:
                    color = { 200, 80, 80, 255 };
                    break;
                default:
                    color = { 150, 150, 150, 255 };
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer.getSDLRenderer(), color.r, color.g, color.b, 255);

        int segWidth = segSpacing - 4;
        int segHeight = 20;
        SDL_Rect segRect = { sx - segWidth / 2, sy - segHeight / 2, segWidth, segHeight };
        SDL_RenderFillRect(renderer.getSDLRenderer(), &segRect);

        if (i > 0) {
            auto& prevSeg = m_editorSegments[i - 1];
            int px = centerX + (i - 1 - segCount / 2) * segSpacing;
            int py = trackY + static_cast<int>(prevSeg.heightDelta * 1.5f);
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 200, 200, 200, 100);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), px + segSpacing / 2, py, sx - segSpacing / 2, sy);
        }

        for (auto& elem : m_editorElements) {
            if (elem.segmentIndex == i) {
                drawElementIcons(renderer, sx, sy - 20, elem);
            }
        }

        if (segCount > 0 && i == segCount - 1) {
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 255, 100, 100, 200);
            int fx = sx + segSpacing / 2;
            SDL_Rect finishRect = { fx - 3, sy - 12, 6, 24 };
            SDL_RenderFillRect(renderer.getSDLRenderer(), &finishRect);
        }
    }

    int totalWidth = segCount * segSpacing;
    int startX = centerX - totalWidth / 2 - segSpacing / 2;
    drawConnectionDots(renderer, startX, trackY, startX + totalWidth + segSpacing, trackY, true);
}

void LevelEditor::drawProperties(Renderer& renderer, int x, int y, int width, int height) {
    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 40, 40, 45, 255);
    SDL_Rect bg = { x, y, width, height };
    SDL_RenderFillRect(renderer.getSDLRenderer(), &bg);

    SDL_Color titleColor = { 200, 200, 200, 255 };
    renderer.drawText(x + 10, y + 10, "Properties", titleColor, 16);

    if (m_selectedSegment >= 0 && m_selectedSegment < static_cast<int>(m_editorSegments.size())) {
        auto& seg = m_editorSegments[m_selectedSegment];
        auto segType = static_cast<SegmentType>(seg.typeIndex);
        auto defProps = TrackSegment::getDefaultProperties(segType);

        int py = y + 40;
        int lineH = 22;

        SDL_Color labelColor = { 160, 160, 160, 255 };
        SDL_Color valueColor = { 220, 220, 220, 255 };

        renderer.drawText(x + 10, py, "Type:", labelColor, 13);
        py += lineH;
        renderer.drawText(x + 60, py - lineH, TrackSegment::getTypeName(segType), valueColor, 13);

        char buf[64];
        snprintf(buf, sizeof(buf), "Length: %.1f", defProps.length);
        renderer.drawText(x + 10, py, buf, labelColor, 13);
        py += lineH;

        snprintf(buf, sizeof(buf), "Height: %.1f", seg.heightDelta);
        renderer.drawText(x + 10, py, buf, labelColor, 13);
        py += lineH;

        snprintf(buf, sizeof(buf), "Min Speed: %.1f", defProps.minSafeSpeed);
        renderer.drawText(x + 10, py, buf, labelColor, 13);
        py += lineH;

        snprintf(buf, sizeof(buf), "Max Speed: %.1f", defProps.maxSafeSpeed);
        renderer.drawText(x + 10, py, buf, labelColor, 13);
        py += lineH;

        snprintf(buf, sizeof(buf), "G-Force: %.1fx", defProps.gForceMultiplier);
        renderer.drawText(x + 10, py, buf, labelColor, 13);
        py += lineH;

        if (m_selectedElement >= 0 && m_selectedElement < static_cast<int>(m_editorElements.size())) {
            py += 10;
            SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 60, 60, 70, 255);
            SDL_Rect sep = { x + 10, py - 3, width - 20, 1 };
            SDL_RenderFillRect(renderer.getSDLRenderer(), &sep);
            py += 10;

            auto& elem = m_editorElements[m_selectedElement];
            if (elem.isToken) {
                renderer.drawText(x + 10, py, "Token", titleColor, 14);
                py += lineH;
                snprintf(buf, sizeof(buf), "Speed: %.0f-%.0f", elem.speedMin, elem.speedMax);
                renderer.drawText(x + 10, py, buf, labelColor, 13);
            } else {
                renderer.drawText(x + 10, py, "Power-Up", titleColor, 14);
                py += lineH;
                renderer.drawText(
                    x + 10, py, TrackElements::getPowerUpName(static_cast<PowerUpType>(elem.type)), labelColor, 13);
            }
            py += lineH;
        }
    } else {
        SDL_Color hintColor = { 120, 120, 120, 255 };
        renderer.drawText(x + 10, y + 40, "Select a segment", hintColor, 13);
        renderer.drawText(x + 10, y + 58, "to edit properties", hintColor, 13);
    }

    int buttonsY = y + height - 120;
    SDL_Color btnText = { 255, 255, 255, 255 };

    auto drawButton = [&](int bx, int by, int bw, int bh, const char* text, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer.getSDLRenderer(), color.r, color.g, color.b, color.a);
        SDL_Rect btn = { bx, by, bw, bh };
        SDL_RenderFillRect(renderer.getSDLRenderer(), &btn);
        renderer.drawText(bx + 8, by + bh / 2 - 8, text, btnText, 13);
    };

    drawButton(x + 10, buttonsY, width - 20, 28, "Play Test", { 60, 160, 70, 255 });
    drawButton(x + 10, buttonsY + 34, width - 20, 28, "Save", { 60, 100, 180, 255 });
    drawButton(x + 10, buttonsY + 68, width - 20, 28, "Validate", { 180, 100, 60, 255 });
}

void LevelEditor::drawSegmentPreview(Renderer& renderer, int x, int y, int segType, bool valid) {
    (void)valid;
    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 200, 200, 200, 200);

    switch (static_cast<SegmentType>(segType)) {
        case SegmentType::Straight:
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x, y + 15, x + 30, y + 15);
            break;
        case SegmentType::Curve:
            for (int a = 0; a < 90; a += 10) {
                float rad1 = a * 3.14159f / 180.0f;
                float rad2 = (a + 10) * 3.14159f / 180.0f;
                int x1 = x + 15 + static_cast<int>(12.0f * cosf(rad1 - 3.14159f / 2));
                int y1 = y + 15 + static_cast<int>(12.0f * sinf(rad1 - 3.14159f / 2));
                int x2 = x + 15 + static_cast<int>(12.0f * cosf(rad2 - 3.14159f / 2));
                int y2 = y + 15 + static_cast<int>(12.0f * sinf(rad2 - 3.14159f / 2));
                SDL_RenderDrawLine(renderer.getSDLRenderer(), x1, y1, x2, y2);
            }
            break;
        case SegmentType::Drop:
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x, y + 5, x + 15, y + 25);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 15, y + 25, x + 30, y + 25);
            break;
        case SegmentType::Jump:
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x, y + 20, x + 10, y + 5);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 10, y + 5, x + 20, y + 20);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 20, y + 20, x + 30, y + 10);
            break;
        case SegmentType::Loop:
            for (int a = 0; a < 360; a += 20) {
                float rad1 = a * 3.14159f / 180.0f;
                float rad2 = (a + 20) * 3.14159f / 180.0f;
                int x1 = x + 15 + static_cast<int>(10.0f * cosf(rad1));
                int y1 = y + 15 + static_cast<int>(10.0f * sinf(rad1));
                int x2 = x + 15 + static_cast<int>(10.0f * cosf(rad2));
                int y2 = y + 15 + static_cast<int>(10.0f * sinf(rad2));
                SDL_RenderDrawLine(renderer.getSDLRenderer(), x1, y1, x2, y2);
            }
            break;
        case SegmentType::Tunnel:
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x, y + 10, x + 30, y + 10);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x, y + 20, x + 30, y + 20);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x, y + 10, x, y + 20);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 30, y + 10, x + 30, y + 20);
            break;
        case SegmentType::WaterSplash:
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x, y + 20, x + 30, y + 20);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 5, y + 15, x + 5, y + 20);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 15, y + 12, x + 15, y + 20);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 25, y + 15, x + 25, y + 20);
            break;
        case SegmentType::FallingTrack:
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x, y + 10, x + 15, y + 10);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 15, y + 10, x + 20, y + 22);
            SDL_RenderDrawLine(renderer.getSDLRenderer(), x + 20, y + 22, x + 30, y + 22);
            break;
        case SegmentType::COUNT:
            break;
    }
}

void LevelEditor::drawConnectionDots(Renderer& renderer, int startX, int startY, int endX, int endY, bool valid) {
    (void)endY;
    if (valid) {
        SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 100, 220, 100, 200);
    } else {
        SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 220, 80, 80, 200);
    }
    SDL_Rect startDot = { startX - 4, startY - 4, 8, 8 };
    SDL_RenderFillRect(renderer.getSDLRenderer(), &startDot);
    SDL_Rect endDot = { endX - 4, startY - 4, 8, 8 };
    SDL_RenderFillRect(renderer.getSDLRenderer(), &endDot);
}

void LevelEditor::drawGrid(Renderer& renderer, int x, int y, int width, int height) {
    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 35, 35, 42, 255);
    int gridSize = m_canvasZoom;

    int startX = x + (m_scrollOffset % gridSize);
    int startY = y + (m_scrollOffset % gridSize);

    for (int gx = startX; gx < x + width; gx += gridSize) {
        SDL_RenderDrawLine(renderer.getSDLRenderer(), gx, y, gx, y + height);
    }
    for (int gy = startY; gy < y + height; gy += gridSize) {
        SDL_RenderDrawLine(renderer.getSDLRenderer(), x, gy, x + width, gy);
    }

    int centerX = x + width / 2;
    int centerY = y + height / 2;
    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 60, 60, 70, 255);
    SDL_RenderDrawLine(renderer.getSDLRenderer(), centerX, y, centerX, y + height);
    SDL_RenderDrawLine(renderer.getSDLRenderer(), x, centerY, x + width, centerY);
}

void LevelEditor::drawElementIcons(Renderer& renderer, int x, int y, const EditorElement& elem) {
    if (elem.isToken) {
        SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 255, 220, 50, 255);
    } else {
        auto puType = static_cast<PowerUpType>(elem.type);
        switch (puType) {
            case PowerUpType::NitroBoost:
                SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 255, 100, 50, 255);
                break;
            case PowerUpType::StickyWheels:
                SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 100, 200, 255, 255);
                break;
            case PowerUpType::Wings:
                SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 200, 100, 255, 255);
                break;
            case PowerUpType::Magnet:
                SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 255, 100, 100, 255);
                break;
            case PowerUpType::SlowMotion:
                SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 100, 255, 200, 255);
                break;
            default:
                SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 200, 200, 200, 255);
                break;
        }
    }

    SDL_Rect icon = { x - 5, y - 5, 10, 10 };
    SDL_RenderFillRect(renderer.getSDLRenderer(), &icon);
    SDL_SetRenderDrawColor(renderer.getSDLRenderer(), 255, 255, 255, 200);
    SDL_Rect border = { x - 5, y - 5, 10, 10 };
    SDL_RenderDrawRect(renderer.getSDLRenderer(), &border);
}

void LevelEditor::handleInput(const InputState& input) {
    (void)input;
}

void LevelEditor::handleCanvasClick(int mouseX, int mouseY, int canvasX, int canvasY, int canvasW, int canvasH) {
    int segIdx = segmentAtPosition(canvasX, canvasY, canvasW, canvasH, mouseX, mouseY);
    if (segIdx >= 0) {
        switch (m_currentTool) {
            case EditorTool::Select:
                selectSegment(segIdx);
                break;
            case EditorTool::PlaceSegment:
                addSegmentAfter(segIdx, m_brushType);
                break;
            case EditorTool::Delete:
                deleteSegment(segIdx);
                break;
            default:
                selectSegment(segIdx);
                break;
        }
    }
}

void LevelEditor::handlePaletteClick(int mouseX, int mouseY, int palX, int palY, int palW, int palH) {
    (void)mouseX;
    (void)palX;
    (void)palW;
    (void)palH;
    int numTypes = static_cast<int>(SegmentType::COUNT);
    int slotH = 50;
    int startY = palY + 35;

    for (int i = 0; i < numTypes; ++i) {
        int sy = startY + i * (slotH + 4);
        if (mouseY >= sy && mouseY < sy + slotH) {
            m_brushType = i;
            m_currentTool = EditorTool::PlaceSegment;
            break;
        }
    }
}

int LevelEditor::segmentAtPosition(int canvasX, int canvasY, int cw, int ch, int mx, int my) const {
    (void)canvasY;
    (void)ch;
    int centerX = canvasX + cw / 2;
    int trackY = canvasY + ch / 2;
    int segSpacing = m_canvasZoom;
    int segCount = static_cast<int>(m_editorSegments.size());

    for (int i = 0; i < segCount; ++i) {
        int sx = centerX + (i - segCount / 2) * segSpacing;
        int sy = trackY + static_cast<int>(m_editorSegments[i].heightDelta * 1.5f);

        if (std::abs(mx - sx) < segSpacing / 2 && std::abs(my - sy) < 20) {
            return i;
        }
    }

    return -1;
}

void LevelEditor::selectSegment(int index) {
    if (index >= 0 && index < static_cast<int>(m_editorSegments.size())) {
        m_selectedSegment = index;
        m_selectedElement = -1;
    }
}

void LevelEditor::deleteSegment(int index) {
    if (index < 0 || index >= static_cast<int>(m_editorSegments.size()))
        return;

    m_editorSegments.erase(m_editorSegments.begin() + index);

    for (auto& elem : m_editorElements) {
        if (elem.segmentIndex > index) {
            elem.segmentIndex--;
        }
    }
    m_editorElements.erase(
        std::remove_if(
            m_editorElements.begin(), m_editorElements.end(),
            [index](const EditorElement& e) { return e.segmentIndex == index; }),
        m_editorElements.end());

    if (m_selectedSegment >= static_cast<int>(m_editorSegments.size())) {
        m_selectedSegment = static_cast<int>(m_editorSegments.size()) - 1;
    }

    rebuildLayout();
}

void LevelEditor::addSegmentAfter(int index, int typeIndex) {
    EditorSegment seg;
    seg.typeIndex = typeIndex;
    seg.heightDelta = TrackSegment::getDefaultProperties(static_cast<SegmentType>(typeIndex)).heightDelta;

    if (index >= 0 && index < static_cast<int>(m_editorSegments.size())) {
        m_editorSegments.insert(m_editorSegments.begin() + index + 1, seg);
        for (auto& elem : m_editorElements) {
            if (elem.segmentIndex > index) {
                elem.segmentIndex++;
            }
        }
    } else {
        m_editorSegments.push_back(seg);
    }

    rebuildLayout();
}

void LevelEditor::newTrack(int segmentCount) {
    m_editorSegments.clear();
    m_editorElements.clear();
    m_selectedSegment = -1;
    m_selectedElement = -1;

    for (int i = 0; i < segmentCount; ++i) {
        EditorSegment seg;
        seg.typeIndex = static_cast<int>(SegmentType::Straight);
        if (i > 0 && i < segmentCount - 1) {
            int variety = i % 5;
            if (variety == 1)
                seg.typeIndex = static_cast<int>(SegmentType::Curve);
            else if (variety == 2)
                seg.typeIndex = static_cast<int>(SegmentType::Drop);
            else if (variety == 3) {
                seg.typeIndex = static_cast<int>(SegmentType::Loop);
                seg.heightDelta = 50.0f;
            }
        }
        m_editorSegments.push_back(seg);
    }

    rebuildLayout();
}

bool LevelEditor::loadTrack(const std::string& path) {
    TrackBuilder builder;
    m_layout = builder.loadFromFile(path);
    if (!m_layout || m_layout->getSegmentCount() == 0) {
        m_statusMessage = "Failed to load track from: " + path;
        m_statusTimer = 3.0f;
        return false;
    }

    m_editorSegments.clear();
    for (int i = 0; i < m_layout->getSegmentCount(); ++i) {
        auto* seg = m_layout->getSegment(i);
        if (seg) {
            EditorSegment es;
            es.typeIndex = static_cast<int>(seg->getType());
            es.heightDelta = seg->getHeightDelta();
            m_editorSegments.push_back(es);
        }
    }

    m_currentPath = path;
    m_statusMessage = "Loaded track: " + path;
    m_statusTimer = 3.0f;
    return true;
}

bool LevelEditor::saveTrack(const std::string& path) {
    rebuildLayout();
    TrackBuilder builder;
    if (!builder.saveToFile(*m_layout, path)) {
        m_statusMessage = "Failed to save track to: " + path;
        m_statusTimer = 3.0f;
        return false;
    }
    m_currentPath = path;
    m_statusMessage = "Saved track to: " + path;
    m_statusTimer = 3.0f;
    return true;
}

void LevelEditor::playTest() {
    if (m_editorSegments.empty())
        return;
    rebuildLayout();

    m_playTesting = true;
    m_playTestSpeed = 20.0f;
    m_playTestSegment = 0;

    Train testTrain(static_cast<TrainType>(m_trainPreview));
    m_statusMessage = "Play testing with " +
        std::string(m_trainPreview == 0       ? "Light"
                        : m_trainPreview == 1 ? "Medium"
                                              : "Heavy") +
        " train";
    m_statusTimer = 3.0f;
}

void LevelEditor::stopPlayTest() {
    m_playTesting = false;
    m_playTestSpeed = 0.0f;
    m_playTestSegment = 0;
}

bool LevelEditor::validateTrack() {
    rebuildLayout();

    TrackValidator validator;
    ValidationResult result;

    if (m_elements && m_elements->getTokenCount() > 0) {
        result = validator.validateWithElements(*m_layout, *m_elements);
    } else {
        result = validator.validate(*m_layout);
    }

    if (result.valid) {
        m_statusMessage = "Track is valid! " + std::to_string(m_layout->getSegmentCount()) + " segments, " +
            std::to_string(m_elements ? m_elements->getTokenCount() : 0) + " tokens";
        m_statusTimer = 4.0f;
    } else {
        m_statusMessage = "Track has " + std::to_string(result.errors.size()) + " errors";
        if (!result.warnings.empty()) {
            m_statusMessage += ", " + std::to_string(result.warnings.size()) + " warnings";
        }
        m_statusTimer = 5.0f;
    }

    return result.valid;
}

void LevelEditor::rebuildLayout() {
    m_layout = std::make_unique<TrackLayout>();

    for (auto& es : m_editorSegments) {
        auto segType = static_cast<SegmentType>(es.typeIndex);
        auto props = TrackSegment::getDefaultProperties(segType);
        if (es.heightDelta != 0.0f) {
            props.heightDelta = es.heightDelta;
        }
        m_layout->addSegment(std::make_unique<TrackSegment>(segType, props));
    }
}

void LevelEditor::moveSegment(int fromIndex, int toIndex) {
    if (fromIndex < 0 || fromIndex >= static_cast<int>(m_editorSegments.size()))
        return;
    if (toIndex < 0 || toIndex >= static_cast<int>(m_editorSegments.size()))
        return;
    if (fromIndex == toIndex)
        return;

    EditorSegment seg = m_editorSegments[fromIndex];
    m_editorSegments.erase(m_editorSegments.begin() + fromIndex);
    m_editorSegments.insert(m_editorSegments.begin() + toIndex, seg);

    for (auto& elem : m_editorElements) {
        if (elem.segmentIndex == fromIndex) {
            elem.segmentIndex = toIndex;
        } else if (fromIndex < toIndex) {
            if (elem.segmentIndex > fromIndex && elem.segmentIndex <= toIndex) {
                elem.segmentIndex--;
            }
        } else {
            if (elem.segmentIndex >= toIndex && elem.segmentIndex < fromIndex) {
                elem.segmentIndex++;
            }
        }
    }

    m_selectedSegment = toIndex;
    rebuildLayout();
}

} // namespace rr99
