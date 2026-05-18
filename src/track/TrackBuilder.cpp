#include "TrackBuilder.h"

#include "TrackElements.h"
#include "TrackLayout.h"
#include "core/Random.h"
#include "physics/TrackSegment.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

namespace rr99 {

std::unique_ptr<TrackLayout> TrackBuilder::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        return nullptr;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    TrackDefinition def;
    def.levelNumber = 0;
    def.theme = "default";

    auto findValue = [&](const std::string& key) -> std::string {
        auto pos = content.find("\"" + key + "\"");
        if (pos == std::string::npos)
            return "";
        pos = content.find(':', pos);
        if (pos == std::string::npos)
            return "";
        pos = content.find_first_of("\"0123456789", pos);
        if (pos == std::string::npos)
            return "";
        if (content[pos] == '"') {
            auto end = content.find('"', pos + 1);
            return content.substr(pos + 1, end - pos - 1);
        }
        auto end = content.find_first_of(",\n\r}", pos);
        return content.substr(pos, end - pos);
    };

    auto levelStr = findValue("level");
    if (!levelStr.empty())
        def.levelNumber = std::stoi(levelStr);

    auto themeStr = findValue("theme");
    if (!themeStr.empty())
        def.theme = themeStr;

    auto parStr = findValue("par_time");
    if (!parStr.empty())
        def.parTime = std::stof(parStr);

    auto weatherStr = findValue("weather");
    if (!weatherStr.empty())
        def.weather = weatherStr;

    auto trainStr = findValue("required_train");
    if (!trainStr.empty())
        def.requiredTrainType = std::stoi(trainStr);

    auto segStart = content.find("\"segments\"");
    if (segStart != std::string::npos) {
        segStart = content.find('[', segStart);
        if (segStart != std::string::npos) {
            auto segEnd = content.find(']', segStart);
            std::string segSection = content.substr(segStart, segEnd - segStart + 1);

            size_t p = 0;
            while ((p = segSection.find('{', p)) != std::string::npos) {
                auto endBrace = segSection.find('}', p);
                if (endBrace == std::string::npos)
                    break;
                std::string seg = segSection.substr(p, endBrace - p + 1);

                auto tPos = seg.find("\"type\"");
                if (tPos != std::string::npos) {
                    auto colon = seg.find(':', tPos);
                    auto valStart = seg.find_first_of("\"0123456789", colon);
                    if (valStart != std::string::npos) {
                        if (seg[valStart] == '"') {
                            auto valEnd = seg.find('"', valStart + 1);
                            std::string typeName = seg.substr(valStart + 1, valEnd - valStart - 1);
                            def.segmentTypes.push_back(parseSegmentType(typeName));
                        } else {
                            auto valEnd = seg.find_first_of(",\n\r}", valStart);
                            def.segmentTypes.push_back(std::stoi(seg.substr(valStart, valEnd - valStart)));
                        }
                    }
                }

                auto hPos = seg.find("\"height\"");
                if (hPos != std::string::npos) {
                    auto colon = seg.find(':', hPos);
                    auto valStart = seg.find_first_of("-0123456789", colon);
                    auto valEnd = seg.find_first_of(",\n\r}", valStart);
                    def.segmentHeights.push_back(std::stof(seg.substr(valStart, valEnd - valStart)));
                } else {
                    def.segmentHeights.push_back(0.0f);
                }

                p = endBrace + 1;
            }
        }
    }

    return loadFromDefinition(def);
}

std::unique_ptr<TrackLayout> TrackBuilder::loadFromDefinition(const TrackDefinition& def) {
    auto layout = std::make_unique<TrackLayout>();

    TrackMetadata meta;
    meta.levelNumber = def.levelNumber;
    meta.theme = def.theme;
    meta.parTime = def.parTime;
    meta.weather = def.weather;
    if (def.requiredTrainType >= 0) {
        meta.requiredTrain = static_cast<TrainType>(def.requiredTrainType);
        meta.hasRequiredTrain = true;
    }
    layout->setMetadata(meta);

    for (size_t i = 0; i < def.segmentTypes.size(); ++i) {
        int typeIndex = def.segmentTypes[i];
        if (typeIndex < 0 || typeIndex >= static_cast<int>(SegmentType::COUNT)) {
            typeIndex = 0;
        }
        auto segType = static_cast<SegmentType>(typeIndex);
        SegmentProperties props = TrackSegment::getDefaultProperties(segType);

        if (i < def.segmentHeights.size() && def.segmentHeights[i] != 0.0f) {
            props.heightDelta = def.segmentHeights[i];
        }

        if (segType == SegmentType::Drop && props.heightDelta >= 0.0f) {
            props.heightDelta = -std::abs(props.heightDelta);
        }

        layout->addSegment(std::make_unique<TrackSegment>(segType, props));
    }

    return layout;
}

