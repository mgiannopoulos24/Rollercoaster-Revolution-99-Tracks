#ifndef RR99_TOOLS_LEVELEDITOR_H
#define RR99_TOOLS_LEVELEDITOR_H

#include <memory>
#include <string>
#include <vector>

namespace rr99 {

class TrackLayout;
class TrackElements;
class Renderer;
class EventBus;
struct InputState;

struct EditorSegment {
    int typeIndex = 0;
    float heightDelta = 0.0f;
    float customLength = 0.0f;
};

struct EditorElement {
    int type = 0;
    int segmentIndex = 0;
    float position = 0.5f;
    float speedMin = 0.0f;
    float speedMax = 100.0f;
    bool isToken = true;
};

enum class EditorTool { Select, PlaceSegment, PlaceToken, PlacePowerUp, PlaceHazard, Delete };

class LevelEditor {
public:
    LevelEditor();
    ~LevelEditor() = default;

    bool initialize();
    void tick(float dt);
    void draw(Renderer& renderer);
    void handleInput(const InputState& input);

    bool isVisible() const {
        return m_visible;
    }
    void setVisible(bool visible) {
        m_visible = visible;
    }

    void newTrack(int segmentCount);
    bool loadTrack(const std::string& path);
    bool saveTrack(const std::string& path);

private:
    void drawPalette(Renderer& renderer, int x, int y, int width, int height);
    void drawCanvas(Renderer& renderer, int x, int y, int width, int height);
    void drawProperties(Renderer& renderer, int x, int y, int width, int height);
    void drawToolbar(Renderer& renderer, int x, int y, int width, int height);
    void drawSegmentPreview(Renderer& renderer, int x, int y, int segType, bool valid);
    void drawConnectionDots(Renderer& renderer, int startX, int startY, int endX, int endY, bool valid);
    void drawGrid(Renderer& renderer, int x, int y, int width, int height);
    void drawElementIcons(Renderer& renderer, int x, int y, const EditorElement& elem);

    void handleCanvasClick(int mouseX, int mouseY, int canvasX, int canvasY, int canvasW, int canvasH);
    void handlePaletteClick(int mouseX, int mouseY, int palX, int palY, int palW, int palH);

    int segmentAtPosition(int canvasX, int canvasY, int cw, int ch, int mx, int my) const;

    void selectSegment(int index);
    void deleteSegment(int index);
    void moveSegment(int fromIndex, int toIndex);
    void addSegmentAfter(int index, int typeIndex);

    void playTest();
    void stopPlayTest();
    bool validateTrack();

    void rebuildLayout();

    bool m_visible = false;
    bool m_playTesting = false;

    EditorTool m_currentTool = EditorTool::PlaceSegment;
    int m_selectedSegment = -1;
    int m_selectedElement = -1;
    int m_brushType = 0;
    int m_trainPreview = 0;

    std::vector<EditorSegment> m_editorSegments;
    std::vector<EditorElement> m_editorElements;

    std::unique_ptr<TrackLayout> m_layout;
    std::unique_ptr<TrackElements> m_elements;

    int m_scrollOffset = 0;
    int m_canvasZoom = 40;

    float m_playTestSpeed = 0.0f;
    int m_playTestSegment = 0;

    std::string m_currentPath;
    std::string m_statusMessage;
    float m_statusTimer = 0.0f;
};

} // namespace rr99

#endif // RR99_TOOLS_LEVELEDITOR_H
