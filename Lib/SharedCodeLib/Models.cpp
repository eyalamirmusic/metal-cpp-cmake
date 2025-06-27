#include "Models.h"
#include "GLM.h"
#include <sstream>
#include <tiny_obj_loader.h>

namespace Graphics
{
Model loadModelFromObjData(const std::string& objData, const std::string& mtlData)
{
    Model model;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::istringstream objStream(objData);
    std::istringstream mtlStream(mtlData);

    tinyobj::MaterialStreamReader matReader(mtlStream);

    bool ok = tinyobj::LoadObj(
        &attrib, &shapes, &materials, &warn, &err, &objStream, &matReader);

    if (!ok)
        throw std::runtime_error("Failed to load model from memory: " + err);

    for (const auto& shape: shapes)
    {
        size_t index_offset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            int fv = shape.mesh.num_face_vertices[f];
            int materialID = shape.mesh.material_ids[f];

            // Default color (white)
            glm::vec3 diffuseColor {1.0f, 1.0f, 1.0f};

            if (materialID >= 0 && materialID < materials.size())
            {
                const auto& mat = materials[materialID];
                diffuseColor =
                    glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
            }

            for (int v = 0; v < fv; v++)
            {
                const auto& idx = shape.mesh.indices[index_offset + v];

                glm::vec3 pos {attrib.vertices[3 * idx.vertex_index + 0],
                               attrib.vertices[3 * idx.vertex_index + 1],
                               attrib.vertices[3 * idx.vertex_index + 2]};

                model.positions.push_back(pos);
                model.colors.push_back(diffuseColor);
            }

            index_offset += fv;
        }
    }

    return model;
}

} // namespace Graphics