bool TrackBuilder::saveToFile(const TrackLayout& layout, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open())
        return false;

    const auto& meta = layout.getMetadata();

    file << "{\n";
    file << "    \"level\": " << meta.levelNumber << ",\n";
    file << "    \"theme\": \"" << meta.theme << "\",\n";
    file << "    \"par_time\": " << meta.parTime << ",\n";
    if (!meta.weather.empty()) {
        file << "    \"weather\": \"" << meta.weather << "\",\n";
    }
    if (meta.hasRequiredTrain) {
        file << "    \"required_train\": " << static_cast<int>(meta.requiredTrain) << ",\n";
    }
    file << "    \"segments\": [\n";

    for (int i = 0; i < layout.getSegmentCount(); ++i) {
        auto* seg = layout.getSegment(i);
        file << "        { \"type\": " << static_cast<int>(seg->getType());
        if (seg->getHeightDelta() != 0.0f) {
            file << ", \"height\": " << seg->getHeightDelta();
        }
        file << " }";
        if (i < layout.getSegmentCount() - 1) {
            file << ",";
        }
        file << "\n";
    }

    file << "    ]\n";
    file << "}\n";

    return true;
}

std::unique_ptr<TrackLayout> TrackBuilder::generateProcedural(int level, int segmentCount) {
    TrackDefinition def = generateDefinition(level, segmentCount);
    auto layout = loadFromDefinition(def);

    TrackMetadata meta = layout->getMetadata();
    meta.levelNumber = level;
    meta.hasRequiredTrain = false;
    layout->setMetadata(meta);

    return layout;
}

void TrackBuilder::generateProceduralElements(TrackElements& elements, const TrackLayout& layout, int difficulty) {
    elements.clear();
    (void)difficulty;

    int segCount = layout.getSegmentCount();
    if (segCount == 0)
        return;

    int tokenCount = 5 + Random::nextInt(0, 5);
    for (int i = 0; i < tokenCount; ++i) {
        SmileToken token;
        token.segmentIndex = Random::nextInt(2, segCount - 1);
        token.position = Random::nextFloat() * 0.8f + 0.1f;
        token.speedMin = 10.0f + Random::nextFloat() * 30.0f;
        token.speedMax = token.speedMin + 20.0f + Random::nextFloat() * 20.0f;

        auto* seg = layout.getSegment(token.segmentIndex);
        if (seg) {
            token.speedMin = std::max(token.speedMin, seg->getMinSafeSpeed());
            token.speedMax = std::min(token.speedMax, seg->getMaxSafeSpeed());
            if (token.speedMin >= token.speedMax) {
                token.speedMin = seg->getMinSafeSpeed();
                token.speedMax = seg->getMaxSafeSpeed();
            }
        }

        elements.addToken(token);
    }

    int powerUpCount = 1 + Random::nextInt(0, 3);
    for (int i = 0; i < powerUpCount; ++i) {
        PowerUp pu;
        pu.type = static_cast<PowerUpType>(Random::nextInt(0, static_cast<int>(PowerUpType::COUNT) - 1));
        pu.segmentIndex = Random::nextInt(1, segCount - 1);
        pu.position = Random::nextFloat() * 0.8f + 0.1f;
        pu.activeDuration = TrackElements::getPowerUpDuration(pu.type);
        elements.addPowerUp(pu);
    }
}

TrackDefinition TrackBuilder::generateDefinition(int level, int segmentCount) {
    TrackDefinition def;
    def.levelNumber = level;
    def.theme = "default";
    def.parTime = 30.0f + segmentCount * 2.0f;
    def.requiredTrainType = -1;

    SegmentType baseTypes[] = {
        SegmentType::Straight, SegmentType::Curve,  SegmentType::Drop,        SegmentType::Jump,
        SegmentType::Loop,     SegmentType::Tunnel, SegmentType::WaterSplash, SegmentType::FallingTrack
    };

    int numBaseTypes = static_cast<int>(sizeof(baseTypes) / sizeof(baseTypes[0]));

    for (int i = 0; i < segmentCount; ++i) {
        int typeIndex;
        if (i < 2) {
            typeIndex = static_cast<int>(SegmentType::Straight);
        } else if (i >= segmentCount - 2) {
            typeIndex = static_cast<int>(SegmentType::Straight);
        } else {
            typeIndex = Random::nextInt(0, numBaseTypes - 1);

            if (level < 5 && typeIndex > static_cast<int>(SegmentType::Curve)) {
                typeIndex = Random::nextInt(0, static_cast<int>(SegmentType::Curve));
            }
        }
        def.segmentTypes.push_back(typeIndex);

        float height = 0.0f;
        auto segType = static_cast<SegmentType>(typeIndex);
        if (segType == SegmentType::Drop) {
            height = -(10.0f + Random::nextFloat() * 30.0f * (1.0f + level * 0.05f));
        } else if (segType == SegmentType::Jump) {
            height = -(5.0f + Random::nextFloat() * 15.0f);
        } else if (segType == SegmentType::Loop) {
            height = 40.0f + Random::nextFloat() * 20.0f;
        } else if (segType == SegmentType::FallingTrack) {
            height = -(20.0f + Random::nextFloat() * 30.0f);
        } else if (segType == SegmentType::WaterSplash) {
            height = -(3.0f + Random::nextFloat() * 5.0f);
        }
        def.segmentHeights.push_back(height);
    }

    return def;
}

int TrackBuilder::parseSegmentType(const std::string& typeName) const {
    if (typeName == "straight")
        return 0;
    if (typeName == "curve")
        return 1;
    if (typeName == "drop")
        return 2;
    if (typeName == "jump")
        return 3;
    if (typeName == "loop")
        return 4;
    if (typeName == "tunnel")
        return 5;
    if (typeName == "water_splash")
        return 6;
    if (typeName == "falling_track")
        return 7;
    return 0;
}

} // namespace rr99
