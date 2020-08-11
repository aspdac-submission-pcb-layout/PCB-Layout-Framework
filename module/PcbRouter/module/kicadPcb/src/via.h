#ifndef KICAD_PCB_VIA_H
#define KICAD_PCB_VIA_H

#include <string>
#include <vector>
#include "point.h"

enum class ViaType {
    THROUGH,
    MICRO,
    BLIND_BURIED
};

class Via {
   public:
    Via(const int id = -1,
        const int netId = -1,
        const double size = 0.0,
        const ViaType type = ViaType::THROUGH)
        : m_id(id), m_netId(netId), m_size(size), m_type(type) {}

    int getId() { return m_id; }
    int getNetId() { return m_netId; }
    double getSize() { return m_size; }
    double getDrillSize() { return m_drill_size; }
    point_2d &getPos() { return m_pos; }
    std::vector<std::string> &getLayers() { return m_layers; }  //layers[0] start layer; layers[1] end layer
    ViaType getType() { return m_type; }

    void setType(const ViaType &type) { m_type = type; }
    void setSize(const double &size) { m_size = size; }
    void setDrillSize(const double &size) { m_drill_size = size; }
    void setPosition(const point_2d &_pos) { m_pos = std::move(_pos); }
    void setLayer(const std::vector<std::string> &_layers) { m_layers = std::move(_layers); }

   private:
    int m_id;
    int m_netId;
    double m_size;  //Diameter
    double m_drill_size;
    std::vector<std::string> m_layers;
    point_2d m_pos;
    ViaType m_type;
};

#endif
