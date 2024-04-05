#pragma once

#include "camera.h"
#include "light.h"
#include "node.h"

#include <array>
#include <memory>

namespace proto {

struct Scene {
	GlobalLight global_light;
	std::array<PointLight, 4> lights;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	std::vector<Node> nodes;

	std::shared_ptr<Camera> get_camera();
	void add_node(Node node);
	void clear_nodes();
};

} // namespace proto
